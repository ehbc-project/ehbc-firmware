#ifndef HW_RTC_H__
#define HW_RTC_H__

#include "types.h"

#include <libehbcfw/device.h>
#include <time.h>

const char *rtc_get_name(struct device *dev);
const char *rtc_get_vendor(struct device *dev);

int rtc_reset(struct device *dev);

time_t rtc_get_time(struct device *dev);
void rtc_set_time(struct device *dev, time_t time);

time_t rtc_get_alarm(struct device *dev);
void rtc_set_alarm(struct device *dev, time_t time);

uint8_t rtc_read_nvsram(struct device *dev, int idx);
void rtc_write_nvsram(struct device *dev, int idx, uint8_t val);

#endif  // HW_RTC_H__
