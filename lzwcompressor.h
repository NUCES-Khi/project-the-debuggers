#ifndef LZWCOMPRESSOR_H
#define LZWCOMPRESSOR_H

#include <QByteArray>
#include <QString>

class LZWCompressor {
private:
    static const int MAX_DICT_SIZE = 4096;  // max dictionary entries

    // dictionary entry structure
    struct DictEntry {
        QString str;
        int code;
        bool used;
        DictEntry() : code(-1), used(false) {}
    };

    DictEntry dictionary[4096];         // for compression
    QString reverseDictionary[4096];    // for decompression

    void initDictionary();  // setup initial 256 entries
    int findInDictionary(const QString& s);

public:
    LZWCompressor() {}
    ~LZWCompressor() {}

    QByteArray compress(const QByteArray& input);
    QByteArray decompress(const QByteArray& input);
};

#endif // LZWCOMPRESSOR_H
