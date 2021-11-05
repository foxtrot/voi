#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

#include "comms/netlink.h"
#include "filter/filter.h"
#include "hooking/hooking.h"

MODULE_LICENSE("APACHE-2.0");
MODULE_AUTHOR("The Voi Authors");
MODULE_DESCRIPTION("Voi Kernel Module");
MODULE_VERSION("1.0");

static int __init voi_init(void) {
    printk(KERN_INFO "Voi: Init\n");

    voi_packet_filter_init();
    voi_register_hooks();
    voi_netlink_init();

    return 0;
}

static void __exit voi_exit(void) {
    voi_unregister_hooks();
    voi_netlink_stop();
    voi_packet_filter_stop();

    printk(KERN_INFO "Voi: Exit\n");
}

module_init(voi_init);
module_exit(voi_exit);
