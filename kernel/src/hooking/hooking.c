#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/pid.h>
#include <linux/sched.h>
#include "hooking.h"
#include "../common/helpers.h"
#include "../packet/packet.h"
#include "../filter/filter.h"


static struct nf_hook_ops *inbound_hk = NULL;
static struct nf_hook_ops *outbound_hk = NULL;


unsigned int voi_hook(void *priv, struct sk_buff *skb, const struct nf_hook_state *state, voi_hook_direction_t direction) {
    voi_packet_t *voi_pkt = NULL;
    char *proc_name = NULL;
    int proc_pid = -1;
    int allow_packet = 1;

    if (!skb) {
        return NF_DROP;
    }

    proc_name = voi_get_process_name(current);
    if (strstr(proc_name, "irq/") != NULL || strstr(proc_name, "swapper/") != NULL) {
        // TODO: Investigate irq/driver/kernel traffic, but ignore for now...
        return NF_ACCEPT;
    }

    proc_pid = task_pid_nr(current);

    voi_pkt = voi_packet_create(skb, proc_name, proc_pid, direction);
    if (!voi_pkt) {
        printk(KERN_INFO "Voi: Failed to allocate voi_packet!\n");
        kfree(proc_name);

        return NF_DROP;
    }

    voi_print_packet(voi_pkt);

    allow_packet = voi_packet_filter_check(voi_pkt);

    voi_packet_free(voi_pkt);
    kfree(proc_name);

    if (allow_packet == VOI_PACKET_ALLOWED) {
        return NF_ACCEPT;
    }

    return NF_DROP;
}

unsigned int voi_in_hook(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
    return voi_hook(priv, skb, state, VOI_HOOK_INBOUND);
}

unsigned int voi_out_hook(void *priv, struct sk_buff *skb, const struct nf_hook_state *state) {
    return voi_hook(priv, skb, state, VOI_HOOK_OUTBOUND);
}

void voi_register_hooks(void) {
    inbound_hk = (struct nf_hook_ops *) kzalloc(sizeof(struct nf_hook_ops), GFP_KERNEL);
    outbound_hk = (struct nf_hook_ops *) kzalloc(sizeof(struct nf_hook_ops), GFP_KERNEL);

    inbound_hk->hook        = (nf_hookfn *) voi_in_hook;
    inbound_hk->hooknum     = NF_INET_PRE_ROUTING;
    inbound_hk->pf          = PF_INET;
    inbound_hk->priority    = NF_IP_PRI_FIRST;

    outbound_hk->hook       = (nf_hookfn *) voi_out_hook;
    outbound_hk->hooknum    = NF_INET_POST_ROUTING;
    outbound_hk->pf         = PF_INET;
    outbound_hk->priority   = NF_IP_PRI_FIRST;

    nf_register_net_hook(&init_net, inbound_hk);
    nf_register_net_hook(&init_net, outbound_hk);

    printk(KERN_INFO "Voi: Registered Hooks\n");
}

void voi_unregister_hooks(void) {
    nf_unregister_net_hook(&init_net, inbound_hk);
    nf_unregister_net_hook(&init_net, outbound_hk);

    kfree(inbound_hk);
    kfree(outbound_hk);

    printk(KERN_INFO "Voi: Unregistered Hooks\n");
}
