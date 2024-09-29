#ifndef __SYSCALL_H__
#define __SYSCALL_H__

/*
Firmware System Call Table (TRAP #0)
Category Code = D0(31:16)
Function Code = D0(15:0)

Category    Description
0000h       Boot Services
0001h       Async I/O Services
0002h       Video Services
0003h       Keyboard / Pointing Device Services
0004h       Storage Services
0005h-FFFEh Reserved for future usage
FFFFh       Miscellaneous Functions

Category 0000h: Boot Services
    FC=0000h: Try to boot from next device
    FC=0001h: Deinitialize firmware and hide ROM from address space
        Return:
            D0 = Deinitialization error code

Category 0001h: Async I/O Services
    Description:
        Asynchronous input / output services for Serial port, Line printer, etc.
    FC=0000h: Initialize Port
        Parameters:
            D1(31:20): Baud Rate
                0h: 75 baud
                1h: 110 baud
                2h: 135 baud
                3h: 150 baud
                4h: 300 baud
                5h: 600 baud
                6h: 1200 baud
                Ah: 1800 baud
                7h: 2000 baud
                8h: 2400 baud
                9h: 4800 baud
                Bh: 9600 baud
                Ch: 19200 baud
                Dh-Fh: Reserved
            D1(19): Break
                0: No break
                1: Break
            D1(18:16): Parity Mode
                0xx: No parity
                100: Odd
                101: Even
                110: Stick parity odd
                111: Stick parity even
            D1(15:14): Stop Bit
                00: 1 stop bit
                01: 1.5 stop bits
                10: 2 stop bits
                11: 2 stop bits if word length is 6, 7, or 8
                    otherwise 1.5 stop bits
            D1(13:12): Word Length
                00: 5 bits
                01: 6 bits
                10: 7 bits
                11: 8 bits
            D1(7:0): Port Identifier
        Return:
            D0(31:16): Port Status
            D0(15:0): Error Code
    FC=0002h: Wait and Transmit Character
        Parameters:
            D1(7 + word length - 1:7): Character to send
            D1(7:0): Port Identifier
        Return:
            D0(31:16): Port Status
            D0(15:0): Error Code
    FC=0003h: Transmit Character
        Parameters:
            D1(7 + word length - 1:7): Character to send
            D1(7:0): Port Identifier
        Return:
            D0(31:16): Port Status
            D0(15:0): Error Code
    FC=0004h: Wait and Receive Character
        Parameters:
            D1(7:0): Port Identifier
        Return:
            D0(31:16): Port Status
            D0(15:0): Error Code
            D1: Received Character
    FC=0005h: Wait and Receive Character
        Parameters:
            D1(7:0): Port Identifier
        Return:
            D0(31:16): Port Status
            D0(15:0): Error Code
            D1: Received Character
    FC=0006h: Read Status
        Parameters:
            D1(7:0): Port Identifier
        Return:
            D0(31:16): Port Status
            D0(15:0): Error Code

Category 0002h: Video Services
    FC=0000h: Set Video Mode
        Parameters:
            D1(15:0): Video mode
        Return:
            A0: Video Mode Paramter Table pointer when success, 0 when fail
    FC=0001h: Get Video Mode Parameters
        Parameters:
            D1(15:0): Video Mode
        Return:
            A0: Video Mode Paramter Table pointer when success, 0 when fail
    FC=0002h: Get Video Adapter Status
        Return:
            D0(15:0): Current Video Mode
            A0: Video Mode Paramter Table pointer
    FC=0003h: Set Text Cursor Shape
        Parameters:
            D1(15:8): Scan Row Start
            D1(7:0): Scan Row End
    FC=0004h: Set Text Cursor Position
        Parameters:
            D1(15:8): Row
            D1(7:0): Column
    FC=0005h: Get Text Cursor Position and Shape
        Return:
            D0(31:24): Cursor Scan Row Start
            D0(23:16): Cursor Scan Row End
            D0(15:8): Row
            D0(7:0): Column
    FC=0006h: Scroll Text Area
        Parameters:
            D1(15:0): Lines to scroll (scroll up if positive, scroll down if
                      negative, clear if 0)
            D2: Text Color Attribute
            D3(31:24): Upper Row Index
            D3(23:16): Lower Row Index
            D3(15:8): Left Column Index
            D3(7:0): Right Column Index
    FC=0007h: Read Character and Attribute at Cursor Position
        Return:
            D0(15:8): Attribute
            D0(7:0): Character
    FC=0008h: Write Character and Attribute at Cursor Position
        Parameters:
            D1(15:8): Attribute
            D1(7:0): Character
    FC=0009h: Write String
        Parameters:
            D1(15:0): String Length
            D2(9): String contains attribute
            D2(8): Update Cursor
            D2(7:0): Attribute
            A0: String Pointer
    FC=000Ah: Write ANSI Teletype String
        Parameters:
            D1(15:0): String Length
            A0: String Pointer
    FC=000Bh: Set Text Font Data
        Parameters:
            A0: Glyph data pointer
    FC=000Ch: Set Palette
        Parameters:
            D1(15:0): Palette size in 3-byte unit
            A0: Palette pointer

Category 0003h: Keyboard / Pointing Device Services
    FC=0000h: Wait and Get Keyboard Stroke
        Return:
            D0(31:24): Device Identifier
            D0(7:0): ASCII Character
    FC=0001h: Get Keyboard Stroke
        Return:
            D0(31:24): Device Identifier
            D0(7:0): ASCII Character
    FC=0003h: Check Keyboard Status
        Parameters:
            D1(7:0): Device Identifier
        Return:
            D0(11): Insert active
            D0(10): CapsLock active
            D0(9): NumLock active
            D0(8): ScrollLock active
            D0(7): Left Alt pressed
            D0(6): Right Alt pressed
            D0(5): Left Ctrl pressed
            D0(4): Right Ctrl pressed
            D0(3): Left Meta pressed
            D0(2): Right Meta pressed
            D0(1): Left Shift pressed
            D0(0): Right Shift pressed
    FC=0004h: Flush Keyboard Buffer
    FC=0006h: Set Mouse Parameter
    FC=0005h: Get Mouse Status
    FC=0007h: Get Mouse Movement

Category 0004h: Storage Services
    FC=0000h: Reset Storage System
        Parameters:
            D1(7:0): Drive Identifier
    FC=0001h: Get Drive Status
        Parameters:
            D1(7:0): Drive Identifier
        Return:
            D0(31:0): Drive Status
    FC=0002h: Get Drive Parameters
        Parameters:
            D1(7:0): Drive Identifier
        Return:
            A0: Drive Parameter Table Pointer
    FC=0003h: Read Sectors CHS
        Parameters:
            D1(31:24): Drive Identifier
            D1(23:16): Head
            D1(15:6): Cylinder
            D1(5:0): Sector
            D2(7:0): Sectors to Read
            A0: Buffer Pointer
        Return:
            D0(31:0): Drive Status
            D1: Sectors Read
    FC=0004h: Write Sectors CHS
        Parameters:
            D1(31:24): Drive Identifier
            D1(23:16): Head
            D1(15:6): Cylinder
            D1(5:0): Sector
            D2(7:0): Sectors to Write
            A0: Buffer Pointer
        Return:
            D0(31:0): Drive Status
            D1: Sectors Written
    FC=0005h: Verify Sectors CHS
        Parameters:
            D1(31:24): Drive Identifier
            D1(23:16): Head
            D1(15:6): Cylinder
            D1(5:0): Sector
            D2(7:0): Sectors to Verify
            Buffer Pointer
        Return:
            D0(31:0): Drive Status
            D1: Sectors Verified
    FC=0006h: Read Sectors LBA
        Parameters:
            D1(31:24): Drive Identifier
            D1(23:16): Sectors to Read
            D1(15:6): LBA(47:32)
            D2: LBA(31:0)
            A0: Buffer Pointer
        Return:
            D0(31:0): Drive Status
            D1: Sectors Read
    FC=0007h: Write Sectors LBA
        Parameters:
            D1(31:24): Drive Identifier
            D1(23:16): Sectors to Write
            D1(15:6): LBA(47:32)
            D2: LBA(31:0)
            A0: Buffer Pointer
        Return:
            D0(31:0): Drive Status
            D1: Sectors Written

Category FFFFh: Miscellaneous Functions
    FC=0000h: Power Off System
    FC=0001h: Get Device Tree
        Return:
            A0: Device Tree Pointer
    FC=0002h: Get System Memory Map Table
        Return:
            A0: System Memory Map Table Pointer
    FC=0003h: Change Processor Clock
        Parameters:
            D1: Processor Clock in kHz unit
        Return:
            D0: Processor Clock Changed in kHz unit
 */

#endif // __SYSCALL_H__
