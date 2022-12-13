# W5100S EVB
- W5100S chip development platform for **net'enabled** microcotroller applications
- Ethernet (W5100S Hardwired TCP/IP chip) and 32-bit ARM® Cortex™-M3 based designs
- Arduino Pin-compatible platform hardware

[link-w5100s-evb]: https://github.com/Wiznet/W5100S-EVB/tree/master/static/images/w5100s-evb.png

For more details, please refer to [W5100S-EVB page](https://docs.wiznet.io/Product/iEthernet/W5100S/w5100s-evb) in WIZnet Docs page.

## Features
- WIZnet W5100S Hardwired TCP/IP chip
  - Hardwired TCP/IP embedded Ethernet controller
  - Parallel Host Interface (External BUS Interface)
  - SPI (Serial Peripheral Interface) Microcontroller Interface
  - Hardwired TCP/IP stack supports TCP, UDP, IPv4, ICMP, ARP, IGMP, and PPPoE protocols
  - Easy to implement of the other network protocols
  
- STMicroelectronics STM32F103VCT6 MCU
  - 32-bit ARM® Cortex™-M3 microcontroller running at up to 72MHz
  - 256kB on-chip flash / 48kB on-chip SRAM / Various peripherals
- Pin-compatible with Arduino Shields designed for the UNO Rev3
- 2 x Push button switch(SW), 1 x RGB LED
- 1-Channel 10/100Mbps Ethernet Connector (RJ-45 with transformer) – (If you want to RJ-45, Contact us)
- Micro USB B connector and 5-pin(2.54mm) Cortex debug connector for SWD(Serial Wire Debug)

- W5100S-EVB Arduino Compatible Pinout
![W5100S-EVB Arduino Compatible Pinout](https://docs.wiznet.io/assets/images/arduino_swd_pinout-6dd964d0354541e6aa999bfee62d903a.png "W5100S-EVB Arduino Compatible Pinout")

- W5100S-EVB External Pinout(left)
![W5100S-EVB External Pinout(left)](https://docs.wiznet.io/assets/images/expansion_pinout_left_v3-0c83eddd276bc8f4af413d9f7e50cb79.png "W5100S-EVB External Pinout(left)")

- W5100S-EVB External Pinout(Right)
![W5100S-EVB External Pinout(Right)](https://docs.wiznet.io/assets/images/expansion_pinout_right_v3-06712f1ef8b750365707af62baa36920.png "W5100S-EVB External Pinout(Right)")

## Software
These are libraries source code and example projects based on STM32CubeIDE
If you want to base on TrueStudio or Ecplipse, change git branch master to truestudio_ver
- W5100S-EVB TrueStudio or Ecplipse Git
 [W5100S-EVB TrueStudio or Ecplipse Git](https://github.com/Wiznet/W5100S-EVB/tree/truestudio_ver)

The tree of Directory is below.

```
W5100S-EVB
    ┣ FSMC_DMA or SPI_DMA
    ┣ Core
    ┃  ┣ Inc
    ┃  ┃  ┗ main.h
    ┃  ┣ Src
    ┃  ┃  ┗ main.c
    ┃  ┗ Startup
    ┗ Drivers
       ┣ CMSIS
       ┣ STM32F1xx_HAL_Driver
       ┗ ioLibrary_Driver
          ┣ Application
          ┣ Ethernet
          ┗ Internet

```


### ioLibrary GitHub Repository
- [ioLibrary](https://github.com/Wiznet/ioLibrary_Driver) : Latest WIZnet chip drivers and protocol libraries for developers

## Hardware material, Documents and Others
Various materials are could be found at [W5100S-EVB page](https://docs.wiznet.io/Product/iEthernet/W5100S/w5100s-evb) in WIZnet Docs.
And refer to this link [W5100S Hardware data](https://github.com/Wiznet/Hardware-Files-of-WIZnet/tree/master/02_iEthernet) 
- Documents
  - Getting Started: Hello world! / Downloading a new program
  - Make New W5100S-EVB Projects
- Technical Reference
  - Datasheet
  - Schematic
  - Partlist
  - Demension
- See also



## Revision History
- First release : May. 2018
- 0.0.0.2       : Dec. 2022
