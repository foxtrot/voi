#include <linux/kernel.h>
#include <linux/slab.h>
#include "filter.h"
#include "../packet/packet.h"
#include "../comms/netlink.h"

voi_filter_counters_t *voi_filter_count = NULL;

int voi_packet_filter_init(void) {
    printk(KERN_INFO "Voi: Packet Filter Init\n");

    voi_filter_count = (voi_filter_counters_t *) kzalloc(sizeof(voi_filter_counters_t), GFP_KERNEL);

    return 0;
}

int voi_packet_filter_stop(void) {
    if (!voi_filter_count) {
        return -EFAULT;
    }

    kfree(voi_filter_count);

    return 0;
}

int voi_packet_filter_count_reset(void) {
    if (!voi_filter_count) {
        return -EFAULT;
    }

    printk(KERN_INFO "Voi: Resetting packet counters\n");

    voi_filter_count->allowed = 0;
    voi_filter_count->denied = 0;

    return 0;
}

int voi_packet_filter_check(voi_packet_t *voi_pkt) {
    /**
        Filtering should compare process information with a list of allowed,
        denied, or unknown processes. If a process is allowed/denied, accept
        or drop the packet accordingly. If a process is unknown, mark it as
        such and let the userspace program prompt the user for action.
    **/
    int pass = 1; // Should be 0 when not testing.

    if (!voi_filter_count) {
        return -EFAULT;
    }

    // TODO: Stricter checking, not just strstr test cases.
    if (strstr(voi_pkt->proc_name, "chromium") != NULL) {
        pass = 1;
    }

    if (pass) {
        voi_filter_count->allowed++;
        printk(KERN_INFO "Voi: Allowing %s\n", voi_pkt->proc_name);
        voi_netlink_sock_send(voi_netlink_us_pid, "Allowed Packet");

        return VOI_PACKET_ALLOWED;
    }

    // while? sleep? wait for userspace agent to speak decision for this packet?

    voi_filter_count->denied++;
    printk(KERN_INFO "Voi: Denying %s\n", voi_pkt->proc_name);

    return VOI_PACKET_DENIED;
}
