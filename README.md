# Linux SDR Final Project

## Information and Overview

EN.525.742.SP25\
28 April 2025\
Jeremiah Schollar

This project contains everything needed to run and operate a simulated software-defined radio (SDR) in a Linux environment. The SDR can be controlled using a web interface that is hosted on a Zybo-Z7 development board.

The *vivado_Schollar* directory has tcl scripting that can be used to create a Vivado project containing the necessary PL design in the form of a .bit.bin file. This is done with the same methodology used in previous labs.
The *web_Schollar.tgz* compressed directory contains the files needed to operate the web interface.

## PL Design Summary

This will describe the PL design. Mention that it consists of the ***full_radio*** peripheral, a Broadcaster IP, the lowlevel_dac_intfc IP to output sound from headphone jack, and a FIFO to send UDP data packets for plotting with MATLAB. Talk more about this?
