#ifndef RLECOMPRESSOR_H
#define RLECOMPRESSOR_H

#include <QByteArray>

class RLECompressor {
public:
    RLECompressor() {}
    ~RLECompressor() {}

    QByteArray compress(const QByteArray& input);
    QByteArray decompress(const QByteArray& input);
};

#endif
