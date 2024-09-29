/*
  This code is based on SeaBIOS: https://github.com/coreboot/seabios
*/

#include "hw/rtc.h"
#include "memory.h"
#include "macros.h"

#include "io.h"

#define CMOS_INDEX        0x0070
#define CMOS_DATA         0x0071

// PORT_CMOS_INDEX nmi disable bit
#define NMI_DISABLE_BIT 0x80

// Standard BIOS RTC chip entries
#define CMOS_RTC_SECONDS         0x00
#define CMOS_RTC_SECONDS_ALARM   0x01
#define CMOS_RTC_MINUTES         0x02
#define CMOS_RTC_MINUTES_ALARM   0x03
#define CMOS_RTC_HOURS           0x04
#define CMOS_RTC_HOURS_ALARM     0x05
#define CMOS_RTC_DAY_WEEK        0x06
#define CMOS_RTC_DAY_MONTH       0x07
#define CMOS_RTC_MONTH           0x08
#define CMOS_RTC_YEAR            0x09
#define CMOS_STATUS_A            0x0a
#define CMOS_STATUS_B            0x0b
#define CMOS_STATUS_C            0x0c
#define CMOS_STATUS_D            0x0d
#define CMOS_RESET_CODE          0x0f

// RTC register flags
#define RTC_A_UIP 0x80

#define RTC_B_SET  0x80
#define RTC_B_PIE  0x40
#define RTC_B_AIE  0x20
#define RTC_B_UIE  0x10
#define RTC_B_BIN  0x04
#define RTC_B_24HR 0x02
#define RTC_B_DSE  0x01

uint8_t rtc_read(int idx)
{
    idx |= NMI_DISABLE_BIT;
    io_write_8(CMOS_INDEX, idx);
    return io_read_8(CMOS_DATA);
}

void rtc_write(int idx, uint8_t val)
{
    idx |= NMI_DISABLE_BIT;
    io_write_8(CMOS_INDEX, idx);
    io_write_8(CMOS_DATA, val);
}

int rtc_wait_update(void)
{
    for (int i = 0; i < 1024; i++) {
        if (!(rtc_read(CMOS_STATUS_A) & RTC_A_UIP)) return 0;
    }
    return -1;
}

int rtc_init(void)
{
    rtc_write(CMOS_STATUS_A, 0x26);
    uint8_t val = rtc_read(CMOS_STATUS_B);
    val &= RTC_B_DSE;
    val |= RTC_B_24HR;
    rtc_write(CMOS_STATUS_B,  val);
    rtc_read(CMOS_STATUS_C);
    rtc_read(CMOS_STATUS_D);

    return rtc_wait_update();
}
