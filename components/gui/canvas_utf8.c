#include "canvas_utf8.h"


size_t canvas_utf8_next(const char* text, size_t offset, uint32_t* codepoint) {
    if(!text || !codepoint) return offset;

    const uint8_t* bytes = (const uint8_t*)text + offset;
    const uint8_t first = bytes[0];

    if(first == 0) {
        *codepoint = 0;
        return offset;
    }

    if(first < 0x80) {
        *codepoint = first;
        return offset + 1;
    }

    if(first >= 0xC2 && first <= 0xDF) {
        const uint8_t second = bytes[1];
        if(second != 0 && (second & 0xC0) == 0x80) {
            *codepoint = ((uint32_t)(first & 0x1F) << 6) | (second & 0x3F);
            return offset + 2;
        }
    }

    if(first >= 0xE0 && first <= 0xEF) {
        const uint8_t second = bytes[1];
        if(second != 0) {
            const uint8_t third = bytes[2];
            if(third != 0 && (second & 0xC0) == 0x80 && (third & 0xC0) == 0x80 &&
               !(first == 0xE0 && second < 0xA0) &&
               !(first == 0xED && second >= 0xA0)) {
                *codepoint = ((uint32_t)(first & 0x0F) << 12) |
                             ((uint32_t)(second & 0x3F) << 6) | (third & 0x3F);
                return offset + 3;
            }
        }
    }

    if(first >= 0xF0 && first <= 0xF4) {
        const uint8_t second = bytes[1];
        if(second != 0) {
            const uint8_t third = bytes[2];
            if(third != 0) {
                const uint8_t fourth = bytes[3];
                if(fourth != 0 && (second & 0xC0) == 0x80 && (third & 0xC0) == 0x80 &&
                   (fourth & 0xC0) == 0x80 && !(first == 0xF0 && second < 0x90) &&
                   !(first == 0xF4 && second > 0x8F)) {
                    *codepoint = '?'; /* The U8g2 backend only supports BMP glyphs. */
                    return offset + 4;
                }
            }
        }
    }

    *codepoint = '?';
    return offset + 1;
}

size_t canvas_utf8_prev(const char* text, size_t offset) {
    if(!text || offset == 0) return 0;

    const uint8_t* bytes = (const uint8_t*)text;
    const size_t earliest_candidate = offset > 4 ? offset - 4 : 0;

    /* Validate a possible lead byte by decoding it to exactly this boundary.
     * Malformed bytes are treated as one-byte replacement characters. */
    for(size_t candidate_end = offset; candidate_end > earliest_candidate; --candidate_end) {
        const size_t candidate = candidate_end - 1;
        if((bytes[candidate] & 0xC0) == 0x80) continue;

        uint32_t codepoint = 0;
        if(canvas_utf8_next(text, candidate, &codepoint) == offset) {
            return candidate;
        }
    }

    /* The preceding byte is malformed or the caller supplied a non-boundary. */
    return offset - 1;
}

bool canvas_utf8_is_cjk(uint32_t codepoint) {
    return (codepoint >= 0x2E80 && codepoint <= 0x2EFF) ||
           (codepoint >= 0x2F00 && codepoint <= 0x2FDF) ||
           (codepoint >= 0x3000 && codepoint <= 0x303F) ||
           (codepoint >= 0x31C0 && codepoint <= 0x31EF) ||
           (codepoint >= 0x3400 && codepoint <= 0x4DBF) ||
           (codepoint >= 0x4E00 && codepoint <= 0x9FFF) ||
           (codepoint >= 0xF900 && codepoint <= 0xFAFF) ||
           (codepoint >= 0xFF01 && codepoint <= 0xFF60);
}

bool canvas_utf8_contains_cjk(const char* text) {
    if(!text) return false;

    for(size_t offset = 0; text[offset] != '\0';) {
        uint32_t codepoint = 0;
        const size_t next = canvas_utf8_next(text, offset, &codepoint);
        if(next <= offset) break;
        if(canvas_utf8_is_cjk(codepoint)) return true;
        offset = next;
    }
    return false;
}

