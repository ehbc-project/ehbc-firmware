#include "cfgutil.h"

#include <stddef.h>
#include <string.h>

#include <libehbcfw/syscall.h>

#include "debug.h"

#define CFGUTIL_TITLE "EHBC Firmware Configuration Utility"
#define CFGUTIL_TITLE_LEN (sizeof(CFGUTIL_TITLE) - 1)

#define TERM_COL    80
#define TERM_ROW    25

#define FG_BLACK    0x00
#define FG_BLUE     0x01
#define FG_GREEN    0x02
#define FG_CYAN     0x03
#define FG_RED      0x04
#define FG_MAGENTA  0x05
#define FG_BROWN    0x06
#define FG_WHITE    0x07
#define FG_BBLACK   0x08
#define FG_BBLUE    0x09
#define FG_BGREEN   0x0A
#define FG_BCYAN    0x0B
#define FG_BRED     0x0C
#define FG_BMAGENTA 0x0D
#define FG_BYELLOW  0x0E
#define FG_BWHITE   0x0F
#define BG_BLACK    0x00
#define BG_BLUE     0x10
#define BG_GREEN    0x20
#define BG_CYAN     0x30
#define BG_RED      0x40
#define BG_MAGENTA  0x50
#define BG_BROWN    0x60
#define BG_WHITE    0x70

static struct cfgutil_window *root_win_head = NULL;
static struct cfgutil_window *head = NULL, *current;

static int refresh;

void cfgutil_add_root_window(struct cfgutil_window *win)
{
    struct cfgutil_window *prev = root_win_head;

    if (!root_win_head) {
        root_win_head = win;
    } else {
        while (prev->next) {
            prev = prev->next;
        }
        prev->next = win;
    }

    win->next = NULL;
}

void cfgutil_add_entry(struct cfgutil_window *win, struct cfgutil_entry *entry)
{
    struct cfgutil_entry *prev = win->entries;

    if (!win->entries) {
        win->entries = entry;
    } else {
        while (prev->next) {
            prev = prev->next;
        }
        prev->next = entry;
    }

    entry->next = NULL;
}

static void cfgutil_draw_box(int top, int bottom, int left, int right, int attr)
{
    ehbcfw_video_set_cursor_pos(2, TERM_COL * top + left);
    ehbcfw_video_write_char_attr(2, 0xDA, attr);
    for (int i = left + 1; i < right; i++) {
        ehbcfw_video_set_cursor_pos(2, TERM_COL * top + i);
        ehbcfw_video_write_char_attr(2, 0xC4, attr);
    }
    ehbcfw_video_set_cursor_pos(2, TERM_COL * top + right);
    ehbcfw_video_write_char_attr(2, 0xBF, attr);

    for (int i = top + 1; i < bottom; i++) {
        ehbcfw_video_set_cursor_pos(2, TERM_COL * i + left);
        ehbcfw_video_write_char_attr(2, 0xB3, attr);
        ehbcfw_video_set_cursor_pos(2, TERM_COL * i + right);
        ehbcfw_video_write_char_attr(2, 0xB3, attr);
    }

    ehbcfw_video_set_cursor_pos(2, TERM_COL * bottom + left);
    ehbcfw_video_write_char_attr(2, 0xC0, attr);
    for (int i = left + 1; i < right; i++) {
        ehbcfw_video_set_cursor_pos(2, TERM_COL * bottom + i);
        ehbcfw_video_write_char_attr(2, 0xC4, attr);
    }
    ehbcfw_video_set_cursor_pos(2, TERM_COL * bottom + right);
    ehbcfw_video_write_char_attr(2, 0xD9, attr);
}

static void cfgutil_draw_separator_vertical(int col, int top, int bottom)
{

}

static void cfgutil_draw_separator_horizontal(int row, int left, int right)
{

}

static void cfgutil_print_string(int row, int col, const char *str, int len)
{
    ehbcfw_video_set_cursor_pos(2, TERM_COL * row + col);
    ehbcfw_video_write_string(2, str, len, VA_AUTO);
}

static void cfgutil_print_string_wrap(int top, int bottom, int left, int right, const char *str)
{

}

static void cfgutil_set_attr(int top, int bottom, int left, int right, int attr)
{

}

static void cfgutil_erase(int top, int bottom, int left, int right, int attr)
{
    ehbcfw_video_scroll_area(2, 0, attr, top, bottom, left, right);
}

static void cfgutil_scroll(int top, int bottom, int left, int right, int attr, int amount)
{

}

static const char *const shortcut_help[] = {
    "\x1A\x1B  Select Tab",  "\x18\x19  Select Entry", "Enter Select",      "+/-  Change Option",
    "F1 General Help", "F3  Use Default",  "F10   Save & Exit", "ESC  Exit",
};

static void cfgutil_draw_base(void)
{
    // erase screen
    cfgutil_erase(0, 1, 0, TERM_COL - 1, FG_WHITE | BG_BLUE);
    cfgutil_erase(2, TERM_ROW - 2, 0, TERM_COL - 1, FG_BLUE | BG_WHITE);
    cfgutil_erase(TERM_ROW - 1, TERM_ROW - 1, 0, TERM_COL - 1, FG_WHITE | BG_BLUE);

    // header
    cfgutil_print_string(0, (TERM_COL - CFGUTIL_TITLE_LEN) / 2, CFGUTIL_TITLE, CFGUTIL_TITLE_LEN);
    
    // TODO: footer

    // box & separator
    cfgutil_set_attr(2, TERM_ROW - 2, 0, TERM_COL - 1, FG_BLUE | BG_WHITE);
    cfgutil_draw_box(2, TERM_ROW - 2, 0, TERM_COL - 1, FG_BLUE | BG_WHITE);
    cfgutil_draw_separator_horizontal(TERM_ROW - 5, 0, TERM_COL - 1);
    cfgutil_draw_separator_vertical(TERM_COL - 24, 2, TERM_ROW - 5);

    // help screen
    cfgutil_print_string(3, TERM_COL - 14, "Help", 4);

    // shortcuts
    int grid_width = (TERM_COL - 4) / 4;
    for (int i = 0; i < 4; i++) {
        cfgutil_print_string(TERM_ROW - 4, 2 + i * grid_width, shortcut_help[i], grid_width);
    }
    for (int i = 0; i < 4; i++) {
        cfgutil_print_string(TERM_ROW - 3, 2 + i * grid_width, shortcut_help[i + 4], grid_width);
    }
}

static void cfgutil_draw_menubar(struct cfgutil_window *head)
{
    int col = 1;
    cfgutil_erase(1, 1, 0, TERM_COL - 1, FG_WHITE | BG_BLUE);
    while (head) {
        int name_len = strlen(head->name);
        if (current == head) {  // highlight if it's selected window
            cfgutil_erase(1, 1, col, col + name_len + 1, FG_BLUE | BG_WHITE);
        } else {
            cfgutil_erase(1, 1, col, col + name_len + 1, FG_WHITE | BG_BLUE);
        }
        cfgutil_print_string(1, col + 1, head->name, name_len);
        col += name_len + 2;
        head = head->next;
    }
}

static void cfgutil_draw_window(struct cfgutil_window *win)
{
    struct cfgutil_entry *entry = win->entries;
    int entry_index = 0;

    cfgutil_erase(3, TERM_ROW - 6, 1, TERM_COL - 24, FG_BLUE | BG_WHITE);

    while (entry) {
        int entry_row = entry_index - win->scroll_pos + 3;
        // draw only if entry is inside the boundary
        if (entry_row >= TERM_ROW - 5) {
            break;
        } else if (entry_row >= 0) {
            if (win->selected == entry) {
                // mark selected entry
                cfgutil_print_string(entry_row, 2, ">", 1);
                // print help string
                cfgutil_print_string_wrap(4, TERM_ROW - 5, TERM_COL - 23, TERM_COL - 1, entry->help);
            }

            // print name
            if (entry->type == CET_NOVALUE) {
                if (entry->name) {
                    cfgutil_print_string(entry_row, 4, entry->name, TERM_COL - 28);
                }
            } else {
                cfgutil_print_string(entry_row, 4, entry->name, TERM_COL - 42);
            }

            // print value
            switch (entry->type) {
                case CET_STRING:
                    cfgutil_print_string(entry_row, TERM_COL - 38, entry->cfg_string.str, 24);
                    break;
                case CET_ENUM: {
                    const char *value = entry->cfg_enum.str_list[entry->cfg_enum.selection];
                    cfgutil_print_string(entry_row, TERM_COL - 38, value, 24);
                } break;
                case CET_DATE:
                case CET_TIME:
                    break;
                case CET_ENABLE: {
                    const char *value = entry->cfg_enable.enable ? "[Enabled]" : "[Disabled]";
                    cfgutil_print_string(entry_row, TERM_COL - 38, value, 24);
                } break;
                case CET_SUBWIN:
                case CET_EXEC:
                    cfgutil_print_string(entry_row, TERM_COL - 38, "[Enter]", 24);
                    break;
                default:
                    break;
            }
        }

        entry = entry->next;
        entry_index++;
    }
}

static void cfgutil_prev_window(void)
{
    if (current == head) return;

    struct cfgutil_window *prev = current;
    current = head;
    while (current->next != prev) {
        current = current->next;
    }
    current->selected = current->entries;
    if (current != prev) {
        refresh = 1;
    }
}

static void cfgutil_next_window(void)
{
    if (current->next) {
        current = current->next;
        current->selected = current->entries;
        refresh = 1;
    }
}

static void cfgutil_prev_entry(void)
{
    if (current->selected == current->entries) return;

    struct cfgutil_entry *prev = current->selected, *entry = current->entries;
    while (entry && entry != prev) {
        if (entry->flags & CEF_SELECTABLE) {
            current->selected = entry;
        }
        entry = entry->next;
    }
    if (current->selected != prev) {
        refresh = 1;
    }
}

static void cfgutil_next_entry(void)
{
    struct cfgutil_entry *entry = current->selected->next;
    
    while (entry) {
        if (entry->flags & CEF_SELECTABLE) {
            current->selected = entry;
            refresh = 1;
            break;
        }
        entry = entry->next;
    }
}

static void cfgutil_esc(void)
{
    head = root_win_head;
    current = head;
    current->selected = current->entries;
    refresh = 1;
}

static void cfgutil_enter(void)
{
    struct cfgutil_entry *entry = current->selected;

    switch (entry->type) {
        case CET_EXEC:
            if (entry->cfg_exec.handler) {
                entry->cfg_exec.handler(entry);
                refresh = 1;
            }
            break;
        case CET_ENABLE:
            if (entry->cfg_enable.handler) {
                entry->cfg_enable.handler(entry, !entry->cfg_enable.enable);
                refresh = 1;
            }
            break;
        case CET_SUBWIN:
            if (entry->cfg_subwin.win) {
                head = entry->cfg_subwin.win;
                current = head;
                current->selected = current->entries;
                refresh = 1;
            }
            break;
        default:
            break;
    }
}

void cfgutil(void)
{
    int keystroke;

    head = root_win_head;
    current = head;
    current->selected = current->entries;

    refresh = 1;

    ehbcfw_video_set_cursor_shape(2, 0x2607);
    cfgutil_draw_base();

    for (;;) {
        if (refresh) {
            cfgutil_draw_menubar(head);
            cfgutil_draw_window(current);
            refresh = 0;
        }

        while (!(keystroke = ehbcfw_kbd_get_keystroke(3))) {}

        switch (keystroke) {
            case 0x01:
                cfgutil_esc();
                break;
            case 0x1C:
                cfgutil_enter();
                break;
            case 0xE0:
                while (!(keystroke = ehbcfw_kbd_get_keystroke(3))) {}
                switch (keystroke) {
                    case 0x4B:  // left
                        cfgutil_prev_window();
                        break;
                    case 0x4D:  // right
                        cfgutil_next_window();
                        break;
                    case 0x48:  // up
                        cfgutil_prev_entry();
                        break;
                    case 0x50:  // down
                        cfgutil_next_entry();
                        break;
                    default:
                        break;
                }
                break;
            case 0x44:
                goto end;
            default:
                break;
        }
    }

end:
    cfgutil_erase(0, TERM_ROW - 1, 0, TERM_COL -1, FG_WHITE | BG_BLACK);

    ehbcfw_video_set_cursor_pos(2, 0);
    ehbcfw_video_set_cursor_shape(2, 0x0C0F);

    return;
}
