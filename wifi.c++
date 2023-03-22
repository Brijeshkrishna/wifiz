#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <linux/wireless.h>
#include <vector>
#include <string>
#include <bitset>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

enum InterfaceFlags  {
    UP          = 1,        // Interface is up
    BROADCAST   = 2,        // Broadcast address valid
    DEBUG       = 3,        // Turn on debugging
    LOOPBACK    = 4,        // Is a loopback net
    POINTOPOINT = 5,       // Interface is point-to-point link
    NOTRAILERS  = 6,       // Avoid use of trailers
    RUNNING     = 7,       // Resources allocated
    NOARP       = 8,       // No address resolution protocol
    PROMISC     = 9,      // Receive all packets
    ALLMULTI    = 10,      // Receive all multicast packets
    MASTER      = 11,      // Master of a load balancer
    SLAVE       = 12,      // Slave of a load balancer
    MULTICAST   = 13,     // Supports multicast
    PORTSEL     = 14,     // Can set media type
    AUTOMEDIA   = 15,     // Auto media selection
    DYNAMIC     = 16,     // Addresses are lost on down
    // LOWER_UP    = 0x10000,    // Driver signals L1 up
    // DORMANT     = 0x20000,    // Driver signals dormant
    // ECHO        = 0x40000     // Echo sent packets
};
std::string getInterfaceFlagName(InterfaceFlags flag) {
    switch (flag) {
        case InterfaceFlags::UP:
            return "UP";
        case InterfaceFlags::BROADCAST:
            return "BROADCAST";
        case InterfaceFlags::DEBUG:
            return "DEBUG";
        case InterfaceFlags::LOOPBACK:
            return "LOOPBACK";
        case InterfaceFlags::POINTOPOINT:
            return "POINTOPOINT";
        case InterfaceFlags::NOTRAILERS:
            return "NOTRAILERS";
        case InterfaceFlags::RUNNING:
            return "RUNNING";
        case InterfaceFlags::NOARP:
            return "NOARP";
        case InterfaceFlags::PROMISC:
            return "PROMISC";
        case InterfaceFlags::ALLMULTI:
            return "ALLMULTI";
        case InterfaceFlags::MASTER:
            return "MASTER";
        case InterfaceFlags::SLAVE:
            return "SLAVE";
        case InterfaceFlags::MULTICAST:
            return "MULTICAST";
        case InterfaceFlags::PORTSEL:
            return "PORTSEL";
        case InterfaceFlags::AUTOMEDIA:
            return "AUTOMEDIA";
         case InterfaceFlags::DYNAMIC:
            return "DYNAMIC";
        default:
            return "UNKNOWN";
    }
}

vector<string> list_of_devices( int sock){
    
    vector<string> devices;
    ifconf ifc;
    
    char buf[1024];
    struct ifreq* list = (ifreq*)(buf);


    ifc.ifc_len = sizeof(buf);
    ifc.ifc_buf = buf;

    if ( ioctl(sock, SIOCGIFCONF, &ifc) <0) {
        perror("ioctl");
    }

    
    int count = ifc.ifc_len / sizeof(ifreq);

    for (int i = 0; i < count ; i++) {
        devices.push_back( string(list[i].ifr_name));
    }
    return devices;

}




bool check_for_wifi( const int sock ,const string &device,string &name){
    struct ifreq ifr;
    strcpy(ifr.ifr_name,device.c_str());
    if (ioctl(sock, SIOCGIWNAME, &ifr) == 0){
        name = ifr.ifr_name;
        return true;
    }
    return false;
}

vector<string> give_wifi_devices(int sock,vector<string> list ){

    string a ;
    vector<string> wifis;
    for (auto i : list ) {

        if (check_for_wifi(sock,i,a)) {
            wifis.push_back(i);
        }
    }
    return wifis;
}
string number_to_ip(unsigned int data){


    string rv;
    rv.append(to_string(data & 0xff)+".");
    rv.append(to_string((data >> 8) & 0xff)+".");
    rv.append(to_string((data >> 16) & 0xff)+".");
    rv.append(to_string((data >> 24) & 0xff));
    return rv;
}

string get_ip(int sock, string name){

    ifreq ifreq;

    strcpy(ifreq.ifr_name, name.c_str());

    if ( ioctl(sock, SIOCGIFADDR, &ifreq) <0) {
        perror("ioctl");
    }
    return number_to_ip(((sockaddr_in *)(&ifreq.ifr_addr))->sin_addr.s_addr);
    
}


vector<InterfaceFlags> get_flages(const int sock,const string name){

    struct ifreq ifr;
    strncpy(ifr.ifr_name,name.c_str(),IFNAMSIZ - 1);

    if ( ioctl(sock, SIOCGIFFLAGS, &ifr) == -1)
        perror("ioctl");

    short if_flag = ifr.ifr_flags;

    vector<InterfaceFlags> flags ;

    int i = 1;
    while (if_flag){
        if (if_flag & 1)
            flags.push_back(InterfaceFlags(i));
        i++;
        if_flag = if_flag >>  1;
    }
    return flags;

}

int main() {

    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    
    give_wifi_devices(sockfd,list_of_devices(sockfd));
    get_flages(sockfd,"lo");
    // cout << get_ip(sockfd,"wlp4s0f3u3");
    
    return 0;
}
