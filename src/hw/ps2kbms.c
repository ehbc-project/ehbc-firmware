/*
  This code is based on SeaBIOS: https://github.com/coreboot/seabios
*/

#include "hw/ps2kbms.h"

#include "io.h"

// I/O port
#define I8042_DATA              0x0060
#define I8042_STATUS            0x0064

// Standard commands
#define I8042_CMD_CTL_RCTR      0x0120
#define I8042_CMD_CTL_WCTR      0x1060
#define I8042_CMD_CTL_TEST      0x01aa

#define I8042_CMD_KBD_TEST      0x01ab
#define I8042_CMD_KBD_DISABLE   0x00ad
#define I8042_CMD_KBD_ENABLE    0x00ae

#define I8042_CMD_AUX_DISABLE   0x00a7
#define I8042_CMD_AUX_ENABLE    0x00a8
#define I8042_CMD_AUX_SEND      0x10d4

// Keyboard commands
#define ATKBD_CMD_SETLEDS       0x10ed
#define ATKBD_CMD_SSCANSET      0x10f0
#define ATKBD_CMD_GETID         0x02f2
#define ATKBD_CMD_ENABLE        0x00f4
#define ATKBD_CMD_RESET_DIS     0x00f5
#define ATKBD_CMD_RESET_BAT     0x01ff

// Mouse commands
#define PSMOUSE_CMD_SETSCALE11  0x00e6
#define PSMOUSE_CMD_SETSCALE21  0x00e7
#define PSMOUSE_CMD_SETRES      0x10e8
#define PSMOUSE_CMD_GETINFO     0x03e9
#define PSMOUSE_CMD_GETID       0x02f2
#define PSMOUSE_CMD_SETRATE     0x10f3
#define PSMOUSE_CMD_ENABLE      0x00f4
#define PSMOUSE_CMD_DISABLE     0x00f5
#define PSMOUSE_CMD_RESET_BAT   0x02ff

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

#define I8042_TIMEOUT           100000
#define I8042_BUFSIZE           16

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

static int i8042_command(int command, uint8_t *param)
{
    int recv = (command >> 8) & 0xF;
    int send = (command >> 12) & 0xF;

    int ret = i8042_wait_write();
    if (ret) return ret;
    io_write_8(I8042_STATUS, command);

    for (int i = 0; i < send; i++) {
        ret = i8042_wait_write();
        if (ret) return ret;
        io_write_8(I8042_DATA, param[i]);
    }

    for (int i = 0; i < recv; i++) {
        ret = i8042_wait_read();
        if (ret) return ret;
        param[i] = io_read_8(I8042_DATA);
    }

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

#define PS2_RET_ACK 0xFA
#define PS2_RET_NAK 0xFE

uint8_t ps2_ctr = I8042_CTR_KBDDIS | I8042_CTR_AUXDIS;

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
    if (ret != PS2_RET_ACK) return 1;

    return 0;
}

static int ps2_command(int aux, int command, uint8_t *param)
{
    int recv = (command >> 8) & 0xf;
    int send = (command >> 12) & 0xf;

    uint8_t ps2_ctr_new = ps2_ctr;
    ps2_ctr_new |= I8042_CTR_AUXDIS | I8042_CTR_KBDDIS;
    ps2_ctr_new &= ~(I8042_CTR_KBDINT | I8042_CTR_AUXINT);
    int ret = i8042_command(I8042_CMD_CTL_WCTR, &ps2_ctr_new);
    if (ret) return ret;

    ps2_ctr = ps2_ctr_new;
    if (aux) {
        ps2_ctr_new &= ~I8042_CTR_AUXDIS;
    } else {
        ps2_ctr_new &= ~I8042_CTR_KBDDIS;
    }
    ret = i8042_command(I8042_CMD_CTL_WCTR, &ps2_ctr_new);
    if (ret) goto fail;

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
            ret = ps2_sendbyte(aux, param[i], 200);
            if (ret)
                goto fail;
        }

        for (int i = 0; i < recv; i++) {
            ret = ps2_recvbyte(aux, 0, 500);
            if (ret < 0)
                goto fail;
            param[i] = ret;
        }
    }

    ret = 0;

fail:
    ps2_ctr = ps2_ctr_new;
    int ret2 = i8042_command(I8042_CMD_CTL_WCTR, &ps2_ctr);
    if (ret2) return ret2;

    return ret;
}

int ps2_kb_command(int command, uint8_t *param)
{
    return ps2_command(0, command, param);
}

int ps2_ms_command(int command, uint8_t* param)
{
    return ps2_command(1, command, param);
}

void ps2kbms_poll()
{
    if ((ps2_ctr & (I8042_CTR_KBDDIS | I8042_CTR_AUXDIS)) ==
        (I8042_CTR_KBDDIS | I8042_CTR_AUXDIS)) {
        return;
    }


    uint8_t status = io_read_8(I8042_STATUS);
    if (!(status & I8042_STR_OBF)) return;
    uint8_t data = io_read_8(I8042_DATA);
    if (status & I8042_STR_AUXDATA) {
        if (!(ps2_ctr & I8042_CTR_AUXDIS)) {
            // process_mouse(data);
        }
    } else {
        if (!(ps2_ctr & I8042_CTR_KBDDIS)) {
            // process_keyboard(data);
        }
    }
}

int ps2kbms_init()
{
    int ret = i8042_flush();
    if (ret) return ret;

    ret = i8042_command(I8042_CMD_KBD_DISABLE, NULL);
    if (ret) return ret;
    ret = i8042_command(I8042_CMD_AUX_DISABLE, NULL);
    if (ret) return ret;
    ret = i8042_flush();
    if (ret) return ret;

    uint8_t param[2];
    ret = i8042_command(I8042_CMD_CTL_TEST, param);
    if (ret) return ret;
    if (param[0] != 0x55) return ret;

    ret = i8042_command(I8042_CMD_KBD_TEST, param);
    if (ret) return ret;
    if (param[0] != 0x00) return 1;

    for (int i = 0; i < 100; i++) {
        ret = ps2_kb_command(ATKBD_CMD_RESET_BAT, param);
        if (!ret) break;
    }
    if (param[0] != 0xAA) return 1;

    ret = ps2_kb_command(ATKBD_CMD_RESET_DIS, NULL);
    if (ret) return ret;

    param[0] = 0x02;
    ret = ps2_kb_command(ATKBD_CMD_SSCANSET, NULL);
    if (ret) return ret;

    ret = ps2_kb_command(ATKBD_CMD_ENABLE, NULL);
    if (ret) return ret;

    ps2_ctr = I8042_CTR_AUXDIS | I8042_CTR_XLATE;

    return 0;
}

