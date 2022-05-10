#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/filter.h>
#include <net/ethernet.h>

int main(int argc, char *argv[]) {

    /* Part I. Initialization */
    
    int sock = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (-1==sock) { perror("socket"); exit(1); } 

    struct ifreq req = { .ifr_name = "enp0s5" };
    ioctl(sock, SIOCGIFINDEX, &req, sizeof(req)); // get 'eth0' if index

    struct sockaddr_ll addr = {    
        .sll_family = AF_PACKET,           // use packet-level 
        .sll_protocol = htons(ETH_P_ALL),  // accept only Ethernet
        .sll_ifindex = req.ifr_ifindex     // device index instead of 'address'
    };

    if (-1==bind(sock, (struct sockaddr*)&addr, sizeof(addr))) {
        perror("bind"); exit(1);
    }

    /* Attach cBPF program */

    struct sock_filter code[] = {
       #include "filter.inc"   
    /* some command values like:
       { 0x28,  0,  0, 0x0000000c },
       { 0x15,  0,  5, 0x00000800 },
              ........
    */
    };
    
    struct sock_fprog program = {
        .filter = code,                      // program code
        .len = sizeof(code)/sizeof(code[0])  // instructions count
    };

    if (0!=setsockopt(
                      sock,             // socket FD
                      SOL_SOCKET,       // the entire socket
                      SO_ATTACH_FILTER,	// 'attach filter' command
                      &program,         // .... command data
                      sizeof(program)   // .... command size
                      ))
    {
        perror("SO_ATTACH_FILTER"); exit(1);
    }

    /* Part II. Capture packets */

    for (;;) {
        char buffer[4096] = {};
        
        size_t cnt = recv(sock, buffer, sizeof(buffer), 0);
        uint32_t from_ip, to_ip;

        /* Extract addresses from IPv4 header */
        memcpy(&from_ip, buffer+26, sizeof(from_ip));
        memcpy(&to_ip, buffer+30, sizeof(to_ip));

        /* Make them human-readable */
        char from_addr[20] = {}, to_addr[20] = {};
        inet_ntop(AF_INET, &from_ip, from_addr, sizeof(from_addr));
        inet_ntop(AF_INET, &to_ip, to_addr, sizeof(to_addr));

        printf("Got communication from %s to %s\n", from_addr, to_addr);
    }
}
