#ifndef HW_I8042_H__
#define HW_I8042_H__

#include "types.h"

int ps2kbms_init();
void ps2kbms_poll();
int ps2_kb_command(int command, uint8_t *param);
int ps2_ms_command(int command, uint8_t *param);

#endif  // HW_I8042_H__
