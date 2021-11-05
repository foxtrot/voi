#ifndef PACKET_H
#define PACKET_H

#include <linux/ip.h>
#include <linux/inet.h>
#include "../hooking/hooking.h"


typedef struct voi_packet {
    int proc_pid;
    int protocol;
    char dev_name[32];
    char saddr[16];
    char daddr[16];
    char proc_name[128];
    voi_hook_direction_t direction;
} voi_packet_t;


voi_packet_t *voi_packet_create(struct sk_buff *skb, char *proc_name, int proc_pid, voi_hook_direction_t direction);
void voi_packet_free(voi_packet_t *voi_pkt);

#endif /* PACKET_H */
