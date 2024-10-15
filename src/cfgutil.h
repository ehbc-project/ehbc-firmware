#ifndef __CFGUTIL_H__
#define __CFGUTIL_H__

#include <stdint.h>

#define CEF_SELECTABLE  0x0001  // selectable by up/down arrow key
#define CFG_TICK        0x0002  // update when tick (e.g. current time)

#define CFG_TEXT_ENTRY(text) { .name = text, .type = CET_NOVALUE, }
#define CFG_BLANK_ENTRY { .name = NULL, .type = CET_NOVALUE, }

enum cfgutil_entry_type {
    CET_NOVALUE = 0,
    CET_STRING,
    CET_ENUM,
    CET_DATE,
    CET_TIME,
    CET_ENABLE,
    CET_SUBWIN,
    CET_EXEC,
};

struct cfgutil_window {
    struct cfgutil_window *next;
    const char *name;
    struct cfgutil_entry *entries, *selected;
    int scroll_pos;
};

struct cfgutil_entry {
    struct cfgutil_entry *next;
    const char *name;
    const char *help;
    uint16_t flags;
    enum cfgutil_entry_type type;
    int (*tick)(struct cfgutil_entry*);
    union {
        struct {
            char *str;
            unsigned int maxlen;
            int (*handler)(struct cfgutil_entry*, const char *str);
        } cfg_string;
        struct {
            const char *const *str_list;
            int selection;
            int (*handler)(struct cfgutil_entry*, int);
        } cfg_enum;
        struct {
            int year, mon, day;
            int (*handler)(struct cfgutil_entry*, int, int, int);
        } cfg_date;
        struct {
            int hour, min, sec;
            int (*handler)(struct cfgutil_entry*, int, int, int);
        } cfg_time;
        struct {
            int enable;
            int (*handler)(struct cfgutil_entry*, int);
        } cfg_enable;
        struct {
            struct cfgutil_window *win;
        } cfg_subwin;
        struct {
            int (*handler)(struct cfgutil_entry*);
        } cfg_exec;
    };
};

void cfgutil_add_root_window(struct cfgutil_window *win);
void cfgutil_add_entry(struct cfgutil_window *win, struct cfgutil_entry *entry);



#endif // __CFGUTIL_H__
