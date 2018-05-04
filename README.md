# W5100S EVB
- W5100S chip development platform for **net'enabled** microcotroller applications
- Ethernet (W5100S Hardwired TCP/IP chip) and 32-bit ARM® Cortex™-M3 based designs
- Arduino Pin-compatible platform hardware
- New Code Samples: [Updated network protocol libraries and example projects](https://github.com/Wiznet/W5500_EVB/blob/master/README.md#related-project-github-repositories)

<!-- W5500 EVB pic -->
![W5100S-EVB](http://wizwiki.net/wiki/lib/exe/fetch.php?media=products%3Aw5100s%3Aw5100s_evb%3Astart&media=products:w5100s:w5100s_evb:w5100s-evb_partdescription.png "W5100S-EVB")

For more details, please refer to [W5100S-EVB page](http://wizwiki.net/wiki/doku.php?id=products:w5100s:w5100s_evb:start) in WIZnet Wiki.

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
![W5100S-EVB Arduino Compatible Pinout](http://wizwiki.net/wiki/lib/exe/fetch.php?media=products%3Aw5100s%3Aw5100s_evb%3Astart&media=products:w5100s:w5100s_evb:arduino_swd_pinout.png "W5100S-EVB Arduino Compatible Pinout")
- W5100S-EVB External Pinout(left)
![W5100S-EVB External Pinout(left)](http://wizwiki.net/wiki/lib/exe/fetch.php?media=products:w5100s:w5100s_evb:expansion_pinout_left_v3.png "W5100S-EVB External Pinout(left)")

- W5100S-EVB External Pinout(Right)
![W5100S-EVB External Pinout(Right)](http://wizwiki.net/wiki/lib/exe/fetch.php?media=products:w5100s:w5100s_evb:expansion_pinout_right_v3.png "W5100S-EVB External Pinout(Right)")

## Software
These are libraries source code and example projects based on Ecplipse & TrueSTUDIO IDE.

Requried Libraries are as below.
- Folder list
  - ioLibrary_Driver (WIZnet W5100S-EVB ethernet library and protocols)
  - CMSIS
  - CMSIS_boot
  - stm_lib (STM Library)
  - syscalls
- File list
  - main.c
  - HALInit.c
  - HALInit.h
  - msgq.c
  - msgq.h
  - serialCommand.c
  - serialCommand.h
  - W5100SRelFunctions.c
  - W5100SRelFunctions.h

Example projects are as below.
- Loopback

### ioLibrary GitHub Repository
- [ioLibrary](https://github.com/Wiznet/ioLibrary_Driver) : Latest WIZnet chip drivers and protocol libraries for developers

For more details, please refer to [W5100S-EVB page](http://wizwiki.net/wiki/doku.php?id=products:w5100s:w5100s_evb:start) in WIZnet Wiki.

## Hardware material, Documents and Others
Various materials are could be found at [W5100S-EVB page](http://wizwiki.net/wiki/doku.php?id=products:w5100s:w5100s_evb:start#technical_reference) in WIZnet Wiki.
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
First release : May. 2018
