#include "types.h"

void main(void)
{
    hwreg8_t* escc_ch0_cmd  = (void*)0xFF000002;
    hwreg8_t* escc_ch0_data = (void*)0xFF000003;
    hwreg8_t* escc_ch1_cmd  = (void*)0xFF000000;
    hwreg8_t* escc_ch1_data = (void*)0xFF000001;

    uint8_t asdf;

    // init ch0
    // reset ch0
    *escc_ch0_cmd = 0x09;
    *escc_ch0_cmd = 0x80;

    // 8bpc (rx)
    *escc_ch0_cmd = 0x03;
    *escc_ch0_cmd = 0xC0;

    // 8bpc (tx)
    *escc_ch0_cmd = 0x05;
    *escc_ch0_cmd = 0x60;

    // no parity, 1 stop bit
    *escc_ch0_cmd = 0x04;
    *escc_ch0_cmd = 0x04;

    // enable rx
    *escc_ch0_cmd = 0x03;
    *escc_ch0_cmd = 0xC1;

    // enable tx
    *escc_ch0_cmd = 0x05;
    *escc_ch0_cmd = 0x64;
    // print chars
    const char* text = "Hello, World!";

    for (; *text; text++) {
        while (!(*escc_ch0_cmd & 0x40));
        *escc_ch0_data = (uint8_t)*text;
    }
}
