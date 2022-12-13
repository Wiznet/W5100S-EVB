# How to Test Loopback Example


## Step 1: Prepare software

The following serial terminal programs are required for Loopback example test, download and install from below links.

- [**ST Flash loader demonstrator**][link-st-flash]
- [**Tera Term**][link-tera_term]
- [**Hercules**][link-hercules]



## Step 2: Prepare hardware

1. Connect ethernet cable to W5100S-EVB ethernet port.

2. Connect W5100S-EVB to desktop or laptop using 5 pin micro USB cable.


## Step 3: Setup Loopback Example

To test the Loopback example, minor settings shall be done in code.

1. If you want to test with the Loopback example using SPI, Set Mode switch [0 0]

![][link-w5100s-evb-mode-sw-spi]

And if you want to using DMA, defined comment in main.h

```cpp
/* Use SPI DMA */
//#define USE_DMA 
```

2. Setup network configuration such as IP in 'main.c' which is the Loopback example in 'SPI_DMA/Core/Src/' directory.

```cpp
/* Network */
static wiz_NetInfo g_net_info =
    {
        .mac ={0x00, 0x08, 0xDC, 0x12, 0x34, 0x56}, // MAC address
        .ip = {192, 168, 11, 2},                     // IP address
        .sn = {255, 255, 255, 0},                    // Subnet Mask
        .gw = {192, 168, 11, 1},                     // Gateway
        .dns = {8, 8, 8, 8},                         // DNS server
        .dhcp = NETINFO_STATIC                       // DHCP enable/disable
};
```

3. Setup loopback configuration in 'main.c' in 'SPI_DMA/Core/Src/' directory.

```cpp
/* Port */
#define PORT_LOOP   5000
```


## Step 4: Build

1. After completing the Loopback example configuration, mouse right click to Project name and selsect 'Build Project'

![][link-build-project]

2. When the build is completed, 'W5100S_EVB_SPI_DMA.hex' is generated in '\SPI_DMA\Debug' or \SPI_DMA\Release' directory.


## Step 5: Upload and Run
### Flash programming via UART
STMicroelectronics Flash loader demonstrator is a program to perform in-system programming (ISP) of the MCU flash via its UART.

1. Press the 'Boot0' push button switch on board until turn on the board after reset or power supply.

2. Set the settings on main window of Flash loader demonstrator program. The figure below shows the default configuration for W5100S-EVB

![][link-flash_down]

3. refer to below pictures If you do not go to the next page in flash loader demonstrator, users try again this action. 'Press the 'Boot0' push button switch on board until turn on the board after reset or power supply.'

![][link-flash_down2]

4. After finish to flash programming and board reset, The MCU do running the program
When the running to loopback program, as below picture serial debug message print out.

![][link-w5100s-evb-terminal-start]

5. Connect to the open loopback server using Hercules TCP client. When connecting to the loopback server, you need to enter is the IP that was configured in Step 3, the port is 5000 by default.

![][link-connect_to_loopback_server_using_hercules_tcp_client_1]

![][link-connect_to_loopback_server_using_hercules_tcp_client_2]

6. Once connected if you send data to the loopback server, you should be able to receive back the sent message.

![][link-receive_back_sent_message]



<!--
Link
-->

[link-tera_term]: https://osdn.net/projects/ttssh2/releases/
[link-hercules]: https://www.hw-group.com/software/hercules-setup-utility
[link-st-flash]: https://www.st.com/en/development-tools/flasher-stm32.html
[link-w5100s-evb-mode-sw-spi]: https://github.com/Wiznet/W5100S-EVB/blob/master/static/images/w5100s-evb-mode-sw-spi.png
[link-build-project]: https://github.com/Wiznet/W5100S-EVB/blob/master/static/images/build-project.png
[link-flash_down]: https://github.com/Wiznet/W5100S-EVB/blob/master/static/images/flash_down.png
[link-flash_down2]: https://github.com/Wiznet/W5100S-EVB/blob/master/static/images/flash_down2.png
[link-w5100s-evb-terminal-start]: https://github.com/Wiznet/W5100S-EVB/blob/master/static/images/w5100s-evb-terminal-start.png
[link-connect_to_loopback_server_using_hercules_tcp_client_1]: https://github.com/Wiznet/W5100S-EVB/blob/master/static/images/connect_to_loopback_server_using_hercules_tcp_client_1.png
[link-connect_to_loopback_server_using_hercules_tcp_client_2]: https://github.com/Wiznet/W5100S-EVB/blob/master/static/images/connect_to_loopback_server_using_hercules_tcp_client_2.png
[link-receive_back_sent_message]: https://github.com/Wiznet/W5100S-EVB/blob/master/static/images/loopback/receive_back_sent_message.png
