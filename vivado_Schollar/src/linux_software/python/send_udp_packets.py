#!/usr/bin/python3
#-----------------------------------------------------------
#
# Author: Jeremiah Schollar
#
# Date: 15 April 2025
#
# Description: 
#       This is Milestone 1 for the Linux SDR project.
#       
#       This script is used to send a specified number of
#       UDP packets to a specified IP address at Port 25344.
#       
#       The destination IP address and number of packets are
#       given as arguments. A shebang is included at the
#       beginning of the script so it can be called directly
#       from a Linux command line.
#       
#       The UDP packets are structured according to what
#       was laid out in the Lab 11 appendix.
#           - bytes 0-3: unsigned 32-bit counter
#           - bytes 4-1027: alternating signed 16-bit I and Q
#             data
#       
#       The I and Q data packets consist of entirely random
#       data.
#       
#       Example of executing from command line:
#       
#           ---------------------------------------
#           | ./send_udp_packets 192.168.1.100 10 |
#           ---------------------------------------
#       
#           (This would send 10 UDP packets to
#            192.168.1.100:25344)
#
#-----------------------------------------------------------

import socket
import struct
import time
import random
import argparse

def send_packets(udp_socket, args, dest_port):
    # Loop to generate and send specified number of packets
    for i in range(int(args.num_packets)):
        # Generate header packet that includes counter
        data_packet = struct.pack("<I",i) # 32-bit unsigned int (4 bytes)

        # Loop to add I and Q data packets (this is random data so they don't need to be differentiated)
        for j in range(512):
            data_packet = data_packet + struct.pack("<h",random.randint(1, 15000)) # 16-bit signed int (2 bytes)

        # Send packet and sleep to simulate 48.8 kHz sampling frequency
        udp_socket.sendto(data_packet, (args.dest_ip,dest_port))
        time.sleep(0.00002048)

    # Print confirmation to terminal
    print(f"Sent {i+1} packets total")

if __name__== "__main__":

    # Initialize variables
    dest_port = 25344

    # Create parser object
    parser = argparse.ArgumentParser(
        prog='send_udp_packets',
        description='''
        This tool can be used to send a specified number of UDP packets (num_packets) to a specified IP address (dest_ip).
        The packets are automatically sent to port 25344. 
        ''',
        epilog='Created by Jeremiah Schollar',
    )

    # Add arguments to parser and parse
    parser.add_argument('dest_ip')
    parser.add_argument('num_packets')
    args = parser.parse_args()

    # Create socket object and send packets
    udp_socket = socket.socket(
                                socket.AF_INET,    #Internet 
                                socket.SOCK_DGRAM  #UDP
                                )
    send_packets(udp_socket, args, dest_port)
