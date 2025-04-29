# Linux SDR Final Project

## Information and Overview

EN.525.742.SP25\
28 April 2025\
Jeremiah Schollar

This project contains everything needed to run and operate a simulated software-defined radio (SDR) in a Linux environment. The SDR can be controlled using a web interface that is hosted on a Zybo-Z7 development board.

The ***vivado_Schollar*** directory has tcl scripting that can be used to create a Vivado project containing the necessary PL design in the form of a .bit.bin file. This is done with the same methodology used in previous labs.
The ***web_Schollar.tgz*** compressed directory contains the files needed to operate the web interface.

## PL Design Summary

The PL was designed as mentioned in class to include:

- The *full_radio* peripheral to accept frequency inputs and generate appropriate outputs
- The *lowlevel_dac_if* IP to accept *full_radio* peripheral outputs and play via headphone jack
- An Axi4-Stream FIFO IP to provide access to the *full_radio* peripheral outputs from the PS
- An Axi4-Stream Broadcaster IP to transmit the *full_radio* peripheral outputs to both the *lowlevel_dac_if* IP and the Axi4-Stream FIFO IP

## Running Web Interface

The ***web_Schollar.tgz*** compressed directory can be moved directly to a Zybo board and extracted using `tar -xvf web_Schollar.tgz`. Once extracted, `cd` to the *web* directory and execute `sudo httpd -p 8080 .` to enable the webpage at port 8080. The webpage will use the files contained within the *cgi-bin* directory to execute necessary tasks.

## A Note on File Locations

The ***web_Schollar.tgz*** compressed directory contains all source files needed to operate the SDR from the webpage. This allows for easy plug-and-play once it has been moved onto a Zybo board and extracted. Files used to perform any actions - whether they are C files, Python files, or Bash scripts - can also be found in the *vivado_Schollar/src/linux_software/* directory.
