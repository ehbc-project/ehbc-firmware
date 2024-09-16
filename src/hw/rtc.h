#ifndef HW_RTC_H__
#define HW_RTC_H__

#include "types.h"

uint8_t rtc_read(int idx);
void rtc_write(int idx, uint8_t val);
int rtc_wait_update(void);
int rtc_init(void);

#endif  // HW_RTC_H__
