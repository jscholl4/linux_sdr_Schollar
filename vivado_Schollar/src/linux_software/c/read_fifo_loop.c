//-----------------------------------------------------------
//
// Author: Jeremiah Schollar
//
// Date: 23 April 2025
//
// Description: 
//      This is Milestone 2 for the Linux SDR project.
//      
//      This script uses mmap to make 480,000 FIFO reads. 
//      
//      Note that if the enable bit of Full_Radio peripheral
//      has somehow been flipped (by writing a '1' to the LSB
//      of register 0x43c00008), no data will be sent to the 
//      FIFO and this script could in theory be stuck forever
//      because 480,000 would never be accomplished.
//      
//-----------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

#define FIFO_BASE_ADDR  0x43C10000   
#define MAP_SIZE        0x1000
#define RDFO_OFFSET     0x1C        // Receive Data FIFO Occupancy
#define RDFD_OFFSET     0x20        // Receive Data FIFO 32-bit Wide Data Read Port

int main() {
    int fd;
    void *mapped_base;
    volatile uint32_t *fifo_ptr;
    uint32_t occupancy, data;
    int read_count = 0;

    // We need to open /dev/mem with read/write permissions
    // and then map physical address to user space
    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        perror("Error: Couldn't open /dev/mem\n");
        return 1;
    }

    mapped_base = mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, FIFO_BASE_ADDR);
    if (mapped_base == MAP_FAILED) {
        perror("Error: mmap failure\n");
        close(fd);
        return 1;
    }

    // Post initial welcome message
    printf("-----------------\nMilestone 2\n23 April 2025\nJeremiah Schollar\n-----------------\n\n");
    printf("Reading 480,000 words from FIFO...\n");
    printf("(This should take about 10 seconds)\n\n");

    // Get pointer and perform reads until done
    fifo_ptr = (volatile uint32_t *) mapped_base;
    do {
        // Read contents of RDFO register to see how full FIFO is
        occupancy = *(fifo_ptr + (RDFO_OFFSET / 4));

        // Perform number of reads equal to amount of data in FIFO
        for (uint32_t i = 0; i < occupancy; i++) {
            data = *(fifo_ptr + (RDFD_OFFSET / 4));
            read_count = read_count + 1; // increment read_count variable each time a read is performed
        }
    } while (read_count < 480000);

    printf("Completed 480,000 reads!\n");

    // Clean up
    munmap((void *)mapped_base, MAP_SIZE);
    close(fd);
    return 0;
}
