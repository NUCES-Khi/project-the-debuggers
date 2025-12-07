#include "lzwcompressor.h"
#include "lzwcompressor.h"
#include <QByteArray>

struct HashEntry {
    QByteArray sequence;
    int code;
    bool used;
    HashEntry() : code(-1), used(false) {}
};

static unsigned int hashBytes(const QByteArray& data) {
    unsigned int hash = 5381;
    for(int i = 0; i < data.size(); i++) {
        hash = ((hash << 5) + hash) + (unsigned char)data[i];
    }
    return hash;
}

void LZWCompressor::initDictionary() {
    for(int i = 0; i < 256; i++) {
        dictionary[i].str = QString(QChar(i));
        dictionary[i].code = i;
        dictionary[i].used = true;

        reverseDictionary[i] = QString(QChar(i));
    }

    for(int i = 256; i < MAX_DICT_SIZE; i++) {
        dictionary[i].used = false;
        reverseDictionary[i] = "";
    }
}

int LZWCompressor::findInDictionary(const QString& s) {
    // Linear search (inefficient but works)
    for(int i = 0; i < MAX_DICT_SIZE; i++) {
        if(dictionary[i].used && dictionary[i].str == s) {
            return dictionary[i].code;
        }
    }
    return -1;
}

QByteArray LZWCompressor::compress(const QByteArray& input) {
    if(input.isEmpty()) return QByteArray();

    initDictionary();
    int dictSize = 256;

    QByteArray result;

    // Write original size (4 bytes)
    int origSize = input.size();
    result.append((char)(origSize & 0xFF));
    result.append((char)((origSize >> 8) & 0xFF));
    result.append((char)((origSize >> 16) & 0xFF));
    result.append((char)((origSize >> 24) & 0xFF));

    QString current = "";

    for(int i = 0; i < input.size(); i++) {
        QChar ch = QChar((unsigned char)input[i]);
        QString combined = current + ch;

        if(findInDictionary(combined) != -1) {
            current = combined;
        } else {
            int code = findInDictionary(current);
            if(code != -1) {
                result.append((char)(code & 0xFF));
                result.append((char)((code >> 8) & 0x0F));
            }

            if(dictSize < MAX_DICT_SIZE) {
                dictionary[dictSize].str = combined;
                dictionary[dictSize].code = dictSize;
                dictionary[dictSize].used = true;
                dictSize++;
            }

            current = QString(ch);
        }
    }

    if(!current.isEmpty()) {
        int code = findInDictionary(current);
        if(code != -1) {
            result.append((char)(code & 0xFF));
            result.append((char)((code >> 8) & 0x0F));
        }
    }

    return result;
}

QByteArray LZWCompressor::decompress(const QByteArray& input) {
    if(input.size() < 6) return QByteArray();

    int origSize = ((unsigned char)input[0]) |
                   ((unsigned char)input[1] << 8) |
                   ((unsigned char)input[2] << 16) |
                   ((unsigned char)input[3] << 24);

    if(origSize <= 0 || origSize > 100000000) {
        return QByteArray();
    }

    initDictionary();
    int dictSize = 256;

    QByteArray result;
    result.reserve(origSize);

    if(input.size() < 6) return QByteArray();
    int prevCode = ((unsigned char)input[4]) |
                   (((unsigned char)input[5] & 0x0F) << 8);

    if(prevCode >= 256) return QByteArray();

    QString prevStr = reverseDictionary[prevCode];
    result.append(prevStr.toUtf8());

    for(int i = 6; i < input.size(); i += 2) {
        if(i + 1 >= input.size()) break;

        int code = ((unsigned char)input[i]) |
                   (((unsigned char)input[i+1] & 0x0F) << 8);

        QString entry;

        if(code < dictSize && !reverseDictionary[code].isEmpty()) {

            entry = reverseDictionary[code];
        } else if(code == dictSize) {

            entry = prevStr + prevStr[0];
        } else {

            break;
        }

        result.append(entry.toUtf8());


        if(result.size() >= origSize) {
            return result.left(origSize);
        }


        if(dictSize < MAX_DICT_SIZE) {
            reverseDictionary[dictSize] = prevStr + entry[0];
            dictSize++;
        }

        prevStr = entry;
    }

    return result;
}


