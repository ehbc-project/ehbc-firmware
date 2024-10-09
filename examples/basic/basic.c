#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <macros.h>
#include <libehbcfw/syscall.h>

#include "mpool.h"

#define MAX_LINE 80

////////////////////////////////////////////////////////////////////////////////
// program line data structure section

struct program_line {
    struct program_line *prev, *next;
    unsigned int line_num;
    unsigned int indent_level;
    char str[];
};

static struct program_line *line_head = NULL;

static int create_prog_line(unsigned int line_num, const char *str)
{
    struct program_line *current, *next, *prev;
    unsigned int line_len = strnlen(str, MAX_LINE);

    current = line_head;
    if (!line_head) {
        current = mpool_alloc(sizeof(struct program_line) + line_len + 1);
        line_head = current;
        next = NULL;
        prev = NULL;
    } else {
        while (current->next) {
            if (line_num == current->line_num) {
                return -1;
            } else if (current->line_num < line_num &&
                (!current->next || line_num < current->next->line_num)) {
                break;
            }

            current = current->next;
        }
        next = current->next;
        prev = current;
        current = mpool_alloc(sizeof(struct program_line) + line_len + 1);
    }

    if (!current) {
        return -1;
    }

    current->indent_level = 0;
    current->line_num = line_num;
    current->next = next;
    current->prev = prev;
    strcpy(current->str, str);

    if (next) next->prev = current;
    if (prev) prev->next = current;

    return 0;
}

static struct program_line *find_prog_line(unsigned int line_num)
{
    struct program_line *current = line_head, *next;

    if (!line_head) {
        return NULL;
    }

    while (current) {
        if (line_num == current->line_num) {
            return current;
        } else if (line_num < current->line_num) {
            break;
        }

        current = current->next;
    }

    return NULL;
}

static int edit_prog_line(unsigned int line_num, const char *str)
{
    struct program_line *line = find_prog_line(line_num);
    if (!line) {
        return create_prog_line(line_num, str);
    }
    int is_head = line == line_head;

    unsigned int line_len = strnlen(str, MAX_LINE);

    line = mpool_realloc(line, sizeof(struct program_line) + line_len + 1);
    if (!line) {
        return 1;
    }

    strcpy(line->str, str);
    if (line->prev) line->prev->next = line;
    if (line->next) line->next->prev = line;
    if (is_head) line_head = line;

    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// variable section

struct variable {
    struct variable *prev, *next;
    unsigned int line_num;
    unsigned int indent_level;
    char name[16];
    union {
        long value_integer;
        double value_float;
    };
    char value_str[];
};

////////////////////////////////////////////////////////////////////////////////
// parser section

enum token_type {
    TOKEN_INVALID = 0,

    TOKEN_SYMBOL = 128,
    TOKEN_STRING,
    TOKEN_INTEGER,
    TOKEN_NUMERIC,
};

static const char *parse_str;
static unsigned int parse_str_len;

static int current_token_type;
static const char *current_token;
static unsigned int current_token_len;

static struct program_line *current_line = NULL;
static struct program_line *next_line = NULL;

static void advance_token() {
    current_token += current_token_len;
}

static int parse_token() {
    current_token_len = 0;
    while (*current_token == ' ') { current_token++; }
    switch (*current_token) {
        case '\0':
            return 1;
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '&':
        case '$':
        case ':':
        case '[':
        case ']':
        case '{':
        case '}':
        case '(':
        case ')':
        case ',':
        case '.':
        case '<':
        case '>':
        case '^':
        case '=':
            current_token_type = *current_token;
            current_token_len = 1;
            break;
        case '"':
        case '`': {
            current_token_type = TOKEN_STRING;
            current_token_len++;
            char quote_chr = *current_token;
            const char *token = current_token + 1;
            
            for (;;) {
                if (*token == quote_chr) {
                    current_token_len++;
                    token++;
                    if (*token != quote_chr) {
                        break;
                    }
                }
                current_token_len++;
                token++;
            }
            break;
        }
        default:
            if (isdigit(*current_token)) {
                // integer or numeric token
                current_token_type = TOKEN_INTEGER;
                current_token_len++;
                const char *token = current_token + 1;
                while (isdigit(*token) || *token == '.') {
                    current_token_len++;
                    token++;
                    if (*token == '.') {
                        current_token_type = TOKEN_NUMERIC;
                    }
                }
            } else if (isalpha(*current_token)) {
                // symbol name token
                current_token_len++;
                const char *token = current_token + 1;
                while (isalnum(*token) || *token == '.') {
                    current_token_len++;
                    token++;
                }
                current_token_type = TOKEN_SYMBOL;
            } else {
                current_token_len = 1;
                current_token_type = TOKEN_INVALID;
            }
            break;
    }
    return 0;
}

static int parse_integer(long *value)
{
    if (current_token_type != TOKEN_INTEGER) {
        return 1;
    }

    *value = 0;

    for (int i = 0; i < current_token_len; i++) {
        *value *= 10;
        *value += current_token[i] - '0';
    }

    return 0;
}

static int parse_stmt_print(void)
{
    if (parse_token() || current_token_type != TOKEN_STRING) {
        return 1;
    }
    
    char quote_chr = *current_token;
    char buf[128], *buf_ptr = buf;
    
    for (int i = 1; i < current_token_len - 1; i++) {
        if (current_token[i] == quote_chr) {
            i++;
        }
        *buf_ptr++ = current_token[i];
    }
    *buf_ptr = 0;
    
    printf("%s\r\n", buf);

    advance_token();

    return 0;
}

static int parse_stmt_goto(void)
{
    if (parse_token() || current_token_type != TOKEN_INTEGER) {
        return 1;
    }

    long line_num;
    parse_integer(&line_num);
    advance_token();

    if (!next_line) {
        next_line = current_line;
    }

    while (next_line && next_line->line_num > line_num) {
        next_line = next_line->prev;
    }
    while (next_line && next_line->line_num < line_num) {
        next_line = next_line->next;
    }

    if (!next_line || next_line->line_num != line_num) {
        return 1;
    }

    return 0;
}

static int parse_stmt_end(void)
{
    next_line = NULL;

    return 0;
}

static int parse_stmt_rem(void)
{
    return 0;
}

static const struct {
    const char *name;
    int (*parser)(void);
} statements[] = {
    { "CASE",       NULL },
    { "CEND",       NULL },
    { "CHAIN",      NULL },
    { "CLEAR",      NULL },
    { "CLOSE",      NULL },
    { "COMMON",     NULL },
    { "CSI",        NULL },
    { "DATA",       NULL },
    { "DEF",        NULL },
    { "DELETE",     NULL },
    { "DIM",        NULL },
    { "ELSE",       NULL },
    { "END",        parse_stmt_end },
    { "FNEND",      NULL },
    { "FOR",        NULL },
    { "GET",        NULL },
    { "GOSUB",      NULL },
    { "GOTO",       parse_stmt_goto },
    { "IF",         NULL },
    { "IFEND",      NULL },
    { "INPUT",      NULL },
    { "LET",        NULL },
    { "LINK",       NULL },
    { "LINPUT",     NULL },
    { "MAT",        NULL },
    { "MOUNT",      NULL },
    { "NEXT",       NULL },
    { "OPTION",     NULL },
    { "OTHERWISE",  NULL },
    { "PRINT",      parse_stmt_print },
    { "PUT",        NULL },
    { "QUIT",       NULL },
    { "RANDOMIZE",  NULL },
    { "READ",       NULL },
    { "READNEXT",   NULL },
    { "REM",        parse_stmt_rem },
    { "RESTORE",    NULL },
    { "RESUME",     NULL },
    { "RETURN",     NULL },
    { "RUN",        NULL },
    { "SELECT",     NULL },
    { "SLEEP",      NULL },
    { "STOP",       NULL },
    { "THEN",       NULL },
    { "UNLOCK",     NULL },
    { "WAIT",       NULL },
    { "WEND",       NULL },
    { "WHILE",      NULL },
    { "WRITE",      NULL },
};

static int parse_statement(void)
{
    if (parse_token() || current_token_type != TOKEN_SYMBOL) {
        return 1;
    }

    for (int i = 0; i < ARRAY_SIZE(statements); i++) {
        if (strncasecmp(statements[i].name, current_token, current_token_len) == 0) {
            advance_token();
            if (statements[i].parser) {
                return statements[i].parser();
            }
        }
    }
    
    return 1;
}

static void prepare_parser(const char *str, unsigned int len)
{
    current_token_len = 0;
    current_token = parse_str = str;
    parse_str_len = len;
}

////////////////////////////////////////////////////////////////////////////////
// command section

static int edit_cursor = 10;
static int auto_inc = 0;

static int command_auto(void)
{
    if (parse_token()) {
        return 1;
    }

    switch (current_token_type) {
        case '.':
            break;
        case TOKEN_INTEGER: {
            long line_num;
            if (parse_integer(&line_num)) {
                return 1;
            }
            edit_cursor = line_num;
        } break;
        case TOKEN_SYMBOL:
            if (strncasecmp("OFF", current_token, current_token_len) == 0) {
                auto_inc = 0;
            }
            return 0;
        default:
            return 1;
    }
    advance_token();

    if (parse_token()) {
        auto_inc = 10;
        return 0;
    }
    if (current_token_type != ',') {
        return 1;
    }
    advance_token();

    if (parse_token() || current_token_type != TOKEN_INTEGER) {
        return 1;
    }

    long inc;
    if (parse_integer(&inc) || inc < 1) {
        return 1;
    }
    advance_token();

    auto_inc = inc;
    return 0;
}

static int command_edit(void)
{
    if (parse_token()) {
        return 1;
    }

    switch (current_token_type) {
        case '.':
            break;
        case TOKEN_INTEGER: {
            long line_num;
            if (parse_integer(&line_num)) {
                return 1;
            }
            edit_cursor = line_num;
        } break;
        default:
            return 1;
    }

    struct program_line *line = find_prog_line(edit_cursor);
    if (!line) return 1;

    printf("%u %s\r\n", line->line_num, line->str);

    return 0;
}

static int command_run(void)
{
    current_line = line_head;
    if (line_head) next_line = line_head->next;

    while (current_line) {
        next_line = current_line->next;
        prepare_parser(current_line->str, strlen(current_line->str));
        parse_statement();
        current_line = next_line;
    }

    return 0;
}

static int command_list(void)
{
    struct program_line *line = line_head;
    while (line) {
        printf("%u %s\r\n", line->line_num, line->str);
        line = line->next;
    }

    return 0;
}

static int command_free(void)
{
    printf("%lu BYTES FREE\r\n", mpool_getfree());

    return 0;
}

static const struct {
    const char *name;
    int (*parser)(void);
} commands[] = {
    { "AUTO",       command_auto },
    { "EDIT",       command_edit },
    { "RUN",        command_run },
    { "LIST",       command_list },
    { "FREE",       command_free },
};

static int parse_command(void)
{
    if (parse_token() || current_token_type != TOKEN_SYMBOL) {
        return 1;
    }

    for (int i = 0; i < ARRAY_SIZE(commands); i++) {
        if (strncasecmp(commands[i].name, current_token, current_token_len) == 0) {
            advance_token();
            if (commands[i].parser) {
                return commands[i].parser();
            }
        }
    }

    return 1;
}


////////////////////////////////////////////////////////////////////////////////
// user interface section

static int read_line(char *buf, int buflen)
{
    int len = 0;
    char rxbyte;

    do {
        if (ehbcfw_aio_rx(0, &rxbyte)) break;

        switch (rxbyte) {
            case '\r':
                printf("\r\n");
                goto finish;
            case '\b':
                if (len > 0) {
                    printf("\b \b");
                    *--buf = 0;
                    len--;
                }
                break;
            default:
                ehbcfw_aio_tx(0, rxbyte);
                *buf++ = rxbyte;
                len++;
        }
    } while (rxbyte && len < buflen);

finish:
    *buf = 0;
    return len;
}

static int parse_line(const char *str, unsigned int len)
{
    prepare_parser(str, len);

    if (parse_token()) {
        return 1;
    }

    switch (current_token_type) {
        case TOKEN_INTEGER: {
            long line_num;
            parse_integer(&line_num);
            advance_token();
            edit_prog_line(line_num, current_token);
        } break;
        case TOKEN_SYMBOL: {
            if (parse_command() && auto_inc) {
                prepare_parser(str, len);
                edit_prog_line(edit_cursor, str);
                edit_cursor += auto_inc;
            }
        } break;
        default:
            printf("SYNTAX ERROR\r\n");
            return 1;
    }
    return 0;
}

void main(void)
{
    mpool_init((void*)0x10000, 65536);

    printf("BASIC Interpreter\r\n");
    printf("%lu BYTES FREE\r\n", mpool_getfree());

    char linebuf[128];
    for (;;) {
        parse_line(linebuf, read_line(linebuf, sizeof(linebuf)));
    }
}
