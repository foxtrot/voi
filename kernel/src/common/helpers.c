#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/fs.h>
#include <linux/dcache.h>

#include "helpers.h"
#include "../packet/packet.h"
#include "../hooking/hooking.h"

char *voi_get_process_path(struct task_struct *tsk) {
    char *p = NULL;
    char *pathname = NULL;
    struct mm_struct *mm = tsk->mm;

    if (mm) {
        down_read(&mm->mmap_lock);

        if (mm->exe_file) {
            pathname = (char *) kzalloc(2048 * sizeof(char), GFP_ATOMIC);
            if (!pathname) {
                printk(KERN_INFO "Voi: Process path allocation failed!\n");
                return NULL;
            }

            p = d_path(&mm->exe_file->f_path, pathname, 2048);
        }

        up_read(&mm->mmap_lock);
    }

    return p;
}

char *voi_get_process_name(struct task_struct *tsk) {
    char *proc_path = NULL;
    char *proc_name_tmp = NULL;
    char *proc_name = (char *) kzalloc(128 * sizeof(char), GFP_KERNEL);

    if (!proc_name) {
        printk(KERN_INFO "Voi: Process name allocation failed!\n");
        return NULL;
    }

    proc_path = voi_get_process_path(tsk);
    if (!proc_path) {
        strncpy(proc_name, tsk->comm, strlen(tsk->comm));
    } else {
        while((proc_name_tmp = strsep(&proc_path, "/")) != NULL) {
            memset(proc_name, 0, 128);
            strncpy(proc_name, proc_name_tmp, 128);
        }

        kfree(proc_path);
    }


    return proc_name;
}

void voi_print_packet(voi_packet_t *voi_pkt) {
    return;

    printk(KERN_INFO "Voi: Hooked Packet\n"
                 "         PID: %d\n"
                 "         Proc: %s\n"
                 "         Protocol: %s\n"
                 "         Src: %s\n"
                 "         Dst: %s\n"
                 "         Direction: %s\n"
                 "         Device: %s\n",
       voi_pkt->proc_pid,
       voi_pkt->proc_name,
       voi_protocol_label(voi_pkt->protocol),
       voi_pkt->saddr,
       voi_pkt->daddr,
       voi_direction_label(voi_pkt->direction),
       voi_pkt->dev_name);
}
