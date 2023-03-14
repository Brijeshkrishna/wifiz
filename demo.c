#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_INTERFACES 10
#define MAX_LINE_LENGTH 200

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

int main() {
    
    FILE *fp;
    char line[MAX_LINE_LENGTH];
    char *token;
    int i = 0;
    struct interface_stats interfaces[MAX_INTERFACES];

    fp = fopen("/proc/net/wireless", "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    // Skip the first two lines of the file
    fgets(line, MAX_LINE_LENGTH, fp);
    fgets(line, MAX_LINE_LENGTH, fp);

    // Parse each line of the file
    while (fgets(line, MAX_LINE_LENGTH, fp) != NULL && i < MAX_INTERFACES) {
        token = strtok(line, " :\t\n");

        // Parse interface name
        strcpy(interfaces[i].name, token);

        // Parse status
        token = strtok(NULL, " :\t\n");
        if (strcmp(token, "up") == 0) {
            interfaces[i].status = 1;
        } else {
            interfaces[i].status = 0;
        }

        // Parse link quality
        token = strtok(NULL, " :\t\n");
        interfaces[i].link_quality = atoi(token);

        // Parse signal level
        token = strtok(NULL, " :\t\n");
        interfaces[i].signal_level = atoi(token);

        // Parse noise level
        token = strtok(NULL, " :\t\n");
        interfaces[i].noise_level = atoi(token);

        // Skip discarded packets
        token = strtok(NULL, " :\t\n");

        // Parse NWID
        token = strtok(NULL, " :\t\n");
        interfaces[i].nwid = atoi(token);

        // Parse Crypt
        token = strtok(NULL, " :\t\n");
        interfaces[i].crypt = atoi(token);

        // Parse Frag
        token = strtok(NULL, " :\t\n");
        interfaces[i].frag = atoi(token);

        // Parse Retry
        token = strtok(NULL, " :\t\n");
        interfaces[i].retry = atoi(token);

        // Parse Missed Beacons
        token = strtok(NULL, " :\t\n");
        interfaces[i].missed_beacons = atoi(token);

        i++;
    }

    fclose(fp);

    // Print the parsed interface data
    for (int j = 0; j < i; j++) {
        printf("Interface: %s\n", interfaces[j].name);
        printf("Status: %d\n", interfaces[j].status);
        printf("Link quality: %d\n", interfaces[j].link_quality);
        printf("Signal level: %d\n", interfaces[j].signal_level);
        printf("Noise level: %d\n", interfaces[j].noise_level);
        printf("Discarded packets: %d\n", interfaces[j].discarded_packets);
        printf("NWID: %d\n", interfaces[j].nwid);
        printf("Crypt: %d\n", interfaces[j].crypt);
        printf("Frag: %d\n", interfaces[j].frag);
        printf("Retry: %d\n", interfaces[j].retry);
        printf("Missed packet%d\n",interfaces[j].missed_beacons);
    }

    return 0;
}