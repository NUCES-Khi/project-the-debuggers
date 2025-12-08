#include "huffmancompressor.h"

HuffmanCompressor::HuffmanCompressor() : root(nullptr) {}

HuffmanCompressor::~HuffmanCompressor() {
    if(root) deleteTree(root);
}

void HuffmanCompressor::deleteTree(HuffmanNode* node) {
    if(!node) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void HuffmanCompressor::buildFrequencyTable(const QByteArray& data) {
    freqTable = FrequencyTable(); // Reset frequency table
    for(int i = 0; i < data.size(); i++) {
        freqTable.increment((unsigned char)data[i]);
    }
}

HuffmanNode* HuffmanCompressor::buildHuffmanTree() {
    MaxHeap MaxHeap(256);

    DynamicArray<unsigned char> keys;
    DynamicArray<unsigned long long> values;
    freqTable.getAllEntries(keys, values);


    if(keys.size() == 0) return nullptr;

    for(int i = 0; i < keys.size(); i++) {
        HuffmanNode* node = new HuffmanNode(keys[i], values[i]);
        MaxHeap.insert(node);
    }


    if(MaxHeap.size() == 1) {
        return MaxHeap.extractMin();
    }

    while(MaxHeap.size() > 1) {
        HuffmanNode* left = MaxHeap.extractMin();
        HuffmanNode* right = MaxHeap.extractMin();

        HuffmanNode* parent = new HuffmanNode(0, left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;

        MaxHeap.insert(parent);
    }

    return MaxHeap.extractMin();
}

void HuffmanCompressor::generateCodes(HuffmanNode* node, QString code) {
    if(!node) return;

    if(node->isLeaf()) {
        codeTable.insert(node->character, code.isEmpty() ? "0" : code);
        return;
    }

    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}


void HuffmanCompressor::serializeTreeBinary(HuffmanNode* node, QByteArray& output) {
    if(!node) {
        output.append((char)0); // Null marker
        return;
    }

    if(node->isLeaf()) {
        output.append((char)1); // Leaf marker
        output.append((char)node->character); // Character value
    } else {
        output.append((char)2); // Internal node marker
        serializeTreeBinary(node->left, output);
        serializeTreeBinary(node->right, output);
    }
}

HuffmanNode* HuffmanCompressor::deserializeTreeBinary(const QByteArray& data, int& pos) {
    if(pos >= data.size()) return nullptr;

    unsigned char marker = (unsigned char)data[pos++];

    if(marker == 0) { // Null
        return nullptr;
    } else if(marker == 1) { // Leaf
        if(pos >= data.size()) return nullptr;
        unsigned char ch = (unsigned char)data[pos++];
        return new HuffmanNode(ch, 0);
    } else if(marker == 2) { // Internal
        HuffmanNode* node = new HuffmanNode(0, 0);
        node->left = deserializeTreeBinary(data, pos);
        node->right = deserializeTreeBinary(data, pos);
        return node;
    }

    return nullptr;
}

QByteArray HuffmanCompressor::compress(const QByteArray& input) {
    if(input.isEmpty()) return QByteArray();

    if(root) {
        deleteTree(root);
        root = nullptr;
    }

    buildFrequencyTable(input);
    root = buildHuffmanTree();

    if(!root) return QByteArray();

    codeTable = CodeTable(); // Reset code table
    generateCodes(root, "");

    // bit string
    QString bitString = "";
    for(int i = 0; i < input.size(); i++) {
        QString* code = codeTable.get((unsigned char)input[i]);
        if(code) {
            bitString += *code;
        }
    }

    QByteArray result;

    // Write original file size (8 bytes for large files)
    qint64 origSize = input.size();
    for(int i = 0; i < 8; i++) {
        result.append((char)((origSize >> (i * 8)) & 0xFF));
    }

    // Serialize tree in BINARY format
    QByteArray treeData;
    serializeTreeBinary(root, treeData);

    // Write tree size (4 bytes)
    int treeSize = treeData.size();
    result.append((char)(treeSize & 0xFF));
    result.append((char)((treeSize >> 8) & 0xFF));
    result.append((char)((treeSize >> 16) & 0xFF));
    result.append((char)((treeSize >> 24) & 0xFF));

    // Write tree data (BINARY)
    result.append(treeData);

    // Calculate padding
    int padding = (8 - (bitString.length() % 8)) % 8;
    result.append((char)padding);

    // Convert bit string to bytes
    for(int i = 0; i < bitString.length(); i += 8) {
        unsigned char byte = 0;
        for(int j = 0; j < 8 && (i + j) < bitString.length(); j++) {
            if(bitString[i + j] == '1') {
                byte |= (1 << (7 - j));
            }
        }
        result.append((char)byte);
    }

    return result;
}

QByteArray HuffmanCompressor::decompress(const QByteArray& input) {
    if(input.size() < 13) return QByteArray(); // 8 (size) + 4 (tree size) + 1 (padding) = 13 minimum

    // Clean up old tree
    if(root) {
        deleteTree(root);
        root = nullptr;
    }

    int pos = 0;

    // Read original file size (8 bytes)
    qint64 origSize = 0;
    for(int i = 0; i < 8; i++) {
        origSize |= ((qint64)(unsigned char)input[pos++]) << (i * 8);
    }

    // Read tree size (4 bytes)
    int treeSize = ((unsigned char)input[pos]) |
                   ((unsigned char)input[pos+1] << 8) |
                   ((unsigned char)input[pos+2] << 16) |
                   ((unsigned char)input[pos+3] << 24);
    pos += 4;

    if(pos + treeSize >= input.size()) return QByteArray();

    // Read tree data (BINARY)
    QByteArray treeData = input.mid(pos, treeSize);
    pos += treeSize;

    // Reconstruct tree
    int treePos = 0;
    root = deserializeTreeBinary(treeData, treePos);

    if(!root) return QByteArray();

    // Handle single character case
    if(root->isLeaf()) {
        QByteArray result;
        for(qint64 i = 0; i < origSize; i++) {
            result.append((char)root->character);
        }
        return result;
    }

    // Read padding
    if(pos >= input.size()) return QByteArray();
    int padding = (unsigned char)input[pos];
    pos++;

    // Decode
    QByteArray result;
    HuffmanNode* current = root;

    for(int i = pos; i < input.size(); i++) {
        unsigned char byte = (unsigned char)input[i];
        int bitsToRead = 8;

        // Last byte: consider padding
        if(i == input.size() - 1) {
            bitsToRead = 8 - padding;
        }

        for(int j = 7; j >= (8 - bitsToRead); j--) {
            if(!current) {
                current = root;
            }

            if(byte & (1 << j)) {
                if(current->right) {
                    current = current->right;
                }
            } else {
                if(current->left) {
                    current = current->left;
                }
            }

            if(current && current->isLeaf()) {
                result.append((char)current->character);

                // Stop if we've decoded all bytes
                if(result.size() >= origSize) {
                    return result;
                }

                current = root;
            }
        }
    }

    return result;
}

