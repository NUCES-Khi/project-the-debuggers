#include "rlecompressor.h"

QByteArray RLECompressor::compress(const QByteArray& input) {
    if(input.isEmpty()) return QByteArray();

    int estimatedSize = 0;
    int i = 0;

    while(i < input.size()) {
        unsigned char current = input[i];
        int count = 1;

        while(i + count < input.size() &&
               input[i + count] == current &&
               count < 255) {
            count++;
        }

        estimatedSize += 2;
        i += count;
    }

    if(estimatedSize >= input.size()) {
        QByteArray result;

        result.append((char)0xFF);
        result.append((char)0xFF);
        result.append((char)0xFF);
        result.append((char)0xFF);

        int origSize = input.size();
        result.append((char)(origSize & 0xFF));
        result.append((char)((origSize >> 8) & 0xFF));
        result.append((char)((origSize >> 16) & 0xFF));
        result.append((char)((origSize >> 24) & 0xFF));

        result.append(input);

        return result;
    }

    QByteArray result;

    int origSize = input.size();
    result.append((char)(origSize & 0xFF));
    result.append((char)((origSize >> 8) & 0xFF));
    result.append((char)((origSize >> 16) & 0xFF));
    result.append((char)((origSize >> 24) & 0xFF));

    i = 0;
    while(i < input.size()) {
        unsigned char current = input[i];
        int count = 1;

        while(i + count < input.size() &&
               input[i + count] == current &&
               count < 255) {
            count++;
        }

        result.append((char)current);
        result.append((char)count);

        i += count;
    }

    return result;
}

QByteArray RLECompressor::decompress(const QByteArray& input) {
    if(input.size() < 4) return QByteArray();

    // Check for uncompressed marker
    if(input.size() >= 8 &&
        (unsigned char)input[0] == 0xFF &&
        (unsigned char)input[1] == 0xFF &&
        (unsigned char)input[2] == 0xFF &&
        (unsigned char)input[3] == 0xFF) {

        int origSize = ((unsigned char)input[4]) |
                       ((unsigned char)input[5] << 8) |
                       ((unsigned char)input[6] << 16) |
                       ((unsigned char)input[7] << 24);

        return input.mid(8, origSize);
    }

    int origSize = ((unsigned char)input[0]) |
                   ((unsigned char)input[1] << 8) |
                   ((unsigned char)input[2] << 16) |
                   ((unsigned char)input[3] << 24);

    if(origSize <= 0 || origSize > 100000000) {
        return QByteArray();
    }

    QByteArray result;
    result.reserve(origSize);

    for(int i = 4; i < input.size(); i += 2) {
        if(i + 1 >= input.size()) break;

        unsigned char ch = input[i];
        unsigned char count = input[i + 1];

        if(count == 0 || count > 255) break;

        for(int j = 0; j < count; j++) {
            result.append((char)ch);

            if(result.size() >= origSize) {
                return result.left(origSize);
            }
        }
    }

    if(result.size() != origSize) {
        return QByteArray();
    }

    return result;
}
