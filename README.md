# EHBC Firmware

Build commands:

```shell
git clone https://github.com/ehbc-project/ehbc-firmware
cd ehbc-firmware
cmake -S. -Bbuild
cmake --build build
```

To emulate firmware, download and build MAME from [here](https://github.com/ehbc-project/ehbc-mame).
The path of the ROM image is: `build/firmware.bin`

Demo
![demo](demo.gif)
