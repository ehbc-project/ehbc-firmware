# EHBC Firmware

## How To Build And Use

```shell
git clone https://github.com/ehbc-project/ehbc-firmware
cd ehbc-firmware
cmake -S. -Bbuild
cmake --build build
```

To emulate firmware, download and build MAME from [here](https://github.com/ehbc-project/ehbc-mame).
The path of the ROM image is: `build/firmware.bin`

## Demo

![demo](demo.gif)

## Brief Description of How This Thing Works

1. Do board-specific initialization routine.
2. Do architecture-specific intitialization routine.
3. Do additional board-specific initialization routine and enumerate on-board devices
4. Initialize on-board devices
5. Load firmware configuration data
6. Enumerate and initialize additional devices
7. Run the configuration utility and reset if needed
8. Search for storage devices
9. Mount volume and find a bootloader binary
10. Load the binary and jump to its entry point
11. Return to 8 if the bootloader terminates
12. If there's no more bootable volume left, print error message and halt

## License Information

Firmware itself follows GPL 3.0 and LGPL 3.0 and libehbcfw follows BSD 3-Clause.
