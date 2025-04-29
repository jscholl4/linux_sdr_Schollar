#!/bin/bash
#---------------------------------------------
#       
#   Author: Jeremiah Schollar
#   
#   Date: 28 April 2025
#   
#   Description:
#       Script that is run when "Read FIFO"
#       button is pressed on webpage.
#       
#       Ensures that the full_radio peripheral is
#		sending data to the FIFO before reading
#		to avoid any freezes.
#		
#		Returns to full_radio output state
#		after all reads have been completed.
#		
#		
#---------------------------------------------

echo "Content-type: text/html"
echo ""

# Check if streaming and manually set to 1
streaming=$(devmem 0x43c00008 w)
devmem 0x43c00008 w 0

# Print output message and send packets
echo "<br>Reading from FIFO 480,000 times...<br>"
./read_fifo_loop 

# Turn streaming back on if it was active
if [ "$streaming" == "0x00000001" ]; then
	echo "Turning streaming off..."
	devmem 0x43c00008 w 1
fi

echo "<br><p><em>All Done!</em></p>"

