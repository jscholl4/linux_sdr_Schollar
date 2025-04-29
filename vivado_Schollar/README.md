# EN.525.742.SP25 Final Project PL Design

This contains everything needed to create the .bit.bin file and run on an FPGA. All changes to the full_radio peripheral IP are found in the ip_repo/full_radio/ directory, while the hdl files for the IP are found in the src/ directory.

The PL design includes the full_radio peripheral which is split into two output streams using a Broadcaster IP. The output of the full_radio peripheral is sent to the lowlevel_dac IP (to play as output to the headphone jack), as well as to an AXI-Stream FIFO IP. This allows the PS to grab the output of the full_radio peripheral and send it out in UDP packets.
