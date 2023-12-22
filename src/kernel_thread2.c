#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/delay.h>
#include <linux/sched/signal.h>

static int loop_continue = 1;

static int pid1 = 0;
static int pid2 = 0;

module_param(pid1, int, S_IRUGO);
module_param(pid2, int, S_IRUGO);

static int __init my_module_init(void) {
    struct pid *pid_struct1;
    struct pid *pid_struct2;
    struct task_struct *task1;
    struct task_struct *task2;
    struct siginfo info;

    printk(KERN_INFO "Xa kernel thread module loaded with pids %d and %d\n", pid1, pid2);

    pid_struct1 = find_get_pid(pid1);
    pid_struct2 = find_get_pid(pid2);
    task1 = pid_task(pid_struct1, PIDTYPE_PID);
    task2 = pid_task(pid_struct2, PIDTYPE_PID);

    memset(&info, 0, sizeof(struct siginfo));
    info.si_signo = SIGSTOP;

    while (loop_continue) {
        if (task1 && task2) {
            printk(KERN_INFO "Sending SIGSTOP to %d and %d\n", pid1, pid2);
            kill_pid(pid_struct1, SIGSTOP, 1);
            kill_pid(pid_struct2, SIGSTOP, 1);
            msleep(10000);

            printk(KERN_INFO "Sending SIGCONT to %d and %d\n", pid1, pid2);
            kill_pid(pid_struct1, SIGCONT, 1);
            kill_pid(pid_struct2, SIGCONT, 1);
            msleep(10000);
        } else {
            printk(KERN_INFO "One or both tasks not found\n");
            break;
        }
    }

    return 0;
}

static void __exit my_module_exit(void) {
    loop_continue = 0;
    printk(KERN_INFO "Xa kernel thread module unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Xu Ao");
MODULE_DESCRIPTION("A simple example Linux module to schedule two processes");
MODULE_VERSION("0.1");
