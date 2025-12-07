#include "rlecompressor.h"

QByteArray RLECompressor::compress(const QByteArray& input) {
    if(input.isEmpty()) return QByteArray();

    // First, do a dry run to see if RLE will actually compress
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

        estimatedSize += 2; // Each run takes 2 bytes (char + count)
        i += count;
    }

    // If RLE would make file bigger or same size, store uncompressed with marker
    if(estimatedSize >= input.size()) {
        QByteArray result;

        // Marker: 0xFF 0xFF 0xFF 0xFF = Uncompressed
        result.append((char)0xFF);
        result.append((char)0xFF);
        result.append((char)0xFF);
        result.append((char)0xFF);

        // Store original size
        int origSize = input.size();
        result.append((char)(origSize & 0xFF));
        result.append((char)((origSize >> 8) & 0xFF));
        result.append((char)((origSize >> 16) & 0xFF));
        result.append((char)((origSize >> 24) & 0xFF));

        // Store original data
        result.append(input);

        return result;
    }

    // RLE will compress - proceed normally
    QByteArray result;

    // Write original size (4 bytes)
    int origSize = input.size();
    result.append((char)(origSize & 0xFF));
    result.append((char)((origSize >> 8) & 0xFF));
    result.append((char)((origSize >> 16) & 0xFF));
    result.append((char)((origSize >> 24) & 0xFF));

    i = 0;
    while(i < input.size()) {
        unsigned char current = input[i];
        int count = 1;

        // Count consecutive same characters (max 255)
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

        // Read original size
        int origSize = ((unsigned char)input[4]) |
                       ((unsigned char)input[5] << 8) |
                       ((unsigned char)input[6] << 16) |
                       ((unsigned char)input[7] << 24);

        // Return original data (skip 8 byte header)
        return input.mid(8, origSize);
    }

    // Normal RLE decompression
    // Read original size
    int origSize = ((unsigned char)input[0]) |
                   ((unsigned char)input[1] << 8) |
                   ((unsigned char)input[2] << 16) |
                   ((unsigned char)input[3] << 24);

    if(origSize <= 0 || origSize > 100000000) { // Sanity check (100MB max)
        return QByteArray();
    }

    QByteArray result;
    result.reserve(origSize); // Pre-allocate for efficiency

    // Start reading from byte 4
    for(int i = 4; i < input.size(); i += 2) {
        if(i + 1 >= input.size()) break;

        unsigned char ch = input[i];
        unsigned char count = input[i + 1];

        // Validate count
        if(count == 0 || count > 255) break;

        for(int j = 0; j < count; j++) {
            result.append((char)ch);

            // Stop if we've reached original size
            if(result.size() >= origSize) {
                return result.left(origSize);
            }
        }
    }

    // Verify we got the right size
    if(result.size() != origSize) {
        return QByteArray(); // Decompression failed
    }

    return result;
}
