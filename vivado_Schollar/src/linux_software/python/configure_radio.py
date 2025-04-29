#! /usr/bin/python3
#-----------------------------------------------------------
#
# Author: Jeremiah Schollar
#
# Date: 23 April 2025
#
# Description: 
#      This script was largely provided by Doug Wenstrand.
#      One original function was created and implemented.
#      The text printed to the webpage was left unchanged.
#      
#      It uses the cgi module to grab frequency information 
#      from the webpage submission hosted on the Zybo board.
#      
#      A function using mmap - write_to_register() - is used
#      to write the frequency values provided on the webpage
#      to the necessary registers.
#      
#-----------------------------------------------------------


import cgi
import os
import mmap


def write_to_register(addr, freq):
    '''
        Function to write values to registers. Used below to write frequency
        values submitted on webpage to respective registers.

        Fake ADC input frequencies are written to 0x43c00000.
        Tuner input frequencies are written to 0x43c00004.
    '''
    page_size = mmap.PAGESIZE
    fd = os.open("/dev/mem", os.O_RDWR | os.O_SYNC)

    # Align to page boundary
    mapped_base = addr & ~(page_size - 1)
    offset = addr - mapped_base

    mem = mmap.mmap(fd, page_size, mmap.MAP_SHARED,
                    mmap.PROT_WRITE | mmap.PROT_READ,
                    offset=mapped_base)
    

    # Convert freq to p_inc and write to register
    p_inc = freq_to_pinc(freq)
    mem[offset:offset+4] = (p_inc).to_bytes(4, byteorder='little')


    mem.close()
    os.close(fd)

def freq_to_pinc(freq):
    '''
        Function to convert an input frequency value to the respective phase
        increment value. Xilinx DDS IP accepts phase increment values as input
        to create an output sinusoid.

        An associated phase increment value for a given sinusoid is dependent on 
        the sampling frequency and number of bits used for the phase accumulator.

        The PL used for this design had the following specs:
            - Sampling Frequency: fs = 125 MHz
            - Phase Accumulator bits: 27
    '''
    p_inc = round(freq / (125e6 / 2**27))

    return p_inc


# Create instance of FieldStorage
form = cgi.FieldStorage()

# Get data from fields
adc_freq_hz  = int(form.getvalue('adc_freq_hz'))
tune_freq_hz  = int(form.getvalue('tune_freq_hz'))
streaming = form.getvalue('streaming')

# Write frequency values to registers
write_to_register(0x43c00000, adc_freq_hz)  
write_to_register(0x43c00004, tune_freq_hz)

# Send the result to the browser
print ("Content-type:text/html")
print()
print ("<html>")
print ('<head>')
print ("<title>Radio Configurator</title>")
print ('</head>')
print ('<body>')
print ("<h2>Radio Configurator</h2>")
print ("Setting up the radio now...")
print ("ADC Freq = %d, Tune Freq = %d" %(adc_freq_hz,tune_freq_hz))
if (streaming == "streaming"):
    print ("streaming is Enabled<br>")
    write_to_register(0x43c00008, 0)
else :
    print ("streaming is Disabled<br>")
    write_to_register(0x43c00008, 1)
print ('</body>')
print ('</html>')


