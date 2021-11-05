#include <linux/slab.h>
#include <linux/ip.h>
#include <linux/inet.h>
#include <linux/netfilter.h>

#include "packet.h"
#include "../hooking/hooking.h"

voi_packet_t *voi_packet_create(struct sk_buff *skb, char *proc_name, int proc_pid, voi_hook_direction_t direction) {
    voi_packet_t *voi_pkt = NULL;
    struct iphdr *ip_h = NULL;

    voi_pkt = kzalloc(sizeof(voi_packet_t), GFP_KERNEL);
    if (!voi_pkt) {
        printk(KERN_INFO "Voi: Failed to allocate packet!\n");
        return NULL;
    }

    voi_pkt->direction = direction;

    if (!skb) {
        printk(KERN_INFO "Voi: SKB went missing!\n");
        return NULL;
    }

    if (skb->dev != NULL) {
        int len = strlen(skb->dev->name);
        if (len > 32) {
            len = 32;
        }

        snprintf(voi_pkt->dev_name, len, "%s", skb->dev->name);
    } else {
        snprintf(voi_pkt->dev_name, 7, "unknown");
    }

    ip_h = ip_hdr(skb);
    voi_pkt->protocol = ip_h->protocol;

    snprintf(voi_pkt->saddr, 16, "%pI4", &ip_h->saddr);
    snprintf(voi_pkt->daddr, 16, "%pI4", &ip_h->daddr);
    strlcpy(voi_pkt->proc_name, proc_name, 128 * sizeof(char));
    voi_pkt->proc_pid = proc_pid;

    return voi_pkt;
}

void voi_packet_free(voi_packet_t *voi_pkt) {
    kfree(voi_pkt);
}
