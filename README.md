# Getting started with Blinky on mbed OS

This is a very simple guide, reviewing the steps required to get Blinky working on an mbed OS platform.

Please install [mbed CLI](https://github.com/ARMmbed/mbed-cli#installing-mbed-cli).

## Get the example application!

From the command line, import the example:

```
mbed import https://github.com/MXCHIP/mbed-wifi-example
cd mbed-wifi-example
```

### Now compile

Invoke `mbed compile` specifying the name of your platform and your favorite toolchain (`GCC_ARM`, `ARM`, `IAR`). For example, for the ARM Compiler 5:

```
mbed compile -m AZ3166 -t GCC_ARM
```

Your PC may take a few minutes to compile your code. At the end you should get the following result:

```
+-----------------------+--------+-------+-------+
| Module                |  .text | .data |  .bss |
+-----------------------+--------+-------+-------+
| Fill                  |    366 |    15 |   121 |
| Misc                  | 158722 |  2564 | 51384 |
| drivers               |   2739 |     4 |   164 |
| features/filesystem   |    627 |     0 |     0 |
| features/netsocket    |   3907 |    85 |    60 |
| hal                   |    518 |     0 |     8 |
| platform              |   2069 |     4 |   297 |
| rtos                  |    213 |     4 |     4 |
| rtos/rtx              |   8163 |    20 |  6874 |
| targets/TARGET_MXCHIP |   2574 |     4 |     0 |
| targets/TARGET_STM    |  23328 |     0 |  1424 |
| Subtotals             | 203226 |  2700 | 60336 |
+-----------------------+--------+-------+-------+
Allocated Heap: unknown
Allocated Stack: unknown
Total Static RAM memory (data + bss): 63036 bytes
Total RAM memory (data + bss + heap + stack): 63036 bytes
Total Flash memory (text + data + misc): 205926 bytes

Image: ./BUILD/AZ3166/GCC_ARM/mbed-wifi-example.bin
```

### Program your board
#### First choise (with Microsoft Azure IOT Developer Kit)

1.Connect your mbed device to the compoter over USB.

2.You will find a removable device USB Mass Storage named AZ3166

3.send mbed-wifi-example.bin into the device

4.reboot your deivce,the application will excute automatically

#### Second choise

1. Connect your mbed device to the computer over USB.
2. Open serial terminal on PC, like secureCRT, with serial configuration: 115200/8/n/1.
3. Reboot your MiCOKit with "BOOT SEL" set to ON, you can see connamds list on serial port.


```
MICO bootloader for EMW3239_1, v3.0, HARDWARE_REVISION: 1.0
+ command -------------------------+ function ------------+
| 0:BOOTUPDATE    <-r>             | Update bootloader    |
| 1:FWUPDATE      <-r>             | Update application   |
| 2:DRIVERUPDATE  <-r>             | Update RF driver     |
| 3:PARUPDATE     <-id n><-r><-e>  | Update MICO partition|
| 4:FLASHUPDATE   <-dev device>    |                      |
|  <-e><-r><-start addr><-end addr>| Update flash content |
| 5:MEMORYMAP                      | List flash memory map|
| 6:BOOT                           | Excute application   |
| 7:REBOOT                         | Reboot               |
+----------------------------------+----------------------+
|    (C) COPYRIGHT 2015 MXCHIP Corporation  By William Xu |
 Notes:
 -e Erase only  -r Read from flash -dev flash device number
  -start flash start address -end flash start address
 Example: Input "4 -dev 0 -start 0x400 -end 0x800": Update 
          flash device 0 from 0x400 to 0x800

MXCHIP> 
```

4. Input command "1" and press return, send mbed_blink.bin using Ymodem protocol.
5. Input command "boot" and press return, MiCOKit will excute the new firmware
6. Reboot your MiCOKit with "BOOT SEL" set to OFF, application will excute automatically

You should see the LED of your platform turning on and off.

Congratulations if you managed to complete this test!

## Export the project to Keil MDK and debug your application

From the command line, run the following command:

```
mbed export -m MK3239 -i uvision
```

To debug the application:

1. Start uVision.
2. Import the uVision project generated earlier.
3. Compile your application and generate an `.axf` file.
4. Make sure uVision is configured to debug over CMSIS-DAP (From the Project menu > Options for Target '...' > Debug tab > Use CMSIS-DAP Debugger).
5. Set breakpoints and start a debug session.

## Troubleshooting

1. Make sure `mbed-cli` is working correctly and its version is `>1.0.0`

 ```
 mbed --version
 ```

 If not, you can update it easily:

 ```
 pip install mbed-cli --upgrade
 ```

2. If using Keil MDK, make sure you have a license installed. [MDK-Lite](http://www.keil.com/arm/mdk.asp) has a 32KB restriction on code size.
