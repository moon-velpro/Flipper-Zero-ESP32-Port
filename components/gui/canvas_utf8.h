#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t canvas_utf8_next(const char* text, size_t offset, uint32_t* codepoint);
/* offset must be a codepoint boundary within text; malformed bytes consume one byte. */
size_t canvas_utf8_prev(const char* text, size_t offset);
bool canvas_utf8_is_cjk(uint32_t codepoint);
bool canvas_utf8_contains_cjk(const char* text);

#ifdef __cplusplus
}
#endif
