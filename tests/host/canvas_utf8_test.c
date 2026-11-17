#include "canvas_utf8.h"

#include <assert.h>
#include <stdint.h>
#include <stdio.h>

static void expect_decode(
    const char* text,
    size_t offset,
    uint32_t expected_codepoint,
    size_t expected_next) {
    uint32_t codepoint = 0;
    const size_t next = canvas_utf8_next(text, offset, &codepoint);
    assert(codepoint == expected_codepoint);
    assert(next == expected_next);
}

int main(void) {
    expect_decode("A", 0, 'A', 1);
    expect_decode("\xC2\xA2", 0, 0x00A2, 2);
    expect_decode("\xE4\xB8\xAD", 0, 0x4E2D, 3);
    expect_decode("\xF0\x9F\x98\x80", 0, '?', 4);

    expect_decode("\xC0\xAF", 0, '?', 1);
    expect_decode("\xE0\x80\x80", 0, '?', 1);
    expect_decode("\xED\xA0\x80", 0, '?', 1);
    expect_decode("\xF4\x90\x80\x80", 0, '?', 1);

    const char truncated2[] = {(char)0xC2, '\0'};
    const char truncated3[] = {(char)0xE4, (char)0xB8, '\0'};
    const char truncated4[] = {(char)0xF0, (char)0x9F, (char)0x98, '\0'};
    expect_decode(truncated2, 0, '?', 1);
    expect_decode(truncated3, 0, '?', 1);
    expect_decode(truncated3, 1, '?', 2);
    expect_decode(truncated4, 0, '?', 1);
    expect_decode(truncated4, 1, '?', 2);
    expect_decode(truncated4, 2, '?', 3);

    const char mixed[] = "A\xE4\xB8\xAD\xF0\x9F\x98\x80" "B";
    assert(canvas_utf8_prev(mixed, 1) == 0);
    assert(canvas_utf8_prev(mixed, 4) == 1);
    assert(canvas_utf8_prev(mixed, 8) == 4);
    assert(canvas_utf8_prev(mixed, 9) == 8);
    assert(canvas_utf8_prev(mixed, 0) == 0);

    const char malformed[] = {'A', (char)0x80, 'B', 0};
    assert(canvas_utf8_prev(malformed, 2) == 1);
    assert(canvas_utf8_prev(malformed, 3) == 2);
    const char malformed_sequence[] = {'A', (char)0xE4, (char)0x80, 'B', 0};
    assert(canvas_utf8_prev(malformed_sequence, 3) == 2);
    assert(canvas_utf8_prev(malformed_sequence, 4) == 3);

    assert(canvas_utf8_is_cjk(0x4E2D));
    assert(canvas_utf8_is_cjk(0x3001));
    assert(canvas_utf8_is_cjk(0xFF0C));
    assert(!canvas_utf8_is_cjk(0x00E9));
    assert(!canvas_utf8_contains_cjk("WiFi"));
    assert(!canvas_utf8_contains_cjk("\xC3\xA9"));
    assert(!canvas_utf8_contains_cjk("\xEE\x80\x80"));
    assert(canvas_utf8_contains_cjk("\xE4\xB8\xad"));
    assert(canvas_utf8_contains_cjk("\xE3\x80\x81"));

    /* Exercise every possible leading and second byte under ASan/UBSan. */
    for(unsigned first = 0; first <= 0xFF; ++first) {
        for(unsigned second = 0; second <= 0xFF; ++second) {
            const uint8_t sample[5] = {
                (uint8_t)first, (uint8_t)second, 0x80, 0x80, 0x00};
            uint32_t codepoint = 0;
            const size_t next = canvas_utf8_next((const char*)sample, 0, &codepoint);
            assert(next <= 4);
            if(first == 0) {
                assert(next == 0);
            } else {
                assert(next >= 1);
            }
        }
    }

    puts("canvas UTF-8 tests passed");
    return 0;
}
