#!/bin/bash
#---------------------------------------------
#       
#   Author: Jeremiah Schollar
#   
#   Date: 27 April 2025
#   
#   Description:
#       Script that is run when "Start Stream"
#       button is pressed on webpage.
#       
#       Grabs IP address that was entered on 
#       webpage and calls stream_udp_data.c
#       executable. This file is called in 
#       the background so that this script 
#       can complete cleanly. 
#       
#---------------------------------------------

echo "Content-type: text/html"
echo ""

# Read the QUERY_STRING environment variable
QUERY_STRING="$QUERY_STRING"

# Extract the IP address from it
IP_ADDRESS=$(echo "$QUERY_STRING" | sed -n 's/^.*ip=\([^&]*\).*$/\1/p')

# Provide a default IP address if none was entered
if [ -z "$IP_ADDRESS" ]; then
    IP_ADDRESS="10.0.0.186"
fi

echo "<br>Starting UDP streamer. Sending packets to $IP_ADDRESS at port 25344...<br>"
./stream_udp_data "$IP_ADDRESS" > /dev/null 2>&1 &

echo "<br><p><em>All Done!</em></p>"

