#include "driver/rtc/rtc.h"

static int mc146818_reset(struct device* device)
{
    return 1;
}

static int mc146818_get_time(struct rtc_device* device, struct rtc_time* time)
{
    return 1;
}

static int mc146818_set_time(struct rtc_device* device, const struct rtc_time* time)
{
    return 1;
}

static const struct rtc_operations mc146818_rtc_ops = {
    .get_time = mc146818_get_time,
    .set_time = mc146818_set_time,
};

const struct rtc_driver mc146818_rtc_driver = {
    .parent_driver = {
        .name = "motorola:mc146818",
    },
    .ops = &mc146818_rtc_ops,
};


