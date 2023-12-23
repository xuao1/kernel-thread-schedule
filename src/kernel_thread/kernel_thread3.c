#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/delay.h>
#include <linux/sched/signal.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

static int loop_continue = 1;

static int pid1 = 0;

module_param(pid1, int, S_IRUGO);

static int __init my_module_init(void) {
    struct pid *pid_struct1;
    struct task_struct *task1;

    pid_struct1 = find_get_pid(pid1);
    task1 = pid_task(pid_struct1, PIDTYPE_PID);

    while (loop_continue) {
        if (task1) {
            printk(KERN_INFO "Sending SIGCONT to %d\n", pid1);
            kill_pid(pid_struct1, SIGCONT, 1);
            msleep(10000);

            printk(KERN_INFO "Sending SIGSTOP to %d\n", pid1);
            kill_pid(pid_struct1, SIGSTOP, 1);
            msleep(10000);
        } else {
            printk(KERN_INFO "The task not found\n");
            break;
        }
    }

    return 0;
}

static void __exit my_module_exit(void) {
    printk(KERN_INFO "Xa kernel thread module unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Xu Ao");
MODULE_DESCRIPTION("A simple example Linux module to schedule two processes");
MODULE_VERSION("0.1");
