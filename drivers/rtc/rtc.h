#ifndef DRIVER_RTC_RTC_H__
#define DRIVER_RTC_RTC_H__

#include <ehbcfw/types.h>
#include <ehbcfw/device.h>

struct rtc_time {
    int year;
    int mon;
    int mday;
    int hour;
    int min;
    int sec;
    int msec;
    int isdst;
};

struct rtc_alarm {
    int enabled;
    int pending;
    struct rtc_time time;
};

struct rtc_feature {
    unsigned int time_resolution;
    unsigned int alarm_resolution;
};

struct rtc_device;

struct rtc_operations {
    int (*get_time)(struct rtc_device* device, struct rtc_time* time);
    int (*set_time)(struct rtc_device* device, const struct rtc_time* time);

    int (*get_alarm)(struct rtc_device* device, struct rtc_alarm* alarm);
    int (*set_alarm)(struct rtc_device* device, const struct rtc_alarm* alarm);

    int (*enable_alarm_irq)(struct rtc_device* device, int enable);

    int (*set_sqw_rate)(struct rtc_device* device, int hz);
    int (*get_sqw_rate)(struct rtc_device* device, int* hz);

    int (*get_features)(struct rtc_device* device, struct rtc_feature* feat);
};

struct rtc_driver {
    struct device_driver parent_driver;
    const struct rtc_operations* ops;
};

struct rtc_device {
    struct device dev;

    const struct rtc_driver* driver;

    unsigned int nvsram_size;
};

#define RTC_DEVICE(dev) ((struct rtc_device*)(dev))

#endif
