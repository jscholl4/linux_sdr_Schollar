#!/bin/bash
#---------------------------------------------
#       
#   Author: Jeremiah Schollar
#   
#   Date: 28 April 2025
#   
#   Description:
#       Script that is run when "Start Stream"
#       button is pressed on webpage for 
#       Milestone 1.
#       
#       Grabs IP address that was entered on 
#       webpage and calls send_udp_packets.py. 
#       Also grabs specified number of packets
#       to send.
#       
#       This script checks to see if UDP 
#       streaming is enabled, temporarily disables
#       it, and then resumes streaming (if
#       applicable) once all packets have been sent.
#       
#---------------------------------------------

echo "Content-type: text/html"
echo ""

# Read the QUERY_STRING environment variable
QUERY_STRING="$QUERY_STRING"

# Extract variables
IP_ADDRESS=$(echo "$QUERY_STRING" | awk -F'[=&]' '{for(i=1;i<=NF;i++) if($i=="ip") print $(i+1)}')
NUM_PACKETS=$(echo "$QUERY_STRING" | awk -F'[=&]' '{for(i=1;i<=NF;i++) if($i=="num_packets") print $(i+1)}')

# Provide defaults if not entered
if [ -z "$IP_ADDRESS" ]; then
    IP_ADDRESS="10.0.0.186"
fi

if [ -z "$NUM_PACKETS" ]; then
    NUM_PACKETS="100"
fi

# Check if streaming and manually set to 1
streaming=$(devmem 0x43c00008 w)
devmem 0x43c00008 w 1

# Print output message and send packets
echo "<br>Sending $NUM_PACKETS randomized packets to $IP_ADDRESS...<br>"
./send_udp_packets.py "$IP_ADDRESS" "$NUM_PACKETS" > /dev/null 2>&1 &

# Turn streaming back on if it was active
if [ "$streaming" == "0x00000000" ]; then
	echo "Resuming streaming"
	devmem 0x43c00008 w 0
fi

echo "<br><p><em>All Done!</em></p>"

