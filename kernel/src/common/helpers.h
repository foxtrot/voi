#ifndef HELPERS_H
#define HELPERS_H

#include <linux/sched.h>
#include <linux/ip.h>
#include "../packet/packet.h"

static inline char *voi_protocol_label(int ip_proto) {
    switch (ip_proto) {
        case IPPROTO_IP:   return "Dummy Protocol";
        case IPPROTO_ICMP: return "ICMP";
        case IPPROTO_TCP:  return "TCP";
        case IPPROTO_UDP:  return "UDP";
        default:           return "Unknown";
    }
}

char *voi_get_process_path(struct task_struct *tsk);
char *voi_get_process_name(struct task_struct *tsk);
void voi_print_packet(voi_packet_t *voi_pkt);


#endif /* HELPERS_H */
