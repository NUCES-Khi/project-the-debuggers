#ifndef HUFFMANCOMPRESSOR_H
#define HUFFMANCOMPRESSOR_H

#include <QString>
#include <QByteArray>
#include "datastructures.h"

struct HuffmanNode {
    unsigned char character;
    unsigned long long frequency;
    HuffmanNode *left, *right;

    HuffmanNode() : character(0), frequency(0), left(nullptr), right(nullptr) {}

    HuffmanNode(unsigned char ch, unsigned long long freq)
        : character(ch), frequency(freq), left(nullptr), right(nullptr) {}

    bool isLeaf() const {
        return (left == nullptr && right == nullptr);
    }
};

class HuffmanCompressor {
private:
    HuffmanNode* root;
    FrequencyTable freqTable;
    CodeTable codeTable;

    void buildFrequencyTable(const QByteArray& data);
    HuffmanNode* buildHuffmanTree();
    void generateCodes(HuffmanNode* node, QString code);
    void deleteTree(HuffmanNode* node);

    // NEW: Binary serialization methods
    void serializeTreeBinary(HuffmanNode* node, QByteArray& output);
    HuffmanNode* deserializeTreeBinary(const QByteArray& data, int& pos);

public:
    HuffmanCompressor();
    ~HuffmanCompressor();

    QByteArray compress(const QByteArray& input);
    QByteArray decompress(const QByteArray& input);
};

#endif // HUFFMANCOMPRESSOR_H
