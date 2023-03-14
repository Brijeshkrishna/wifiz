#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/wireless.h>
#include <iwlib.h>
#include <ifaddrs.h>

#define PROC_NET_WIRELESS	"/proc/net/wireless"
#define PROC_NET_DEV		"/proc/net/dev"

#define MAX_LINE_LENGTH 1024
#define INTERFACE_LENGTH 16

struct interface_stats {
    char name[20];
    int status;
    int link_quality;
    int signal_level;
    int noise_level;
    int discarded_packets;
    int nwid;
    int crypt;
    int frag;
    int retry;
    int missed_beacons;
};

void connectedwifi(struct interface_stats *interfaces){

    FILE *fp;
    
    char line[MAX_LINE_LENGTH];
    char *token;
    

    fp = fopen("/proc/net/wireless", "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Skip the first two lines of the file
    fgets(line, MAX_LINE_LENGTH, fp);
    fgets(line, MAX_LINE_LENGTH, fp);

    // Parse each line of the file
    if ( fgets(line, MAX_LINE_LENGTH, fp) != NULL ) {

        token = strtok(line, " :\t\n");

        // Parse interface name
        strcpy(interfaces->name, token);

        // Parse status
        token = strtok(NULL, " :\t\n");
        interfaces->status = 1;

        // Parse link quality
        token = strtok(NULL, " :\t\n");
        interfaces->link_quality = atoi(token);

        // Parse signal level
        token = strtok(NULL, " :\t\n");
        interfaces->signal_level = atoi(token);

        // Parse noise level
        token = strtok(NULL, " :\t\n");
        interfaces->noise_level = atoi(token);

        // Skip discarded packets
        token = strtok(NULL, " :\t\n");

        // Parse NWID
        token = strtok(NULL, " :\t\n");
        interfaces->nwid = atoi(token);

        // Parse Crypt
        token = strtok(NULL, " :\t\n");
        interfaces->crypt = atoi(token);

        // Parse Frag
        token = strtok(NULL, " :\t\n");
        interfaces->frag = atoi(token);

        // Parse Retry
        token = strtok(NULL, " :\t\n");
        interfaces->retry = atoi(token);

        // Parse Missed Beacons
        token = strtok(NULL, " :\t\n");
        interfaces->missed_beacons = atoi(token);

    
    }
    fclose(fp);
}


int main()
{

    // create a socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    //     struct iwreq iw;
    //     memset(&iw, 0, sizeof(iw));

    //     // set interface to wlp3s0
    //     strncpy(iw.ifr_name, "wlp3s0", IFNAMSIZ-1); // IFNAMSIZ = 16 length of interface

    // //    // sys call
    // //    struct iw_range range;
    // //
    // //    iw.u.data.pointer = (caddr_t) &range;
    // //    iw.u.data.length = sizeof(range);
    // //    iw.u.data.flags = 0;

    //     struct ifreq ifr;
    //     strncpy(ifr.ifr_name, "wlp3s0", IFNAMSIZ); // IFNAMSIZ = 16 length of interface

    //   unsigned char * buffer = malloc(IW_SCAN_MAX_DATA*10);
    // //    iw.u.essid.pointer = (char*)malloc(32+1);
    // //    iw.u.essid.length = 32;
    //     iw.u.data.pointer = buffer;
    //     iw.u.data.flags = 0;

    //     iw.u.data.length = IW_SCAN_MAX_DATA*10;
    //     // int a = iw_get_ext(sockfd,"wlp3s0",SIOCGIWSCAN,&iw);
    // // ioctl(sockfd,  SIOCGIWSCAN  , &iw)
    //     while (1)
    //     {
    //         if ( ioctl(sockfd,  SIOCGIWSCAN  , &iw) < 0) {
    //             perror("ioctl");
    //         }
    //         else{
    //             break;
    //         }
    //     }

    //     // for (int i = 0 ; i <IW_SCAN_MAX_DATA;i++){
    //     // }
    //     //     printf("%lu",  sizeof(iw.u.data.pointer));

    // //    struct iw_statistics *stats = (struct iw_statistics *) & iw.u.data;
    // //
    // //    printf("Stats:\n");
    // //    printf("\tReceived packets: %d\n", stats->qual.level);
    // //    printf("\tDropped packets: %d\n", stats->discard.retries);
    // //    printf("\tRetransmitted packets: %d\n", stats->discard.misc);
    // //
    // //    printf("Interface flags for %d:\n",  ifr.ifr_ifru.ifru_flags);

    //     if (ioctl(sockfd,SIOCGIWAP, &iw) == -1) {
    //         perror("IOCTL SIOCGIWAP Failed,error");
    //         exit(2);
    //     } else {
    //         printf("IOCTL SIOCGIWAP Successfull\n");
    //     }
    //   for (int i = 0; i < 6; i++) {
    //         unsigned char *APaddr = (unsigned char *) iw.u.ap_addr.sa_data;
    //         printf("%02x", (int) APaddr[i]);//mac[i] means 1 byte ,i.e. 8 bits.
    //         if (i != 5)
    //             printf("%c", ':');
    //          else
    //             printf("\n");
    //     }
    //     close(sockfd);

    struct ifconf ifc;
    char buf[1024*10];
    ifc.ifc_len = sizeof(buf);
    ifc.ifc_ifcu.ifcu_buf = buf;

    if (ioctl(sockfd, SIOCGIFCONF, &ifc) < 0)
    {
        perror("ioctl");
        return 1;
    }

    int num_interfaces = ifc.ifc_len / sizeof(struct ifreq);
    printf("%d",num_interfaces);

    for (int i = 0; i <= num_interfaces; i++)
    {

        struct ifreq *ifr = &ifc.ifc_req[i];

            struct iwreq wrq;
            strncpy(wrq.ifr_name, ifr->ifr_name, IFNAMSIZ - 1);
               printf("%s\n", wrq.ifr_name);
    }


    return 0;
}
