/*
  This code is based on SeaBIOS: https://github.com/coreboot/seabios
*/

#include "hw/ps2kbms.h"

#include <asm/io.h>

#include "debug.h"

// I/O port
#define I8042_DATA              0x0060
#define I8042_STATUS            0x0064
#define I8042_CMD               0x0064

// Standard commands
//      command                 send        recv       command
#define I8042_CMD_CTL_RCTR      ((0 << 12) | (1 << 8) | 0x20)
#define I8042_CMD_CTL_WCTR      ((1 << 12) | (0 << 8) | 0x60)
#define I8042_CMD_CTL_TEST      ((0 << 12) | (1 << 8) | 0xAA)

#define I8042_CMD_KBD_TEST      ((0 << 12) | (1 << 8) | 0xAB)
#define I8042_CMD_KBD_DISABLE   ((0 << 12) | (0 << 8) | 0xAD)
#define I8042_CMD_KBD_ENABLE    ((0 << 12) | (0 << 8) | 0xAE)

#define I8042_CMD_AUX_DISABLE   ((0 << 12) | (0 << 8) | 0xA7)
#define I8042_CMD_AUX_ENABLE    ((0 << 12) | (0 << 8) | 0xA8)
#define I8042_CMD_AUX_SEND      ((1 << 12) | (0 << 8) | 0xD4)

// Keyboard commands
#define ATKBD_CMD_SETLEDS       ((1 << 12) | (0 << 8) | 0xED)
#define ATKBD_CMD_SSCANSET      ((1 << 12) | (0 << 8) | 0xF0)
#define ATKBD_CMD_GETID         ((0 << 12) | (2 << 8) | 0xF2)
#define ATKBD_CMD_ENABLE        ((0 << 12) | (0 << 8) | 0xF4)
#define ATKBD_CMD_RESET_DIS     ((0 << 12) | (0 << 8) | 0xF5)
#define ATKBD_CMD_RESET_BAT     ((0 << 12) | (1 << 8) | 0xFF)

// Mouse commands
#define PSMOUSE_CMD_SETSCALE11  0x00E6
#define PSMOUSE_CMD_SETSCALE21  0x00E7
#define PSMOUSE_CMD_SETRES      0x10E8
#define PSMOUSE_CMD_GETINFO     0x03E9
#define PSMOUSE_CMD_GETID       0x02F2
#define PSMOUSE_CMD_SETRATE     0x10F3
#define PSMOUSE_CMD_ENABLE      0x00F4
#define PSMOUSE_CMD_DISABLE     0x00F5
#define PSMOUSE_CMD_RESET_BAT   0x02FF

// Status register bits.
#define I8042_STR_PARITY        0x80
#define I8042_STR_TIMEOUT       0x40
#define I8042_STR_AUXDATA       0x20
#define I8042_STR_KEYLOCK       0x10
#define I8042_STR_CMDDAT        0x08
#define I8042_STR_MUXERR        0x04
#define I8042_STR_IBF           0x02
#define I8042_STR_OBF           0x01

// Control register bits.
#define I8042_CTR_KBDINT        0x01
#define I8042_CTR_AUXINT        0x02
#define I8042_CTR_IGNKEYLOCK    0x08
#define I8042_CTR_KBDDIS        0x10
#define I8042_CTR_AUXDIS        0x20
#define I8042_CTR_XLATE         0x40

#define PS2_RET_ACK 0xFA
#define PS2_RET_NAK 0xFE

#define I8042_TIMEOUT           100000
#define I8042_BUFSIZE           16

// keyboard state bits
#define KS_CAPSLOCK             0x0001
#define KS_SCROLLLOCK           0x0002
#define KS_NUMLOCK              0x0004
#define KS_ALT                  0x0008
#define KS_CTRL                 0x0010
#define KS_SHIFT                0x0020
#define KS_META                 0x0040
#define KS_INSERT               0x0080
#define KS_LALT                 0x0100
#define KS_RALT                 0x0200
#define KS_LCTRL                0x0400
#define KS_RCTRL                0x0800
#define KS_LSHIFT               0x1000
#define KS_RSHIFT               0x2000
#define KS_LMETA                0x4000
#define KS_RMETA                0x8000

// keyboard scan code conversion table
#define SC_SPECIAL              -1

static const char scancode_table[2][0x50] = {
    {  // unshifted
        0, SC_SPECIAL, '1', '2', '3', '4', '5', '6',  /* 00h-07h */
        '7', '8', '9', '0', '-', '=', '\b', '\t',  /* 08h-0Fh */
        'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',  /* 10h-17h */
        'o', 'p', '[', ']', '\n', SC_SPECIAL, 'a', 's',  /* 18h-1Fh */
        'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',  /* 20h-27h */
        '\'', '`', SC_SPECIAL, '\\', 'z', 'x', 'c', 'v',  /* 28h-2Fh */
        'b', 'n', 'm', ',', '.', '/', SC_SPECIAL, SC_SPECIAL,  /* 30h-37h */
        SC_SPECIAL, ' ', SC_SPECIAL, SC_SPECIAL,  /* 38h-3Bh */
        SC_SPECIAL, SC_SPECIAL, SC_SPECIAL, SC_SPECIAL,  /* 3Ch-3Fh */
        SC_SPECIAL, SC_SPECIAL, SC_SPECIAL, SC_SPECIAL,  /* 40h-43h */
        SC_SPECIAL, SC_SPECIAL, SC_SPECIAL, SC_SPECIAL,  /* 44h-47h */
        SC_SPECIAL, SC_SPECIAL, '-', SC_SPECIAL,  /* 48h-4Bh */
        SC_SPECIAL, SC_SPECIAL, '+', SC_SPECIAL,  /* 4Ch-4Fh */
    },
    {  // shifted
        0, SC_SPECIAL, '!', '@', '#', '$', '%', '^',  /* 00h-07h */
        '&', '*', '(', ')', '_', '+', '\b', '\t',  /* 08h-0Fh */
        'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I',  /* 10h-17h */
        'O', 'P', '{', '}', '\n', SC_SPECIAL, 'A', 'S',  /* 18h-1Fh */
        'D', 'F', 'G', 'H', 'J', 'K', 'L', ':',  /* 20h-27h */
        '"', '~', SC_SPECIAL, '|', 'Z', 'X', 'C', 'V',  /* 28h-2Fh */
        'B', 'N', 'M', '<', '>', '?', SC_SPECIAL, SC_SPECIAL,  /* 30h-37h */
        SC_SPECIAL, ' ', SC_SPECIAL, SC_SPECIAL,  /* 38h-3Bh */
        SC_SPECIAL, SC_SPECIAL, SC_SPECIAL, SC_SPECIAL,  /* 3Ch-3Fh */
        SC_SPECIAL, SC_SPECIAL, SC_SPECIAL, SC_SPECIAL,  /* 40h-43h */
        SC_SPECIAL, SC_SPECIAL, SC_SPECIAL, SC_SPECIAL,  /* 44h-47h */
        SC_SPECIAL, SC_SPECIAL, '-', SC_SPECIAL,  /* 48h-4Bh */
        SC_SPECIAL, SC_SPECIAL, '+', SC_SPECIAL,  /* 4Ch-4Fh */
    },
};

static int i8042_wait_read()
{
    for (int i = 0; i < I8042_TIMEOUT; i++) {
        uint8_t status = io_read_8(I8042_STATUS);
        if (status & I8042_STR_OBF) return 0;
    }
    return 1;
}

static int i8042_wait_write()
{
    for (int i = 0; i < I8042_TIMEOUT; i++) {
        uint8_t status = io_read_8(I8042_STATUS);
        if (!(status & I8042_STR_IBF)) return 0;
    }
    return 1;
}

static int i8042_flush()
{
    for (int i = 0; i < I8042_BUFSIZE; i++) {
        uint8_t status = io_read_8(I8042_STATUS);
        if (!(status & I8042_STR_OBF)) return 0;
        uint8_t data = io_read_8(I8042_DATA);
    }
    return 1;
}

static int i8042_command(uint16_t command, uint8_t *param)
{
    int recv = (command >> 8) & 0xF;
    int send = (command >> 12) & 0xF;

    // debug_printf("i8042 command=%02X", (uint8_t)command);

    int ret = i8042_wait_write();
    if (ret) return ret;
    io_write_8(I8042_CMD, command);

    for (int i = 0; i < send; i++) {
        ret = i8042_wait_write();
        if (ret) return ret;
        // debug_printf(" >%02X", param[i]);
        io_write_8(I8042_DATA, param[i]);
    }

    for (int i = 0; i < recv; i++) {
        ret = i8042_wait_read();
        if (ret) return ret;
        param[i] = io_read_8(I8042_DATA);
        // debug_printf(" <%02X", param[i]);
    }

    // debug_printf("\n");

    return 0;
}

static int i8042_kbd_write(uint8_t c)
{
    int ret = i8042_wait_write();
    if (!ret) io_write_8(I8042_DATA, c);
    return ret;
}

static int i8042_aux_write(uint8_t c)
{
    return i8042_command(I8042_CMD_AUX_SEND, &c);
}

static uint8_t ps2_ctr = I8042_CTR_KBDDIS | I8042_CTR_AUXDIS;

static int ps2_recvbyte(int aux, int needack, int timeout)
{
    for (int i = 0; i < timeout; i++) {
        uint8_t status = io_read_8(I8042_STATUS);
        if (status & I8042_STR_OBF) {
            uint8_t data = io_read_8(I8042_DATA);

            if (!(status & I8042_STR_AUXDATA) == !aux) {
                return data;
            }
        }
    }
    return -1;
}

static int ps2_sendbyte(int aux, uint8_t command, int timeout)
{
    int ret;
    if (aux) {
        ret = i8042_aux_write(command);
    } else {
        ret = i8042_kbd_write(command);
    }
    if (ret) return ret;

    ret = ps2_recvbyte(aux, 1, timeout);
    if (ret < 0) return ret;
    // if (ret != PS2_RET_ACK) return 1;

    return 0;
}

static int ps2_command(int aux, uint16_t command, uint8_t *param)
{
    int recv = (command >> 8) & 0xf;
    int send = (command >> 12) & 0xf;

    // disable interrupt while sending command to the keyboard or mouse
    uint8_t ps2_ctr_orig = ps2_ctr;
    uint8_t ps2_ctr_temp = ps2_ctr;
    ps2_ctr_temp |= I8042_CTR_AUXDIS | I8042_CTR_KBDDIS;
    ps2_ctr_temp &= ~(I8042_CTR_KBDINT | I8042_CTR_AUXINT);
    int ret = i8042_command(I8042_CMD_CTL_WCTR, &ps2_ctr_temp);
    if (ret) return ret;

    ps2_ctr = ps2_ctr_temp;
    if (aux) {
        ps2_ctr_temp &= ~I8042_CTR_AUXDIS;
    } else {
        ps2_ctr_temp &= ~I8042_CTR_KBDDIS;
    }
    ret = i8042_command(I8042_CMD_CTL_WCTR, &ps2_ctr_temp);
    if (ret) goto fail;

    // debug_printf("ps2 %s cmd %02X", aux ? "aux" : "kbd", (uint8_t)command);

    if ((uint8_t)command == (uint8_t)ATKBD_CMD_RESET_BAT) {
        ret = ps2_sendbyte(aux, command, 1000);
        if (ret) goto fail;

        ret = ps2_recvbyte(aux, 0, 4000);
        if (ret < 0) goto fail;
        param[0] = ret;
        for (int i = 1; i < recv; i++) {
            ret = ps2_recvbyte(aux, 0, 500);
            if (ret < 0) goto fail;
            param[i] = ret;
        } 
    } else if (command == ATKBD_CMD_GETID) {
        ret = ps2_sendbyte(aux, command, 200);
        if (ret) goto fail;

        ret = ps2_recvbyte(aux, 0, 500);
        if (ret < 0) goto fail;
        param[0] = ret;

        switch (ret) {
            case 0xAB:
            case 0xAC:
            case 0x2B:
            case 0x5D:
            case 0x60:
            case 0x47:
                ret = ps2_recvbyte(aux, 0, 500);
                if (ret < 0) goto fail;
                param[1] = ret;
            default:
                param[1] = 0;
        }
    } else {
        ret = ps2_sendbyte(aux, command, 200);
        if (ret) goto fail;

        for (int i = 0; i < send; i++) {
            // debug_printf(" >%02X", param[i]);
            ret = ps2_sendbyte(aux, param[i], 200);
            if (ret)
                goto fail;
        }

        for (int i = 0; i < recv; i++) {
            ret = ps2_recvbyte(aux, 0, 500);
            if (ret < 0)
                goto fail;
            param[i] = ret;
            // debug_printf(" <%02X", param[i]);
        }
    }

    ret = 0;

fail:
    // restore original status
    ps2_ctr = ps2_ctr_orig;
    int ret2 = i8042_command(I8042_CMD_CTL_WCTR, &ps2_ctr_orig);
    if (ret2) return ret2;

    return ret;
}

static int ps2_kb_command(uint16_t command, uint8_t *param)
{
    return ps2_command(0, command, param);
}

static int ps2_ms_command(uint16_t command, uint8_t* param)
{
    return ps2_command(1, command, param);
}

void ps2kbms_irq_handler(struct device *dev)
{
    struct device_ps2kbms *param = dev->param;

    if ((ps2_ctr & (I8042_CTR_KBDDIS | I8042_CTR_AUXDIS)) ==
        (I8042_CTR_KBDDIS | I8042_CTR_AUXDIS)) {
        return;
    }

    uint8_t status, data;

    while ((status = io_read_8(I8042_STATUS)) & I8042_STR_OBF) {
        if (status & I8042_STR_AUXDATA) {
            if (!(ps2_ctr & I8042_CTR_AUXDIS)) {
                // process_mouse(data);
            }
        } else {
            if (!(ps2_ctr & I8042_CTR_KBDDIS)) {
                data = io_read_8(I8042_DATA);
                // debug_printf("ps2kbd data: %02X\n", data);
                ringbuf8_push(param->kbbuf, data);
            }
        }
    }
}

const char *ps2kb_get_name(struct device *dev)
{
    return "Generic PS/2 Keyboard";
}

const char *ps2kb_get_vendor(struct device *dev)
{
    return "Unknown";
}

const char *ps2ms_get_name(struct device *dev)
{
    return "Generic PS/2 Mouse";
}

const char *ps2ms_get_vendor(struct device *dev)
{
    return "Unknown";
}

int ps2kbms_probe(struct device *dev)
{
    return 0;
}

int ps2kbms_reset(struct device *dev)
{
    struct device_ps2kbms *dparam = dev->param;

    dparam->kbd_state = 0;
    dparam->mouse_x = 0;
    dparam->mouse_y = 0;

    int ret = i8042_flush();
    if (ret) return ret;

    // disable keyboard and mouse, then flush i8042 buffer
    ret = i8042_command(I8042_CMD_KBD_DISABLE, NULL);
    if (ret) return ret;
    ret = i8042_command(I8042_CMD_AUX_DISABLE, NULL);
    if (ret) return ret;
    ret = i8042_flush();
    if (ret) return ret;

    // i8042 self-test
    uint8_t param[2];
    ret = i8042_command(I8042_CMD_CTL_TEST, param);
    if (ret) return ret;
    if (param[0] != 0x55) return ret;

    // keyboard self-test
    ret = i8042_command(I8042_CMD_KBD_TEST, param);
    if (ret) return ret;
    if (param[0] != 0x00) return 1;

    // reset
    for (int i = 0; i < 100; i++) {
        ret = ps2_kb_command(ATKBD_CMD_RESET_BAT, param);
        if (!ret) break;
    }
    if (param[0] != 0xAA) return 1;

    ret = ps2_kb_command(ATKBD_CMD_RESET_DIS, NULL);
    if (ret) return ret;

    // set keyboard scan code set to 1
    param[0] = 0x01;
    ret = ps2_kb_command(ATKBD_CMD_SSCANSET, NULL);
    if (ret) return ret;

    ret = ps2_kb_command(ATKBD_CMD_ENABLE, NULL);
    if (ret) return ret;

    ps2_ctr = I8042_CTR_AUXDIS | I8042_CTR_XLATE | I8042_CTR_KBDINT;

    ret = ps2_kb_command(ATKBD_CMD_ENABLE, NULL);
    if (ret) return ret;

    return 0;
}

static int process_keystroke(struct device *dev)
{
    struct device_ps2kbms *param = dev->param;
    uint8_t seq[6];

    // get next code
    while (ringbuf8_pop(param->kbbuf, &seq[0])) {}

    int shifted;
    if (param->kbd_state & KS_CAPSLOCK) {
        shifted = (param->kbd_state & KS_SHIFT) ? 0 : 1;
    } else {
        shifted = (param->kbd_state & KS_SHIFT) ? 1 : 0;
    }

    if (0 < seq[0] && seq[0] < 0x50 &&
        scancode_table[shifted][seq[0]] != SC_SPECIAL) {
        return scancode_table[shifted][seq[0]];
    }

    switch (seq[0]) {
        case 0x1D:  // lctrl down
            param->kbd_state |= KS_CTRL | KS_LCTRL;
            return 0;
        case 0x9D:  // lctrl or rctrl up
            param->kbd_state &= ~(KS_CTRL | KS_LCTRL | KS_RCTRL);
            return 0;
        case 0x2A:  // lshift down
            param->kbd_state |= KS_SHIFT | KS_LSHIFT;
            return 0;
        case 0xAA:  // lshift up
            param->kbd_state &= ~KS_LSHIFT;
            if (!(param->kbd_state & (KS_LSHIFT | KS_RSHIFT))) {
                param->kbd_state &= ~KS_SHIFT;
            }
            return 0;
        case 0x36:  // rshift down
            param->kbd_state |= KS_SHIFT | KS_RSHIFT;
            return 0;
        case 0xB6:  // rshift up
            param->kbd_state &= ~KS_RSHIFT;
            if (!(param->kbd_state & (KS_LSHIFT | KS_RSHIFT))) {
                param->kbd_state &= ~KS_SHIFT;
            }
            return 0;
        case 0x38:  // lalt down
            param->kbd_state |= KS_ALT | KS_LALT;
            return 0;
        case 0xB8:  // lalt or ralt up
            param->kbd_state &= ~(KS_ALT | KS_LALT | KS_RALT);
            return 0;
        case 0x3A:  // capslock down
            // toggle capslock state
            if (param->kbd_state & KS_CAPSLOCK) {
                param->kbd_state &= ~KS_CAPSLOCK;
            } else {
                param->kbd_state |= KS_CAPSLOCK;
            }
            return 0;
        case 0x45:  // numlock down
            // toggle numlock state
            if (param->kbd_state & KS_NUMLOCK) {
                param->kbd_state &= ~KS_NUMLOCK;
            } else {
                param->kbd_state |= KS_NUMLOCK;
            }
            return 0;
        case 0x46:  // scrolllock down
            // toggle scrolllock state
            if (param->kbd_state & KS_SCROLLLOCK) {
                param->kbd_state &= ~KS_SCROLLLOCK;
            } else {
                param->kbd_state |= KS_SCROLLLOCK;
            }
            return 0;
        case 0xE0:
        case 0xE1:  // extended
            break;
        default:  // unhandled
            return 0;
    }

    // get next code
    while (ringbuf8_pop(param->kbbuf, &seq[1])) {}
    
    if (seq[0] == 0xE0) {
        switch (seq[1]) {
            case 0x35:  // numpad '/'
                return '/';
            case 0x1C:  // numpad enter
                return '\n';
            case 0x52:  // insert down
                // toggle insert state
                if (param->kbd_state & KS_INSERT) {
                    param->kbd_state &= ~KS_INSERT;
                } else {
                    param->kbd_state |= KS_INSERT;
                }
                return 0;
            case 0x38:  // ralt down
                param->kbd_state |= KS_ALT | KS_RALT;
                return 0;
            case 0x1D:  // rctrl down
                param->kbd_state |= KS_CTRL | KS_RCTRL;
                return 0;
            default:
                return 0;
        }
    } else if (seq[0] == 0xE1) {  // Pause
        if (seq[1] != 0x1D) return 0;

        while (ringbuf8_pop(param->kbbuf, &seq[2])) {}
        if (seq[2] != 0x45) return 0;

        while (ringbuf8_pop(param->kbbuf, &seq[3])) {}
        if (seq[3] != 0xE1) return 0;

        while (ringbuf8_pop(param->kbbuf, &seq[4])) {}
        if (seq[4] != 0x9D) return 0;

        while (ringbuf8_pop(param->kbbuf, &seq[5])) {}
        if (seq[5] != 0xC5) return 0;

        return 0;
    }

    return 0;
}

int ps2kb_get_key_char(struct device *dev)
{
    int ret;

    while (!(ret = process_keystroke(dev))) {}

    return ret;
}

int ps2kb_get_keystroke(struct device *dev)
{
    struct device_ps2kbms *param = dev->param;
    uint8_t data;

    if(ringbuf8_pop(param->kbbuf, &data)) {
        return 0;
    }

    return data;
}

int ps2kb_get_keyboard_flags(struct device *dev)
{
    struct device_ps2kbms *param = dev->param;
    return param->kbd_state;
}

void ps2kb_flush_buffer(struct device *dev)
{
    struct device_ps2kbms *param = dev->param;
    ringbuf8_flush(param->kbbuf);
}
