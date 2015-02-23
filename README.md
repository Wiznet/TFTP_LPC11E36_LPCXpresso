# NXP TFTP Example
Common to Any MCU, Easy to Add-on. Internet Offload co-Processor, HW TCP/IP chip, 
best fits for low-end Non-OS devices connecting to Ethernet for the Internet of Things. These will be updated continuously.

<!-- W5500 EVB pic -->
<p align="center">
  <img width="50%" src="http://wizwiki.net/wiki/lib/exe/fetch.php?media=products:w5500:w5500_evb:w5500-evb_side.png" />
</p>

## How to download the file from TFTP Server.
- Refer to W5500EVB+TFTP Project Tutorial http://youtu.be/oL7ptK-jsJE
- Refer to [WinAgents TFTP Server](http://www.winagents.com/en/products/tftp-server/)

## Related Project GitHub Repositories
- [W5500-EVB Main](https://github.com/Wiznet/W5500_EVB)
- [Loopback Test](https://github.com/Wiznet/Loopback_LPC11E36_LPCXpresso): Loopback test example project (TCP server / TCP client / UDP)
- [HTTP Server](https://github.com/Wiznet/HTTPServer_LPC11E36_LPCXpresso): Web server example project
- [FTP Server](https://github.com/Wiznet/FTP_LPC11E36_LPCXpresso): FTP server example project
- [SNMPv1 Agent](https://github.com/Wiznet/SNMP_LPC11E36_LPCXpresso): SNMPv1 agent example project (Get/Set/Trap)

## How to add a submodule of ioLibrary in project
- $ git submodule add git@github.com:Wiznet/ioLibrary_Driver.git project_src/ioLibrary
- $ git commit -m "description"
- $ git push

## How to clone a submodule of ioLibrary
- $ git clone git@github.com:Wiznet/TFTP_LPC11E36_LPCXpresso.git
- $ git submodule init
- $ git submodule update

## Revision History
Last release : Feb. 2015
