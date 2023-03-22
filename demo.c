#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    struct ifreq ifr;
    std::memset(&ifr, 0, sizeof(ifr));
    std::strcpy(ifr.ifr_name, "wlp3s0");

    if (ioctl(sockfd, SIOCGIFHWADDR, &ifr) < 0) {
        std::cerr << "Error sending ioctl request" << std::endl;
        return 1;
    }

    std::cout << "Interface: " << ifr.ifr_name << std::endl;
    std::cout << "MAC Address: ";
    for (int i = 0; i < 6; i++) {
        std::cout << std::hex << (int)(unsigned char)ifr.ifr_hwaddr.sa_data[i];
        if (i < 5) {
            std::cout << ":";
        }
    }
    std::cout << std::endl;

    // Get IP address
    if (ioctl(sockfd, SIOCGIFADDR, &ifr) < 0) {
        std::cerr << "Error sending ioctl request" << std::endl;
        return 1;
    }

    std::cout << "IP Address: " << inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr) << std::endl;

    // Get netmask
    if (ioctl(sockfd, SIOCGIFNETMASK, &ifr) < 0) {
        std::cerr << "Error sending ioctl request" << std::endl;
        return 1;
    }

    std::cout << "Netmask: " << inet_ntoa(((struct sockaddr_in *)&ifr.ifr_netmask)->sin_addr) << std::endl;

    // Get broadcast address
    if (ioctl(sockfd, SIOCGIFBRDADDR, &ifr) < 0) {
        std::cerr << "Error sending ioctl request" << std::endl;
        return 1;
    }

    std::cout << "Broadcast Address: " << inet_ntoa(((struct sockaddr_in *)&ifr.ifr_broadaddr)->sin_addr) << std::endl;

    return 0;
}
