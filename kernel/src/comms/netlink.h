#ifndef NETLINK_H
#define NETLINK_H

#include <linux/skbuff.h>

extern int voi_netlink_us_pid;

void voi_netlink_sock_recv(struct sk_buff *skb);
int voi_netlink_sock_send(int pid, char *msg);
void voi_netlink_init(void);
void voi_netlink_stop(void);

#endif /* NETLINK_H */