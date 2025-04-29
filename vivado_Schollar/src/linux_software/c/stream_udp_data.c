//------------------------------------------------------
//  
//  Author: Jeremiah Schollar
//  
//  Date: 24 April 2025
//  
//  Description:
//      
//      Script used to read contents of FIFO data register,
//      append data into packetized structure (as defined
//      in lab handout), and send data to specified IP 
//      address at port 25344.
//      
//      The occupancy register (RDFO) is first read to 
//      ensure the FIFO contains at least 256 words. If
//      this is true, the contents are invidually read
//      and appended to data packet and then sent.
//      
//------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/mman.h>

#define FIFO_BASE_ADDR  0x43c10000
#define RDFO_OFFSET     0x1C        // Receive Data FIFO Occupancy
#define RDFD_OFFSET     0x20        // Receive Data FIFO 32-bit Wide Data Read Port
#define MAP_SIZE        4096UL
#define PACKET_SIZE     1028

int create_udp_socket(const char *ip, int port, struct sockaddr_in *dest_addr) {
    /*
        Function to configure UDP socket.

        Accepts IP address, port, and dest_addr struct as inputs. Outputs a socket
        object.
    */
    int send_socket;

    // Create socket
    send_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (send_socket < 0) {
        perror("Failed to create socket");
        return -1;
    }

    // Zero out the address structure
    memset(dest_addr, 0, sizeof(struct sockaddr_in));
    dest_addr->sin_family = AF_INET;
    dest_addr->sin_port = htons(port);
    
    // Convert IP address from text to binary form
    if (inet_pton(AF_INET, ip, &(dest_addr->sin_addr)) <= 0) {
        perror("Invalid IP address");
        close(send_socket);
        return -1;
    }

    return send_socket;
}

void* map_physical_memory(off_t phys_addr, int* fd_out) {
    // TODO: Fix this up to be more customized
    int fd;
    void* mapped_base;

    fd = open("/dev/mem", O_RDWR | O_SYNC);
    if (fd == -1) {
        perror("Error: Couldn't open /dev/mem");
        return NULL;
    }

    mapped_base = mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, phys_addr);
    if (mapped_base == MAP_FAILED) {
        perror("Error: mmap failure");
        close(fd);
        return NULL;
    }

    // Pass the file descriptor back to caller if needed
    if (fd_out) {
        *fd_out = fd;
    } else {
        // If caller doesn't want it, just close it now
        close(fd);
    }

    return mapped_base;
}

// Append an unsigned 32-bit integer (little-endian)
void append_counter(uint8_t *packet, int *offset, uint32_t value) {
    for (int i = 0; i < 4; i++) {
        packet[(*offset)++] = (value >> (8 * i)) & 0xFF;
    }
}

// Append a 32-bit signed integer as two 16-bit signed integers (little-endian)
void append_fifo_read(uint8_t *packet, int *offset, int32_t value) {
    // Extract the lower 16 bits and upper 16 bits
    int16_t lower_16 = (int16_t)(value & 0xFFFF);            // Mask lower 16 bits
    int16_t upper_16 = (int16_t)((value >> 16) & 0xFFFF);    // Mask upper 16 bits
    
    // Append the lower 16 bits (little-endian)
    packet[(*offset)++] = lower_16 & 0xFF;          // LSB
    packet[(*offset)++] = (lower_16 >> 8) & 0xFF;   // MSB

    // Append the upper 16 bits (little-endian)
    packet[(*offset)++] = upper_16 & 0xFF;          // LSB
    packet[(*offset)++] = (upper_16 >> 8) & 0xFF;   // MSB
}



int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <ip_address>\n", argv[0]);
        return 1;
    }

    // Initialize variables
    const char *ip = argv[1];
    int send_socket;
    int fd;
    struct sockaddr_in dest_addr;
    int port = 25344;
    uint8_t packet[PACKET_SIZE];
    int offset = 0;
    int counter = 0;
    int32_t data;
    int i;

    // Create UDP Socket
    send_socket = create_udp_socket(ip, port, &dest_addr);
    if (send_socket < 0) {
        return 1;
    }

    // Open /dev/mem with read/write permission and map physical address to user space
    void* mapped_base = map_physical_memory(FIFO_BASE_ADDR, &fd);
    if (!mapped_base) {
        return 1;
    }

    while (1) {
        // Reset packet and offset for each new packet
        memset(packet, 0, PACKET_SIZE);
        offset = 0;

        // Read contents of RDFO register
        volatile uint32_t *fifo_ptr = (volatile uint32_t *) mapped_base;
        uint32_t occupancy = *(fifo_ptr + (RDFO_OFFSET / 4));
            if (occupancy >= 256) {
                // Add counter to first 4 bytes
                append_counter(packet, &offset, counter);

                // Loop to add all data to packet
                for (i = 0; i <256; i++) {
                    data = *(fifo_ptr + (RDFD_OFFSET / 4));
                    append_fifo_read(packet, &offset, data);
                }

                // Send the message
                ssize_t sent_bytes = sendto(
                    send_socket,
                    packet,
                    offset,
                    0,
                    (struct sockaddr*)&dest_addr,
                    sizeof(dest_addr)
                );
                if (sent_bytes < 0) {
                    perror("Failed to send message");
                }

                // Increment counter
                counter++;
            }
        };
}