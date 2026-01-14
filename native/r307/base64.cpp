#include "base64.h"

static const char b64_table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

std::string base64_encode(const uint8_t* data, size_t len) {
    std::string out;
    out.reserve(((len + 2) / 3) * 4);

    for (size_t i = 0; i < len; i += 3) {
        uint32_t triple = 0;
        triple |= data[i] << 16;
        if (i + 1 < len) triple |= data[i + 1] << 8;
        if (i + 2 < len) triple |= data[i + 2];

        out.push_back(b64_table[(triple >> 18) & 0x3F]);
        out.push_back(b64_table[(triple >> 12) & 0x3F]);
        out.push_back(i + 1 < len ? b64_table[(triple >> 6) & 0x3F] : '=');
        out.push_back(i + 2 < len ? b64_table[triple & 0x3F] : '=');
    }
    return out;
}
