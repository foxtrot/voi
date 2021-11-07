#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <linux/netlink.h>


#define NETLINK_USER 31
#define MAX_PAYLOAD 1024 /* maximum payload size*/


struct sockaddr_nl src_addr, devoi_addr;
struct nlmsghdr *nlh = NULL;
struct iovec iov;
int sock_fd;
struct msghdr msg;

void do_listen() {
    sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);
    if (sock_fd < 0)
        return;

    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();

    bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr));

    memset(&devoi_addr, 0, sizeof(devoi_addr));
    memset(&devoi_addr, 0, sizeof(devoi_addr));
    devoi_addr.nl_family = AF_NETLINK;
    devoi_addr.nl_pid = 0;
    devoi_addr.nl_groups = 0;

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    msg.msg_name = (void *)&devoi_addr;
    msg.msg_namelen = sizeof(devoi_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    /* Read message from kernel */
    while (1) {
        recvmsg(sock_fd, &msg, 0);
        printf("Received message payload: %s\n", NLMSG_DATA(nlh));
    }
    close(sock_fd);
}

void send_set_pid() {
    sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);
    if (sock_fd < 0)
        return;

    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();

    bind(sock_fd, (struct sockaddr *)&src_addr, sizeof(src_addr));

    memset(&devoi_addr, 0, sizeof(devoi_addr));
    memset(&devoi_addr, 0, sizeof(devoi_addr));
    devoi_addr.nl_family = AF_NETLINK;
    devoi_addr.nl_pid = 0;
    devoi_addr.nl_groups = 0;

    nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
    memset(nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
    nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    sprintf(NLMSG_DATA(nlh), "set_userspace_pid %d", getpid());

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    msg.msg_name = (void *)&devoi_addr;
    msg.msg_namelen = sizeof(devoi_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    printf("Sending message to kernel\n");
    sendmsg(sock_fd, &msg, 0);
    printf("Waiting for message from kernel\n");

    /* Read message from kernel */
    recvmsg(sock_fd, &msg, 0);
    printf("Received message payload: %s\n", NLMSG_DATA(nlh));
    close(sock_fd);
}

int main() {
    send_set_pid();
    do_listen();
	
    return 0;
}
