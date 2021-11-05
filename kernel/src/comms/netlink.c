#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>

#include "netlink.h"

int voi_netlink_us_pid = -1;

struct sock *voi_netlink_sock = NULL;
static struct netlink_kernel_cfg voi_netlink_cfg = {
    .input = voi_netlink_sock_recv,
};

void voi_netlink_sock_recv(struct sk_buff *skb) {
    struct nlmsghdr *nlh = NULL;
    char *recv_msg = "";
    int send_pid = -1;

    nlh = (struct nlmsghdr *) skb->data;
    send_pid = nlh->nlmsg_pid;

    recv_msg = (char *) NLMSG_DATA(nlh);
    printk(KERN_INFO "Voi: Receiving Netlink Message: %s\n", recv_msg);

    // TODO: Improve this ugly parsing.
    if (strstr(recv_msg, "set_userspace_pid") != NULL) {
        char *pid = strrchr(recv_msg, ' ') + 1;

        if (kstrtoint(pid, 10, &voi_netlink_us_pid) != 0) {
            printk(KERN_INFO "Voi: Couldn't set socket PID!\n");
            return;
        }

        voi_netlink_sock_send(send_pid, "success");
    } else {
        voi_netlink_sock_send(send_pid, "Unknown Message");
    }
}

int voi_netlink_sock_send(int pid, char *msg) {
    struct sk_buff *skb_out = NULL;
    struct nlmsghdr *nlh = NULL;
    int res = -1;
    int msg_size = strlen(msg);

    if (pid == -1) {
        return 0;
    }

    skb_out = nlmsg_new(msg_size, GFP_KERNEL);
    if (!skb_out) {
        printk(KERN_INFO "Voi: Failed to create SKB!\n");
        return res;
    }

    nlh = nlmsg_put(skb_out, 0, 0, NLMSG_DONE, msg_size, 0);
    NETLINK_CB(skb_out).dst_group = 0;
    strncpy(NLMSG_DATA(nlh), msg, msg_size);

    res = nlmsg_unicast(voi_netlink_sock, skb_out, pid);
    if(res < 0) {
        printk(KERN_INFO "Voi: Couldn't send Netlink Message!\n");
    }

    return res;
}

void voi_netlink_init(void) {
    printk(KERN_INFO "Voi: Netlink Init\n");

    voi_netlink_sock = netlink_kernel_create(&init_net, MAX_LINKS - 1, &voi_netlink_cfg);

    if (!voi_netlink_sock) {
        printk(KERN_INFO "Voi: Failed to create Netlink socket!\n");
        return;
    }
}

void voi_netlink_stop(void) {
    printk(KERN_INFO "Voi: Netlink Stop\n");
    netlink_kernel_release(voi_netlink_sock);
}
