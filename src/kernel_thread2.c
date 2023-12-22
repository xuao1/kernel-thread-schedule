#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/delay.h>

static int pid1 = 64977;
static int pid2 = 64533;

module_param(pid1, int, S_IRUGO);
module_param(pid2, int, S_IRUGO);

static int __init my_module_init(void) {
    struct task_struct *task1;
    struct task_struct *task2;

    printk(KERN_INFO "Xa kernel thread module loaded with pids %d and %d\n", pid1, pid2);

    task1 = pid_task(find_get_pid(pid1), PIDTYPE_PID);
    task2 = pid_task(find_get_pid(pid2), PIDTYPE_PID);

    while (1) {
        if (task1 && task2) {
            printk(KERN_INFO "Sending SIGSTOP to %d and %d\n", pid1, pid2);
            send_sig_info(SIGSTOP, SEND_SIG_NOINFO, task1);
            send_sig_info(SIGSTOP, SEND_SIG_NOINFO, task2);
            msleep(5000); // Sleep for 5000ms

            printk(KERN_INFO "Sending SIGCONT to %d and %d\n", pid1, pid2);
            send_sig_info(SIGCONT, SEND_SIG_NOINFO, task1);
            send_sig_info(SIGCONT, SEND_SIG_NOINFO, task2);
            msleep(5000);
        } else {
            printk(KERN_INFO "One or both tasks not found\n");
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
