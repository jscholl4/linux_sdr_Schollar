#!/bin/bash
#---------------------------------------------
#       
#   Author: Jeremiah Schollar
#   
#   Date: 27 April 2025
#   
#   Description:
#       Script that is run when "Stop Stream"
#       button is pressed on webpage.
#       
#       Uses 'killall' to kill UDP Streamer
#       programmer that was started in the 
#       background.
#       
#       It is important to run this script
#       if the PL needs to be re-initialized
#       for any reason.
#       
#---------------------------------------------

echo "Content-type: text/html"
echo ""

killall stream_udp_data
echo "<br>UDP Streamer has been stopped...<br>"