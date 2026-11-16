#include "elements.h"
#include <m-core.h>
#include <assets_icons.h>
#include <furi_hal.h>

#include "canvas.h"

#ifdef FLIPPER_ZH_CN
#include "canvas_utf8.h"
#endif
#include "icon_i.h"
#include "icon_animation_i.h"

#include <furi.h>
#include <math.h>
extern float ceilf(float);

#include <math.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int32_t x;
    int32_t y;
    int32_t leading_min;
    int32_t leading_default;
    size_t height;
    size_t descender;
    size_t len;
    const char* text;
} ElementTextBoxLine;

void elements_progress_bar(Canvas* canvas, int32_t x, int32_t y, size_t width, float progress) {
    furi_check(canvas);
    furi_check((progress >= 0.0f) && (progress <= 1.0f));
    size_t height = 9;

    float progress_width = roundf(progress * (width - 2));

    canvas_set_color(canvas, ColorWhite);
    canvas_draw_box(canvas, x + 1, y + 1, width - 2, height - 2);
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_rframe(canvas, x, y, width, height, 3);

    canvas_draw_box(canvas, x + 1, y + 1, progress_width, height - 2);
}

void elements_progress_bar_with_text(
    Canvas* canvas,
    int32_t x,
    int32_t y,
    size_t width,
    float progress,
    const char* text) {
    furi_check(canvas);
    furi_check((progress >= 0.0f) && (progress <= 1.0f));
    size_t height = 11;

    float progress_width = roundf(progress * (width - 2));

    canvas_set_color(canvas, ColorWhite);
    canvas_draw_box(canvas, x + 1, y + 1, width - 2, height - 2);
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_rframe(canvas, x, y, width, height, 3);

    canvas_draw_box(canvas, x + 1, y + 1, progress_width, height - 2);

    canvas_set_color(canvas, ColorXOR);
    canvas_set_font(canvas, FontSecondary);
    canvas_draw_str_aligned(canvas, x + width / 2, y + 2, AlignCenter, AlignTop, text);
}

void elements_scrollbar_pos(
    Canvas* canvas,
    int32_t x,
    int32_t y,
    size_t height,
    size_t pos,
    size_t total) {
    furi_check(canvas);

    // prevent overflows
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_box(canvas, x - 3, y, 3, height);

    // dot line
    canvas_set_color(canvas, ColorBlack);
    for(int32_t i = y; i < (int32_t)height + y; i += 2) {
        canvas_draw_dot(canvas, x - 2, i);
    }

    // Position block
    if(total) {
        float block_h = ((float)height) / total;
        canvas_draw_box(canvas, x - 3, y + (block_h * pos), 3, MAX(block_h, 1));
    }
}

void elements_scrollbar(Canvas* canvas, size_t pos, size_t total) {
    furi_check(canvas);

    size_t width = canvas_width(canvas);
    size_t height = canvas_height(canvas);

    // prevent overflows
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_box(canvas, width - 3, 0, 3, height);

    // dot line
    canvas_set_color(canvas, ColorBlack);
    for(size_t i = 0; i < height; i += 2) {
        canvas_draw_dot(canvas, width - 2, i);
    }

    // Position block
    if(total) {
        float block_h = ((float)height) / total;
        canvas_draw_box(canvas, width - 3, block_h * pos, 3, MAX(block_h, 1));
    }
}

void elements_scrollbar_horizontal(
    Canvas* canvas,
    int32_t x,
    int32_t y,
    size_t width,
    size_t pos,
    size_t total) {
    furi_check(canvas);

    // prevent overflows
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_box(canvas, x, y, width, 3);

    // dot line
    canvas_set_color(canvas, ColorBlack);
    for(size_t i = 0; i < width; i += 2) {
        canvas_draw_dot(canvas, x + (int32_t)i, y + 1);
    }

    // Position block
    if(total) {
        float block_w = ((float)width) / total;
        canvas_draw_box(canvas, x + (int32_t)(block_w * pos), y, MAX(block_w, 1), 3);
    }
}

void elements_frame(Canvas* canvas, int32_t x, int32_t y, size_t width, size_t height) {
    furi_check(canvas);

    canvas_draw_line(canvas, x + 2, y, x + width - 2, y);
    canvas_draw_line(canvas, x + 1, y + height - 1, x + width, y + height - 1);
    canvas_draw_line(canvas, x + 2, y + height, x + width - 1, y + height);

    canvas_draw_line(canvas, x, y + 2, x, y + height - 2);
    canvas_draw_line(canvas, x + width - 1, y + 1, x + width - 1, y + height - 2);
    canvas_draw_line(canvas, x + width, y + 2, x + width, y + height - 2);

    canvas_draw_dot(canvas, x + 1, y + 1);
}

void elements_button_left(Canvas* canvas, const char* str) {
    furi_check(canvas);

    const size_t button_height = 12;
    const size_t vertical_offset = 3;
    const size_t horizontal_offset = 3;
    const size_t string_width = canvas_string_width(canvas, str);
    const Icon* icon = &I_ButtonLeft_4x7;
    const int32_t icon_h_offset = 3;
    const int32_t icon_width_with_offset = icon->width + icon_h_offset;
    const int32_t icon_v_offset = icon->height + vertical_offset;
    const size_t button_width = string_width + horizontal_offset * 2 + icon_width_with_offset;

    const int32_t x = 0;
    const int32_t y = canvas_height(canvas);

    canvas_draw_box(canvas, x, y - button_height, button_width, button_height);
    canvas_draw_line(canvas, x + button_width + 0, y, x + button_width + 0, y - button_height + 0);
    canvas_draw_line(canvas, x + button_width + 1, y, x + button_width + 1, y - button_height + 1);
    canvas_draw_line(canvas, x + button_width + 2, y, x + button_width + 2, y - button_height + 2);

    canvas_invert_color(canvas);
    canvas_draw_icon(canvas, x + horizontal_offset, y - icon_v_offset, &I_ButtonLeft_4x7);
    canvas_draw_str(
        canvas, x + horizontal_offset + icon_width_with_offset, y - vertical_offset, str);
    canvas_invert_color(canvas);
}

void elements_button_right(Canvas* canvas, const char* str) {
    furi_check(canvas);

    const size_t button_height = 12;
    const size_t vertical_offset = 3;
    const size_t horizontal_offset = 3;
    const size_t string_width = canvas_string_width(canvas, str);
    const Icon* icon = &I_ButtonRight_4x7;
    const int32_t icon_h_offset = 3;
    const int32_t icon_width_with_offset = icon->width + icon_h_offset;
    const int32_t icon_v_offset = icon->height + vertical_offset;
    const size_t button_width = string_width + horizontal_offset * 2 + icon_width_with_offset;

    const int32_t x = canvas_width(canvas);
    const int32_t y = canvas_height(canvas);

    canvas_draw_box(canvas, x - button_width, y - button_height, button_width, button_height);
    canvas_draw_line(canvas, x - button_width - 1, y, x - button_width - 1, y - button_height + 0);
    canvas_draw_line(canvas, x - button_width - 2, y, x - button_width - 2, y - button_height + 1);
    canvas_draw_line(canvas, x - button_width - 3, y, x - button_width - 3, y - button_height + 2);

    canvas_invert_color(canvas);
    canvas_draw_str(canvas, x - button_width + horizontal_offset, y - vertical_offset, str);
    canvas_draw_icon(
        canvas, x - horizontal_offset - icon->width, y - icon_v_offset, &I_ButtonRight_4x7);
    canvas_invert_color(canvas);
}

void elements_button_up(Canvas* canvas, const char* str) {
    furi_check(canvas);

    const Icon* icon = &I_ButtonUp_7x4;

    const size_t button_height = 12;
    const size_t vertical_offset = 3;
    const size_t horizontal_offset = 3;
    const size_t string_width = canvas_string_width(canvas, str);
    const int32_t icon_h_offset = 3;
    const int32_t icon_width_with_offset = icon_get_width(icon) + icon_h_offset;
    const int32_t icon_v_offset = icon_get_height(icon) + (int32_t)vertical_offset;
    const size_t button_width = string_width + horizontal_offset * 2 + icon_width_with_offset;

    const int32_t x = 0;
    const int32_t y = 0 + button_height;

    int32_t line_x = x + button_width;
    int32_t line_y = y - button_height;

    canvas_draw_box(canvas, x, line_y, button_width, button_height);
    canvas_draw_line(canvas, line_x + 0, line_y, line_x + 0, y - 1);
    canvas_draw_line(canvas, line_x + 1, line_y, line_x + 1, y - 2);
    canvas_draw_line(canvas, line_x + 2, line_y, line_x + 2, y - 3);

    canvas_invert_color(canvas);
    canvas_draw_icon(canvas, x + horizontal_offset, y - icon_v_offset, icon);
    canvas_draw_str(
        canvas, x + horizontal_offset + icon_width_with_offset, y - vertical_offset, str);
    canvas_invert_color(canvas);
}

void elements_button_down(Canvas* canvas, const char* str) {
    furi_check(canvas);

    const Icon* icon = &I_ButtonDown_7x4;

    const size_t button_height = 12;
    const size_t vertical_offset = 3;
    const size_t horizontal_offset = 3;
    const size_t string_width = canvas_string_width(canvas, str);
    const int32_t icon_h_offset = 3;
    const int32_t icon_width_with_offset = icon_get_width(icon) + icon_h_offset;
    const int32_t icon_v_offset = icon_get_height(icon) + vertical_offset + 1;
    const size_t button_width = string_width + horizontal_offset * 2 + icon_width_with_offset;

    const int32_t x = canvas_width(canvas);
    const int32_t y = button_height;

    int32_t line_x = x - button_width;
    int32_t line_y = y - button_height;

    canvas_draw_box(canvas, line_x, line_y, button_width, button_height);
    canvas_draw_line(canvas, line_x - 1, line_y, line_x - 1, y - 1);
    canvas_draw_line(canvas, line_x - 2, line_y, line_x - 2, y - 2);
    canvas_draw_line(canvas, line_x - 3, line_y, line_x - 3, y - 3);

    canvas_invert_color(canvas);
    canvas_draw_str(canvas, x - button_width + horizontal_offset, y - vertical_offset, str);
    canvas_draw_icon(
        canvas, x - horizontal_offset - icon_get_width(icon), y - icon_v_offset, icon);
    canvas_invert_color(canvas);
}

void elements_button_center(Canvas* canvas, const char* str) {
    furi_check(canvas);

    const size_t button_height = 12;
    const size_t vertical_offset = 3;
    const size_t horizontal_offset = 1;
    const size_t string_width = canvas_string_width(canvas, str);
    const Icon* icon = &I_ButtonCenter_7x7;
    const int32_t icon_h_offset = 3;
    const int32_t icon_width_with_offset = icon->width + icon_h_offset;
    const int32_t icon_v_offset = icon->height + vertical_offset;
    const size_t button_width = string_width + horizontal_offset * 2 + icon_width_with_offset;

    const int32_t x = (canvas_width(canvas) - button_width) / 2;
    const int32_t y = canvas_height(canvas);

    canvas_draw_box(canvas, x, y - button_height, button_width, button_height);

    canvas_draw_line(canvas, x - 1, y, x - 1, y - button_height + 0);
    canvas_draw_line(canvas, x - 2, y, x - 2, y - button_height + 1);
    canvas_draw_line(canvas, x - 3, y, x - 3, y - button_height + 2);

    canvas_draw_line(canvas, x + button_width + 0, y, x + button_width + 0, y - button_height + 0);
    canvas_draw_line(canvas, x + button_width + 1, y, x + button_width + 1, y - button_height + 1);
    canvas_draw_line(canvas, x + button_width + 2, y, x + button_width + 2, y - button_height + 2);

    canvas_invert_color(canvas);
    canvas_draw_icon(canvas, x + horizontal_offset, y - icon_v_offset, &I_ButtonCenter_7x7);
    canvas_draw_str(
        canvas, x + horizontal_offset + icon_width_with_offset, y - vertical_offset, str);
    canvas_invert_color(canvas);
}

static size_t
    elements_get_max_chars_to_fit(Canvas* canvas, Align horizontal, const char* text, int32_t x) {
    const char* end = strchr(text, '\n');
    if(end == NULL) {
        end = text + strlen(text);
    }
    size_t text_size = end - text;
    FuriString* str;
    str = furi_string_alloc_set(text);
    furi_string_left(str, text_size);
    size_t result = 0;

    size_t len_px = canvas_string_width(canvas, furi_string_get_cstr(str));
    size_t px_left = 0;
    if(horizontal == AlignCenter) {
        if(x > (int32_t)(canvas_width(canvas) / 2)) {
            px_left = (canvas_width(canvas) - x) * 2;
        } else {
            px_left = x * 2;
        }
    } else if(horizontal == AlignLeft) {
        px_left = canvas_width(canvas) - x;
    } else if(horizontal == AlignRight) {
        px_left = x;
    } else {
        furi_crash();
    }

    if(len_px > px_left) {
        size_t excess_symbols_approximately =
            ceilf((float)(len_px - px_left) / ((float)len_px / (float)text_size));
        // reduce to 5 to be sure dash fit, and next line will be at least 5 symbols long
        if(excess_symbols_approximately > 0) {
            excess_symbols_approximately = MAX(excess_symbols_approximately, 5u);
            result = text_size - excess_symbols_approximately - 1;
        } else {
            result = text_size;
        }
    } else {
        result = text_size;
    }

    furi_string_free(str);
    return result;
}

#ifdef FLIPPER_ZH_CN
/* Return a byte count ending at a complete codepoint, never at a continuation
 * byte. An empty line (newline) legitimately consumes zero text bytes. */
static size_t elements_cjk_line_bytes(Canvas* canvas, const char* text, size_t available) {
    size_t offset = 0;
    size_t pixels = 0;
    while(text[offset] != '\0' && text[offset] != '\n') {
        uint32_t codepoint;
        size_t next = canvas_utf8_next(text, offset, &codepoint);
        if(next <= offset) break;
        size_t glyph = canvas_glyph_width(
            canvas, codepoint <= UINT16_MAX ? (uint16_t)codepoint : '?');

        if(pixels + glyph > available && offset != 0) break;
        /* Force one character when the available box is narrower than a glyph:
         * this is better than looping forever at byte offset zero. */
        if(pixels + glyph > available && offset == 0) return next;
        pixels += glyph;
        offset = next;
    }
    return offset;
}

static size_t elements_cjk_line_space(Canvas* canvas, Align horizontal, int32_t x) {
    int32_t available;
    if(horizontal == AlignCenter) {
        int32_t left = x;
        int32_t right = (int32_t)canvas_width(canvas) - x;
        available = 2 * (left < right ? left : right);
    } else if(horizontal == AlignRight) {
        available = x;
    } else {
        available = (int32_t)canvas_width(canvas) - x;
    }
    return available > 0 ? (size_t)available : 0;
}

static void elements_cjk_multiline_aligned(
    Canvas* canvas,
    int32_t x,
    int32_t y,
    Align horizontal,
    Align vertical,
    const char* text) {
    size_t available = elements_cjk_line_space(canvas, horizontal, x);
    size_t line_height = canvas_current_font_height(canvas);
    if(line_height < 12) line_height = 12;
    size_t lines = 0;

    /* Count text lines with exactly the same codepoint stepping as drawing. */
    for(const char* p = text; *p;) {
        size_t bytes = elements_cjk_line_bytes(canvas, p, available);
        p += bytes;
        if(*p == '\n') ++p;
        ++lines;
    }
    if(lines == 0) return;

    if(vertical == AlignBottom) {
        y -= (int32_t)(line_height * (lines - 1));
    } else if(vertical == AlignCenter) {
        y -= (int32_t)((line_height * (lines - 1)) / 2);
    }

    FuriString* line = furi_string_alloc();
    for(const char* p = text; *p;) {
        size_t bytes = elements_cjk_line_bytes(canvas, p, available);
        furi_string_set_strn(line, p, bytes);
        p += bytes;
        if(*p == '\n') ++p;

        canvas_draw_str_aligned(
            canvas, x, y, horizontal, vertical, furi_string_get_cstr(line));
        if(y > (int32_t)canvas_height(canvas)) break;
        y += (int32_t)line_height;
    }
    furi_string_free(line);
}
#endif /* FLIPPER_ZH_CN */

void elements_multiline_text_aligned(
    Canvas* canvas,
    int32_t x,
    int32_t y,
    Align horizontal,
    Align vertical,
    const char* text) {
    furi_check(canvas);
    furi_check(text);
#ifdef FLIPPER_ZH_CN
    if(canvas_utf8_contains_cjk(text)) {
        elements_cjk_multiline_aligned(canvas, x, y, horizontal, vertical, text);
        return;
    }
#endif

    size_t lines_count = 0;
    size_t font_height = canvas_current_font_height(canvas);
    FuriString* line;

    /* go through text line by line and count lines */
    for(const char* start = text; start[0];) {
        size_t chars_fit = elements_get_max_chars_to_fit(canvas, horizontal, start, x);
        ++lines_count;
        start += chars_fit;
        start += start[0] == '\n' ? 1 : 0;
    }

    if(vertical == AlignBottom) {
        y -= font_height * (lines_count - 1);
    } else if(vertical == AlignCenter) {
        y -= (font_height * (lines_count - 1)) / 2;
    }

    /* go through text line by line and print them */
    for(const char* start = text; start[0];) {
        size_t chars_fit = elements_get_max_chars_to_fit(canvas, horizontal, start, x);

        if((start[chars_fit] == '\n') || (start[chars_fit] == 0)) {
            line = furi_string_alloc_printf("%.*s", chars_fit, start);
        } else if((y + font_height) > canvas_height(canvas)) {
            line = furi_string_alloc_printf("%.*s...\n", chars_fit, start);
        } else {
            chars_fit -= 1; // account for the dash
            line = furi_string_alloc_printf("%.*s-\n", chars_fit, start);
        }
        canvas_draw_str_aligned(canvas, x, y, horizontal, vertical, furi_string_get_cstr(line));
        furi_string_free(line);
        y += font_height;
        if(y > (int32_t)canvas_height(canvas)) {
            break;
        }

        start += chars_fit;
        start += start[0] == '\n' ? 1 : 0;
    }
}

void elements_multiline_text(Canvas* canvas, int32_t x, int32_t y, const char* text) {
    furi_check(canvas);
    furi_check(text);

    size_t font_height = canvas_current_font_height(canvas);
#ifdef FLIPPER_ZH_CN
    if(canvas_utf8_contains_cjk(text) && font_height < 12) {
        font_height = 12;
    }
#endif
    FuriString* str;
    str = furi_string_alloc();
    const char* start = text;
    char* end;
    do {
        end = strchr(start, '\n');
        if(end) {
            furi_string_set_strn(str, start, end - start);
            start = end + 1;
        } else {
            furi_string_set(str, start);
        }
        canvas_draw_str(canvas, x, y, furi_string_get_cstr(str));
        y += font_height;
    } while(end && y < 64);
    furi_string_free(str);
}

void elements_multiline_text_framed(Canvas* canvas, int32_t x, int32_t y, const char* text) {
    furi_check(canvas);
    furi_check(text);

    size_t font_height = canvas_current_font_height(canvas);
    size_t str_width = 0;
    size_t lines = 1;
#ifdef FLIPPER_ZH_CN
    if(canvas_utf8_contains_cjk(text)) {
        if(font_height < 12) font_height = 12;

        FuriString* line = furi_string_alloc();
        const char* start = text;
        for(const char* p = text;; ++p) {
            if(*p == '\n' || *p == '\0') {
                furi_string_set_strn(line, start, (size_t)(p - start));
                const size_t width = canvas_string_width(canvas, furi_string_get_cstr(line));
                if(width > str_width) str_width = width;
                if(*p == '\0') break;
                ++lines;
                start = p + 1;
            }
        }
        furi_string_free(line);
    } else
#endif
    {
        /* Keep the original frame sizing for non-CJK strings and other boards. */
        str_width = canvas_string_width(canvas, text);
        const char* t = text;
        while(*t != '\0') {
            if(*t == '\n') {
                lines++;
                const size_t temp_width = canvas_string_width(canvas, t + 1);
                str_width = temp_width > str_width ? temp_width : str_width;
            }
            t++;
        }
    }

    canvas_set_color(canvas, ColorWhite);
    canvas_draw_box(canvas, x, y - font_height, str_width + 8, font_height * lines + 4);
    canvas_set_color(canvas, ColorBlack);
    elements_multiline_text(canvas, x + 4, y - 1, text);
    elements_frame(canvas, x, y - font_height, str_width + 8, font_height * lines + 4);
}
void elements_slightly_rounded_frame(
    Canvas* canvas,
    int32_t x,
    int32_t y,
    size_t width,
    size_t height) {
    furi_check(canvas);
    canvas_draw_rframe(canvas, x, y, width, height, 1);
}

void elements_slightly_rounded_box(
    Canvas* canvas,
    int32_t x,
    int32_t y,
    size_t width,
    size_t height) {
    furi_check(canvas);
    canvas_draw_rbox(canvas, x, y, width, height, 1);
}

void elements_bold_rounded_frame(Canvas* canvas, int32_t x, int32_t y, size_t width, size_t height) {
    furi_check(canvas);

    canvas_set_color(canvas, ColorWhite);
    canvas_draw_box(canvas, x + 2, y + 2, width - 3, height - 3);
    canvas_set_color(canvas, ColorBlack);

    canvas_draw_line(canvas, x + 3, y, x + width - 3, y);
    canvas_draw_line(canvas, x + 2, y + 1, x + width - 2, y + 1);

    canvas_draw_line(canvas, x, y + 3, x, y + height - 3);
    canvas_draw_line(canvas, x + 1, y + 2, x + 1, y + height - 2);

    canvas_draw_line(canvas, x + width, y + 3, x + width, y + height - 3);
    canvas_draw_line(canvas, x + width - 1, y + 2, x + width - 1, y + height - 2);

    canvas_draw_line(canvas, x + 3, y + height, x + width - 3, y + height);
    canvas_draw_line(canvas, x + 2, y + height - 1, x + width - 2, y + height - 1);

    canvas_draw_dot(canvas, x + 2, y + 2);
    canvas_draw_dot(canvas, x + 3, y + 2);
    canvas_draw_dot(canvas, x + 2, y + 3);

    canvas_draw_dot(canvas, x + width - 2, y + 2);
    canvas_draw_dot(canvas, x + width - 3, y + 2);
    canvas_draw_dot(canvas, x + width - 2, y + 3);

    canvas_draw_dot(canvas, x + 2, y + height - 2);
    canvas_draw_dot(canvas, x + 3, y + height - 2);
    canvas_draw_dot(canvas, x + 2, y + height - 3);

    canvas_draw_dot(canvas, x + width - 2, y + height - 2);
    canvas_draw_dot(canvas, x + width - 3, y + height - 2);
    canvas_draw_dot(canvas, x + width - 2, y + height - 3);
}

void elements_bubble(Canvas* canvas, int32_t x, int32_t y, size_t width, size_t height) {
    furi_check(canvas);
    canvas_draw_rframe(canvas, x + 4, y, width, height, 3);
    int32_t y_corner = y + height * 2 / 3;
    canvas_draw_line(canvas, x, y_corner, x + 4, y_corner - 4);
    canvas_draw_line(canvas, x, y_corner, x + 4, y_corner + 4);
    canvas_set_color(canvas, ColorWhite);
    canvas_draw_line(canvas, x + 4, y_corner - 3, x + 4, y_corner + 3);
    canvas_set_color(canvas, ColorBlack);
}

void elements_bubble_str(
    Canvas* canvas,
    int32_t x,
    int32_t y,
    const char* text,
    Align horizontal,
    Align vertical) {
    furi_check(canvas);
    furi_check(text);

    size_t font_height = canvas_current_font_height(canvas);
    size_t str_width = canvas_string_width(canvas, text);

    // count \n's
    size_t lines = 1;
    const char* t = text;
    while(*t != '\0') {
        if(*t == '\n') {
            lines++;
            size_t temp_width = canvas_string_width(canvas, t + 1);
            str_width = temp_width > str_width ? temp_width : str_width;
        }
        t++;
    }

    int32_t frame_x = x;
    int32_t frame_y = y;
    size_t frame_width = str_width + 8;
    size_t frame_height = font_height * lines + 4;

    canvas_set_color(canvas, ColorWhite);
    canvas_draw_box(canvas, frame_x + 1, frame_y + 1, frame_width - 2, frame_height - 2);
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_rframe(canvas, frame_x, frame_y, frame_width, frame_height, 1);
    elements_multiline_text(canvas, x + 4, y - 1 + font_height, text);

    int32_t x1 = 0;
    int32_t x2 = 0;
    int32_t x3 = 0;
    int32_t y1 = 0;
    int32_t y2 = 0;
    int32_t y3 = 0;
    if((horizontal == AlignLeft) && (vertical == AlignTop)) {
        x1 = frame_x;
        y1 = frame_y;
        x2 = frame_x - 4;
        y2 = frame_y;
        x3 = frame_x;
        y3 = frame_y + 4;
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_box(canvas, x2 + 2, y2 + 1, 2, 2);
        canvas_set_color(canvas, ColorBlack);
    } else if((horizontal == AlignLeft) && (vertical == AlignCenter)) {
        x1 = frame_x;
        y1 = frame_y + (frame_height - 1) / 2 - 4;
        x2 = frame_x - 4;
        y2 = frame_y + (frame_height - 1) / 2;
        x3 = frame_x;
        y3 = frame_y + (frame_height - 1) / 2 + 4;
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_box(canvas, x2 + 2, y2 - 2, 2, 5);
        canvas_draw_dot(canvas, x2 + 1, y2);
        canvas_set_color(canvas, ColorBlack);
    } else if((horizontal == AlignLeft) && (vertical == AlignBottom)) {
        x1 = frame_x;
        y1 = frame_y + (frame_height - 1) - 4;
        x2 = frame_x - 4;
        y2 = frame_y + (frame_height - 1);
        x3 = frame_x;
        y3 = frame_y + (frame_height - 1);
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_box(canvas, x2 + 2, y2 - 2, 2, 2);
        canvas_set_color(canvas, ColorBlack);
    } else if((horizontal == AlignRight) && (vertical == AlignTop)) {
        x1 = frame_x + (frame_width - 1);
        y1 = frame_y;
        x2 = frame_x + (frame_width - 1) + 4;
        y2 = frame_y;
        x3 = frame_x + (frame_width - 1);
        y3 = frame_y + 4;
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_box(canvas, x2 - 3, y2 + 1, 2, 2);
        canvas_set_color(canvas, ColorBlack);
    } else if((horizontal == AlignRight) && (vertical == AlignCenter)) {
        x1 = frame_x + (frame_width - 1);
        y1 = frame_y + (frame_height - 1) / 2 - 4;
        x2 = frame_x + (frame_width - 1) + 4;
        y2 = frame_y + (frame_height - 1) / 2;
        x3 = frame_x + (frame_width - 1);
        y3 = frame_y + (frame_height - 1) / 2 + 4;
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_box(canvas, x2 - 3, y2 - 2, 2, 5);
        canvas_draw_dot(canvas, x2 - 1, y2);
        canvas_set_color(canvas, ColorBlack);
    } else if((horizontal == AlignRight) && (vertical == AlignBottom)) {
        x1 = frame_x + (frame_width - 1);
        y1 = frame_y + (frame_height - 1) - 4;
        x2 = frame_x + (frame_width - 1) + 4;
        y2 = frame_y + (frame_height - 1);
        x3 = frame_x + (frame_width - 1);
        y3 = frame_y + (frame_height - 1);
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_box(canvas, x2 - 3, y2 - 2, 2, 2);
        canvas_set_color(canvas, ColorBlack);
    } else if((horizontal == AlignCenter) && (vertical == AlignTop)) {
        x1 = frame_x + (frame_width - 1) / 2 - 4;
        y1 = frame_y;
        x2 = frame_x + (frame_width - 1) / 2;
        y2 = frame_y - 4;
        x3 = frame_x + (frame_width - 1) / 2 + 4;
        y3 = frame_y;
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_box(canvas, x2 - 2, y2 + 2, 5, 2);
        canvas_draw_dot(canvas, x2, y2 + 1);
        canvas_set_color(canvas, ColorBlack);
    } else if((horizontal == AlignCenter) && (vertical == AlignBottom)) {
        x1 = frame_x + (frame_width - 1) / 2 - 4;
        y1 = frame_y + (frame_height - 1);
        x2 = frame_x + (frame_width - 1) / 2;
        y2 = frame_y + (frame_height - 1) + 4;
        x3 = frame_x + (frame_width - 1) / 2 + 4;
        y3 = frame_y + (frame_height - 1);
        canvas_set_color(canvas, ColorWhite);
        canvas_draw_box(canvas, x2 - 2, y2 - 3, 5, 2);
        canvas_draw_dot(canvas, x2, y2 - 1);
        canvas_set_color(canvas, ColorBlack);
    }

    canvas_set_color(canvas, ColorWhite);
    canvas_draw_line(canvas, x3, y3, x1, y1);
    canvas_set_color(canvas, ColorBlack);
    canvas_draw_line(canvas, x1, y1, x2, y2);
    canvas_draw_line(canvas, x2, y2, x3, y3);
}

#ifdef FLIPPER_ZH_CN
/* Codepoint-aware width fitting prevents truncating one of a Han character's bytes. */
static void elements_utf8_fit_to_width(Canvas* canvas, FuriString* line, size_t width) {
    while(canvas_string_width(canvas, furi_string_get_cstr(line)) > width) {
        const char* bytes = furi_string_get_cstr(line);
        size_t byte_length = furi_string_size(line);
        if(byte_length == 0) break;
        size_t last_start = canvas_utf8_prev(bytes, byte_length);
        furi_string_left(line, last_start);
    }
}

static size_t elements_utf8_codepoints(const char* bytes, size_t byte_length) {
    size_t count = 0;
    size_t pos = 0;
    while(pos < byte_length) {
        uint32_t codepoint;
        size_t next = canvas_utf8_next(bytes, pos, &codepoint);
        if(next <= pos) break;
        pos = next;
        count++;
    }
    return count;
}

static size_t elements_utf8_byte_offset(const char* bytes, size_t byte_length, size_t count) {
    size_t pos = 0;
    while(count > 0 && pos < byte_length) {
        uint32_t codepoint;
        size_t next = canvas_utf8_next(bytes, pos, &codepoint);
        if(next <= pos) break;
        pos = next;
        count--;
    }
    return pos;
}

static void elements_utf8_scroll_and_fit(
    Canvas* canvas,
    FuriString* line,
    size_t available,
    size_t scroll,
    bool ellipsis) {
    const size_t ellipsis_width = canvas_string_width(canvas, "...");
    bool add_dots = ellipsis && available >= ellipsis_width;
    size_t text_width = add_dots ? available - ellipsis_width : available;

    const char* bytes = furi_string_get_cstr(line);
    size_t byte_length = furi_string_size(line);
    size_t suffix_start = byte_length;
    size_t suffix_width = 0;

    /* The number of characters that do NOT fit at the right end determines
     * the scroll cycle. Count code points, not UTF-8 bytes. */
    while(suffix_start > 0) {
        size_t previous = canvas_utf8_prev(bytes, suffix_start);
        uint32_t codepoint;
        canvas_utf8_next(bytes, previous, &codepoint);
        size_t glyph_width = canvas_glyph_width(
            canvas, codepoint <= UINT16_MAX ? (uint16_t)codepoint : '?');
        if(suffix_width + glyph_width > text_width) break;
        suffix_width += glyph_width;
        suffix_start = previous;
    }

    size_t hidden_count = elements_utf8_codepoints(bytes, suffix_start);
    size_t cycle = hidden_count + 3;
    if(cycle > 0) scroll %= cycle;

    size_t bytes_to_skip = elements_utf8_byte_offset(bytes, byte_length, scroll);
    if(bytes_to_skip) furi_string_right(line, bytes_to_skip);

    elements_utf8_fit_to_width(canvas, line, text_width);
    if(add_dots) furi_string_cat(line, "...");
}

void elements_string_fit_width(Canvas* canvas, FuriString* string, size_t width) {
    furi_check(canvas);
    furi_check(string);

    if(!canvas_utf8_contains_cjk(furi_string_get_cstr(string))) {
        /* Keep the original implementation unchanged for ASCII and other scripts. */
        size_t len_px = canvas_string_width(canvas, furi_string_get_cstr(string));
        if(len_px > width) {
            width -= canvas_string_width(canvas, "...");
            do {
                furi_string_left(string, furi_string_size(string) - 1);
                len_px = canvas_string_width(canvas, furi_string_get_cstr(string));
            } while(len_px > width);
            furi_string_cat(string, "...");
        }
        return;
    }

    if(canvas_string_width(canvas, furi_string_get_cstr(string)) > width) {
        size_t dots_width = canvas_string_width(canvas, "...");
        bool add_dots = width >= dots_width;
        elements_utf8_fit_to_width(canvas, string, add_dots ? width - dots_width : width);
        if(add_dots) furi_string_cat(string, "...");
    }
}
void elements_scrollable_text_line_str(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    const char* string,
    size_t scroll,
    bool ellipsis,
    bool centered) {
    furi_check(canvas);
    if(!string) return;

    if(!canvas_utf8_contains_cjk(string)) {
        /* Preserve the source project's original non-CJK scrolling path. */
        FuriString* line = furi_string_alloc_set_str(string);
        size_t len_px = canvas_string_width(canvas, furi_string_get_cstr(line));
        if(len_px > width) {
            if(centered) {
                centered = false;
                x -= width / 2;
            }

            if(ellipsis) {
                width -= canvas_string_width(canvas, "...");
            }

            size_t scroll_size = furi_string_size(line);
            size_t right_width = 0;
            for(size_t i = scroll_size - 1; i > 0; i--) {
                right_width += canvas_glyph_width(canvas, furi_string_get_char(line, i));
                if(right_width > width) break;
                scroll_size--;
                if(!scroll_size) break;
            }
            if(scroll_size) {
                scroll_size += 3;
                scroll = scroll % scroll_size;
                furi_string_right(line, scroll);
            }

            len_px = canvas_string_width(canvas, furi_string_get_cstr(line));
            while(len_px > width) {
                furi_string_left(line, furi_string_size(line) - 1);
                len_px = canvas_string_width(canvas, furi_string_get_cstr(line));
            }

            if(ellipsis) furi_string_cat(line, "...");
        }

        if(centered) {
            canvas_draw_str_aligned(
                canvas, x, y, AlignCenter, AlignBottom, furi_string_get_cstr(line));
        } else {
            canvas_draw_str(canvas, x, y, furi_string_get_cstr(line));
        }
        furi_string_free(line);
        return;
    }

    FuriString* line = furi_string_alloc_set_str(string);
    int32_t draw_x = x;

    if(canvas_string_width(canvas, furi_string_get_cstr(line)) > width) {
        if(centered) {
            centered = false;
            draw_x -= width / 2;
        }
        elements_utf8_scroll_and_fit(canvas, line, width, scroll, ellipsis);
    }

    if(centered) {
        canvas_draw_str_aligned(
            canvas, draw_x, y, AlignCenter, AlignBottom, furi_string_get_cstr(line));
    } else {
        canvas_draw_str(canvas, draw_x, y, furi_string_get_cstr(line));
    }
    furi_string_free(line);
}
void elements_scrollable_text_line(
    Canvas* canvas,
    int32_t x,
    int32_t y,
    size_t width,
    FuriString* string,
    size_t scroll,
    bool ellipsis) {
    furi_check(canvas);
    furi_check(string);

    if(!canvas_utf8_contains_cjk(furi_string_get_cstr(string))) {
        /* Keep the original non-CJK path, including its scroll cadence. */
        FuriString* line = furi_string_alloc_set(string);
        size_t len_px = canvas_string_width(canvas, furi_string_get_cstr(line));
        if(len_px > width) {
            if(ellipsis) {
                width -= canvas_string_width(canvas, "...");
            }

            size_t scroll_size = furi_string_size(line);
            size_t right_width = 0;
            for(size_t i = scroll_size; i > 0; i--) {
                right_width += canvas_glyph_width(canvas, furi_string_get_char(line, i - 1));
                if(right_width > width) break;
                scroll_size--;
                if(!scroll_size) break;
            }

            if(scroll_size) {
                scroll_size += 3;
                scroll = scroll % scroll_size;
                furi_string_right(line, scroll);
            }

            len_px = canvas_string_width(canvas, furi_string_get_cstr(line));
            while(len_px > width) {
                furi_string_left(line, furi_string_size(line) - 1);
                len_px = canvas_string_width(canvas, furi_string_get_cstr(line));
            }

            if(ellipsis) furi_string_cat(line, "...");
        }

        canvas_draw_str(canvas, x, y, furi_string_get_cstr(line));
        furi_string_free(line);
        return;
    }

    FuriString* line = furi_string_alloc_set(string);
    if(canvas_string_width(canvas, furi_string_get_cstr(line)) > width) {
        elements_utf8_scroll_and_fit(canvas, line, width, scroll, ellipsis);
    }

    canvas_draw_str(canvas, x, y, furi_string_get_cstr(line));
    furi_string_free(line);
}
#else
void elements_string_fit_width(Canvas* canvas, FuriString* string, size_t width) {
    furi_check(canvas);
    furi_check(string);

    size_t len_px = canvas_string_width(canvas, furi_string_get_cstr(string));
    if(len_px > width) {
        width -= canvas_string_width(canvas, "...");
        do {
            furi_string_left(string, furi_string_size(string) - 1);
            len_px = canvas_string_width(canvas, furi_string_get_cstr(string));
        } while(len_px > width);
        furi_string_cat(string, "...");
    }
}

void elements_scrollable_text_line_str(
    Canvas* canvas,
    uint8_t x,
    uint8_t y,
    uint8_t width,
    const char* string,
    size_t scroll,
    bool ellipsis,
    bool centered) {
    FuriString* line = furi_string_alloc_set_str(string);

    size_t len_px = canvas_string_width(canvas, furi_string_get_cstr(line));
    if(len_px > width) {
        if(centered) {
            centered = false;
            x -= width / 2;
        }

        if(ellipsis) {
            width -= canvas_string_width(canvas, "...");
        }

        // Calculate scroll size
        size_t scroll_size = furi_string_size(line);
        size_t right_width = 0;
        for(size_t i = scroll_size - 1; i > 0; i--) {
            right_width += canvas_glyph_width(canvas, furi_string_get_char(line, i));
            if(right_width > width) break;
            scroll_size--;
            if(!scroll_size) break;
        }
        // Ensure that we have something to scroll
        if(scroll_size) {
            scroll_size += 3;
            scroll = scroll % scroll_size;
            furi_string_right(line, scroll);
        }

        len_px = canvas_string_width(canvas, furi_string_get_cstr(line));
        while(len_px > width) {
            furi_string_left(line, furi_string_size(line) - 1);
            len_px = canvas_string_width(canvas, furi_string_get_cstr(line));
        }

        if(ellipsis) {
            furi_string_cat(line, "...");
        }
    }

    if(centered) {
        canvas_draw_str_aligned(
            canvas, x, y, AlignCenter, AlignBottom, furi_string_get_cstr(line));
    } else {
        canvas_draw_str(canvas, x, y, furi_string_get_cstr(line));
    }
    furi_string_free(line);
}

void elements_scrollable_text_line(
    Canvas* canvas,
    int32_t x,
    int32_t y,
    size_t width,
    FuriString* string,
    size_t scroll,
    bool ellipsis) {
    furi_check(canvas);
    furi_check(string);

    FuriString* line = furi_string_alloc_set(string);

    size_t len_px = canvas_string_width(canvas, furi_string_get_cstr(line));
    if(len_px > width) {
        if(ellipsis) {
            width -= canvas_string_width(canvas, "...");
        }

        // Calculate scroll size
        size_t scroll_size = furi_string_size(line);
        size_t right_width = 0;
        for(size_t i = scroll_size; i > 0; i--) {
            right_width += canvas_glyph_width(canvas, furi_string_get_char(line, i - 1));
            if(right_width > width) break;
            scroll_size--;
            if(!scroll_size) break;
        }
        // Ensure that we have something to scroll
        if(scroll_size) {
            scroll_size += 3;
            scroll = scroll % scroll_size;
            furi_string_right(line, scroll);
        }

        len_px = canvas_string_width(canvas, furi_string_get_cstr(line));
        while(len_px > width) {
            furi_string_left(line, furi_string_size(line) - 1);
            len_px = canvas_string_width(canvas, furi_string_get_cstr(line));
        }

        if(ellipsis) {
            furi_string_cat(line, "...");
        }
    }

    canvas_draw_str(canvas, x, y, furi_string_get_cstr(line));
    furi_string_free(line);
}

#endif /* FLIPPER_ZH_CN */

#ifdef FLIPPER_ZH_CN
static void elements_text_box_utf8(
    Canvas* canvas,
    int32_t x,
    int32_t y,
    size_t width,
    size_t height,
    Align horizontal,
    Align vertical,
    const char* text,
    bool strip_to_dots) {
    ElementTextBoxLine lines[ELEMENTS_MAX_LINES_NUM] = {0};
    bool bold = false;
    bool mono = false;
    bool inverse = false;
    bool inverse_present = false;
    Font current_font = FontSecondary;
    Font previous_font = FontSecondary;
    const CanvasFontParameters* font_params = canvas_get_font_params(canvas, current_font);

    size_t line_leading_min = font_params->leading_min;
    size_t line_leading_default = font_params->leading_default;
    size_t line_height = font_params->height;
    size_t line_descender = font_params->descender;
    size_t line_num = 0;
    size_t line_width = 0;
    size_t line_len = 0;
    size_t total_height_min = 0;
    size_t total_height_default = 0;
    size_t offset = 0;
    bool full_text_processed = false;

    canvas_set_font(canvas, FontSecondary);
    const size_t dots_width = canvas_string_width(canvas, "...");
    lines[0].text = text;

    while(!full_text_processed && line_num < ELEMENTS_MAX_LINES_NUM) {
        if(previous_font != current_font) {
            font_params = canvas_get_font_params(canvas, current_font);
            line_leading_min = MAX(line_leading_min, font_params->leading_min);
            line_leading_default = MAX(line_leading_default, font_params->leading_default);
            line_height = MAX(line_height, font_params->height);
            line_descender = MAX(line_descender, font_params->descender);
            previous_font = current_font;
        }

        if(text[offset] == '\e' && text[offset + 1] != '\0') {
            const char marker = text[offset + 1];
            offset += 2;
            line_len += 2;
            if(marker == ELEMENTS_BOLD_MARKER) {
                bold = !bold;
            } else if(marker == ELEMENTS_MONO_MARKER) {
                mono = !mono;
            } else if(marker == ELEMENTS_INVERSE_MARKER) {
                inverse_present = true;
            }

            current_font = mono ? FontKeyboard : (bold ? FontPrimary : FontSecondary);
            canvas_set_font(canvas, current_font);
            continue;
        }

        if(text[offset] == '\0' || text[offset] == '\n') {
            if(text[offset] == '\0') {
                full_text_processed = true;
            } else {
                ++offset;
            }
        } else {
            uint32_t codepoint = 0;
            size_t next_offset = canvas_utf8_next(text, offset, &codepoint);
            if(next_offset <= offset) next_offset = offset + 1;
            const uint16_t glyph =
                codepoint <= UINT16_MAX ? (uint16_t)codepoint : (uint16_t)'?';
            const size_t glyph_width = canvas_glyph_width(canvas, glyph);

            if(line_width > 0 && line_width + glyph_width > width) {
                /* Wrap before this glyph; offset remains at a codepoint boundary. */
            } else {
                line_width += glyph_width;
                line_len += next_offset - offset;
                if(canvas_utf8_is_cjk(codepoint)) {
                    if(line_leading_min < 12) line_leading_min = 12;
                    if(line_leading_default < 12) line_leading_default = 12;
                    if(line_height < 12) line_height = 12;
                }
                offset = next_offset;
                continue;
            }
        }

        if(inverse_present) {
            line_leading_min += 1;
            line_leading_default += 1;
            inverse_present = false;
        }

        if(total_height_min + line_leading_min > height) break;

        lines[line_num].leading_min = line_leading_min;
        lines[line_num].leading_default = line_leading_default;
        lines[line_num].height = line_height;
        lines[line_num].descender = line_descender;
        lines[line_num].len = line_len;

        if(horizontal == AlignCenter) {
            const size_t remaining = line_width < width ? width - line_width : 0;
            lines[line_num].x = x + (int32_t)(remaining / 2);
        } else if(horizontal == AlignRight) {
            const size_t remaining = line_width < width ? width - line_width : 0;
            lines[line_num].x = x + (int32_t)remaining;
        } else {
            lines[line_num].x = x;
        }

        total_height_min += line_leading_min;
        total_height_default += line_leading_default;
        lines[line_num].y = total_height_min;
        ++line_num;
        if(line_num >= ELEMENTS_MAX_LINES_NUM || full_text_processed) break;

        lines[line_num].text = &text[offset];
        line_leading_min = font_params->leading_min;
        line_leading_default = font_params->leading_default;
        line_height = font_params->height;
        line_descender = font_params->descender;
        line_width = 0;
        line_len = 0;
    }

    if(line_num == 0) {
        canvas_set_font(canvas, FontSecondary);
        return;
    }

    if(total_height_default < height) {
        if(vertical == AlignTop) {
            lines[0].y = y + lines[0].height;
        } else if(vertical == AlignCenter) {
            lines[0].y = y + lines[0].height + (height - total_height_default) / 2;
        } else if(vertical == AlignBottom) {
            lines[0].y = y + lines[0].height + (height - total_height_default);
        }
        for(size_t i = 1; i < line_num; ++i) {
            lines[i].y = lines[i - 1].y + lines[i - 1].leading_default;
        }
    } else {
        lines[0].y = y + lines[0].height;
        for(size_t i = 1; i < line_num; ++i) {
            lines[i].y = lines[i - 1].y + lines[i - 1].leading_min;
        }

        const size_t free_pixels = height - total_height_min;
        for(size_t i = 0; i < free_pixels && line_num > 1; ++i) {
            const size_t start = 1 + (i % (line_num - 1));
            for(size_t j = start; j < line_num; ++j) {
                lines[j].y++;
            }
        }
    }

    canvas_set_font(canvas, FontSecondary);
    bold = false;
    mono = false;
    inverse = false;

    for(size_t i = 0; i < line_num; ++i) {
        size_t byte_offset = 0;
        while(byte_offset < lines[i].len) {
            if(lines[i].text[byte_offset] == '\e' && byte_offset + 1 < lines[i].len) {
                const char marker = lines[i].text[byte_offset + 1];
                byte_offset += 2;
                if(marker == ELEMENTS_BOLD_MARKER) {
                    bold = !bold;
                } else if(marker == ELEMENTS_MONO_MARKER) {
                    mono = !mono;
                } else if(marker == ELEMENTS_INVERSE_MARKER) {
                    inverse = !inverse;
                }
                current_font = mono ? FontKeyboard : (bold ? FontPrimary : FontSecondary);
                canvas_set_font(canvas, current_font);
                continue;
            }

            uint32_t codepoint = 0;
            size_t next_offset =
                canvas_utf8_next(lines[i].text, byte_offset, &codepoint);
            if(next_offset <= byte_offset || next_offset > lines[i].len) {
                codepoint = '?';
                next_offset = byte_offset + 1;
            }
            if(codepoint == '\n') {
                byte_offset = next_offset;
                continue;
            }

            const uint16_t glyph =
                codepoint <= UINT16_MAX ? (uint16_t)codepoint : (uint16_t)'?';
            const size_t glyph_width = canvas_glyph_width(canvas, glyph);

            if(inverse) {
                canvas_draw_box(
                    canvas,
                    lines[i].x - 1,
                    lines[i].y - lines[i].height - 1,
                    glyph_width + 1,
                    lines[i].height + lines[i].descender + 2);
                canvas_invert_color(canvas);
                canvas_draw_glyph(canvas, lines[i].x, lines[i].y, glyph);
                canvas_invert_color(canvas);
            } else if((i == line_num - 1) && strip_to_dots &&
                      (lines[i].x + (int32_t)glyph_width + (int32_t)dots_width >
                       x + (int32_t)width)) {
                canvas_draw_str(canvas, lines[i].x, lines[i].y, "...");
                break;
            } else {
                canvas_draw_glyph(canvas, lines[i].x, lines[i].y, glyph);
            }

            lines[i].x += glyph_width;
            byte_offset = next_offset;
        }
    }

    canvas_set_font(canvas, FontSecondary);
}
#endif /* FLIPPER_ZH_CN */

void elements_text_box(
    Canvas* canvas,
    int32_t x,
    int32_t y,
    size_t width,
    size_t height,
    Align horizontal,
    Align vertical,
    const char* text,
    bool strip_to_dots) {
    furi_check(canvas);
#ifdef FLIPPER_ZH_CN
    if(canvas_utf8_contains_cjk(text)) {
        elements_text_box_utf8(canvas, x, y, width, height, horizontal, vertical, text, strip_to_dots);
        return;
    }
#endif

    ElementTextBoxLine line[ELEMENTS_MAX_LINES_NUM];
    bool bold = false;
    bool mono = false;
    bool inverse = false;
    bool inverse_present = false;
    Font current_font = FontSecondary;
    Font prev_font = FontSecondary;
    const CanvasFontParameters* font_params = canvas_get_font_params(canvas, current_font);

    // Fill line parameters
    size_t line_leading_min = font_params->leading_min;
    size_t line_leading_default = font_params->leading_default;
    size_t line_height = font_params->height;
    size_t line_descender = font_params->descender;
    size_t line_num = 0;
    size_t line_width = 0;
    size_t line_len = 0;
    size_t total_height_min = 0;
    size_t total_height_default = 0;
    size_t i = 0;
    bool full_text_processed = false;
    size_t dots_width = canvas_string_width(canvas, "...");

    canvas_set_font(canvas, FontSecondary);

    // Fill all lines
    line[0].text = text;
    for(i = 0; !full_text_processed; i++) {
        line_len++;
        // Identify line height
        if(prev_font != current_font) {
            font_params = canvas_get_font_params(canvas, current_font);
            line_leading_min = MAX(line_leading_min, font_params->leading_min);
            line_leading_default = MAX(line_leading_default, font_params->leading_default);
            line_height = MAX(line_height, font_params->height);
            line_descender = MAX(line_descender, font_params->descender);
            prev_font = current_font;
        }
        // Set the font
        if(text[i] == '\e' && text[i + 1]) {
            i++;
            line_len++;
            if(text[i] == ELEMENTS_BOLD_MARKER) {
                if(bold) {
                    current_font = FontSecondary;
                } else {
                    current_font = FontPrimary;
                }
                canvas_set_font(canvas, current_font);
                bold = !bold;
            }
            if(text[i] == ELEMENTS_MONO_MARKER) {
                if(mono) {
                    current_font = FontSecondary;
                } else {
                    current_font = FontKeyboard;
                }
                canvas_set_font(canvas, FontKeyboard);
                mono = !mono;
            }
            if(text[i] == ELEMENTS_INVERSE_MARKER) {
                inverse_present = true;
            }
            continue;
        }
        if(text[i] != '\n') {
            line_width += canvas_glyph_width(canvas, text[i]);
        }
        // Process new line
        if(text[i] == '\n' || text[i] == '\0' || line_width > width) {
            if(line_width > width) {
                line_width -= canvas_glyph_width(canvas, text[i--]);
                line_len--;
            }
            if(text[i] == '\0') {
                full_text_processed = true;
            }
            if(inverse_present) {
                line_leading_min += 1;
                line_leading_default += 1;
                inverse_present = false;
            }
            line[line_num].leading_min = line_leading_min;
            line[line_num].leading_default = line_leading_default;
            line[line_num].height = line_height;
            line[line_num].descender = line_descender;
            if(total_height_min + line_leading_min > height) {
                break;
            }
            total_height_min += line_leading_min;
            total_height_default += line_leading_default;
            line[line_num].len = line_len;
            if(horizontal == AlignCenter) {
                line[line_num].x = x + (width - line_width) / 2;
            } else if(horizontal == AlignRight) {
                line[line_num].x = x + (width - line_width);
            } else {
                line[line_num].x = x;
            }
            line[line_num].y = total_height_min;
            line_num++;
            if(!full_text_processed) {
                line[line_num].text = &text[i + 1];
            }
            line_leading_min = font_params->leading_min;
            line_height = font_params->height;
            line_descender = font_params->descender;
            line_width = 0;
            line_len = 0;
        }
    }

    // Set vertical alignment for all lines
    if(total_height_default < height) {
        if(vertical == AlignTop) {
            line[0].y = y + line[0].height;
        } else if(vertical == AlignCenter) {
            line[0].y = y + line[0].height + (height - total_height_default) / 2;
        } else if(vertical == AlignBottom) {
            line[0].y = y + line[0].height + (height - total_height_default);
        }
        if(line_num > 1) {
            for(size_t i = 1; i < line_num; i++) {
                line[i].y = line[i - 1].y + line[i - 1].leading_default;
            }
        }
    } else if(line_num > 1) {
        size_t free_pixel_num = height - total_height_min;
        size_t fill_pixel = 0;
        size_t j = 1;
        line[0].y = y + line[0].height;
        while(fill_pixel < free_pixel_num) {
            line[j].y = line[j - 1].y + line[j - 1].leading_min + 1;
            fill_pixel++;
            j = j % (line_num - 1) + 1;
        }
    }

    // Draw line by line
    canvas_set_font(canvas, FontSecondary);
    bold = false;
    mono = false;
    inverse = false;
    for(size_t i = 0; i < line_num; i++) {
        for(size_t j = 0; j < line[i].len; j++) {
            // Process format symbols
            if(line[i].text[j] == '\e' && j < line[i].len - 1) { //-V781
                ++j;
                if(line[i].text[j] == ELEMENTS_BOLD_MARKER) {
                    if(bold) {
                        current_font = FontSecondary;
                    } else {
                        current_font = FontPrimary;
                    }
                    canvas_set_font(canvas, current_font);
                    bold = !bold;
                    continue;
                }
                if(line[i].text[j] == ELEMENTS_MONO_MARKER) {
                    if(mono) {
                        current_font = FontSecondary;
                    } else {
                        current_font = FontKeyboard;
                    }
                    canvas_set_font(canvas, current_font);
                    mono = !mono;
                    continue;
                }
                if(line[i].text[j] == ELEMENTS_INVERSE_MARKER) {
                    inverse = !inverse;
                    continue;
                }
            }
            if(inverse) {
                canvas_draw_box(
                    canvas,
                    line[i].x - 1,
                    line[i].y - line[i].height - 1,
                    canvas_glyph_width(canvas, line[i].text[j]) + 1,
                    line[i].height + line[i].descender + 2);
                canvas_invert_color(canvas);
                canvas_draw_glyph(canvas, line[i].x, line[i].y, line[i].text[j]);
                canvas_invert_color(canvas);
            } else {
                if((i == line_num - 1) && strip_to_dots) {
                    size_t next_symbol_width = canvas_glyph_width(canvas, line[i].text[j]);
                    if((line[i].x + (int32_t)next_symbol_width + (int32_t)dots_width) >
                       (x + (int32_t)width)) {
                        canvas_draw_str(canvas, line[i].x, line[i].y, "...");
                        break;
                    }
                }
                canvas_draw_glyph(canvas, line[i].x, line[i].y, line[i].text[j]);
            }
            line[i].x += canvas_glyph_width(canvas, line[i].text[j]);
        }
    }
    canvas_set_font(canvas, FontSecondary);
}
