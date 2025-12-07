#ifndef LZWCOMPRESSOR_H
#define LZWCOMPRESSOR_H

#include <QByteArray>
#include <QString>

class LZWCompressor {
private:
    static const int MAX_DICT_SIZE = 4096;

    struct DictEntry {
        QString str;
        int code;
        bool used;
        DictEntry() : code(-1), used(false) {}
    };

    DictEntry dictionary[4096];
    QString reverseDictionary[4096];

    void initDictionary();
    int findInDictionary(const QString& s);

public:
    LZWCompressor() {}
    ~LZWCompressor() {}

    QByteArray compress(const QByteArray& input);
    QByteArray decompress(const QByteArray& input);
};

#endif
