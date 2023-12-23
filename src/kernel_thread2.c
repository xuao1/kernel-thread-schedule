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
static struct proc_dir_entry *proc_file; 

static int pid1 = 0;
static int pid2 = 0;

module_param(pid1, int, S_IRUGO);
module_param(pid2, int, S_IRUGO);

static ssize_t proc_write(struct file *file, const char __user *buffer, size_t count, loff_t *pos) {
    int ret;
    char *buf = (char *)kmalloc(count + 1, GFP_KERNEL);
    if (!buf) return -ENOMEM;

    if (copy_from_user(buf, buffer, count)) {
        kfree(buf);
        return -EFAULT;
    }

    buf[count] = '\0';
    ret = kstrtoint(buf, 10, &loop_continue);
    kfree(buf);
    if (ret) return ret;
    return count;
}

static const struct proc_ops proc_fops = {
    .proc_write = proc_write, 
};

static int __init my_module_init(void) {
    struct pid *pid_struct1;
    struct pid *pid_struct2;
    struct task_struct *task1;
    struct task_struct *task2;

    proc_file = proc_create("loop_control", 0666, NULL, &proc_fops);
    
    printk(KERN_INFO "Xa kernel thread module loaded with pids %d and %d\n", pid1, pid2);

    pid_struct1 = find_get_pid(pid1);
    pid_struct2 = find_get_pid(pid2);
    task1 = pid_task(pid_struct1, PIDTYPE_PID);
    task2 = pid_task(pid_struct2, PIDTYPE_PID);

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
    struct pid *pid_struct1;
    struct pid *pid_struct2;

    pid_struct1 = find_get_pid(pid1);
    pid_struct2 = find_get_pid(pid2);

    if (pid_struct1) {
        printk(KERN_INFO "Sending SIGTERM to %d\n", pid1);
        kill_pid(pid_struct1, SIGTERM, 1);
        put_pid(pid_struct1); 
    }
    if (pid_struct2) {
        printk(KERN_INFO "Sending SIGTERM to %d\n", pid2);
        kill_pid(pid_struct2, SIGTERM, 1);
        put_pid(pid_struct2);
    }

    proc_remove(proc_file);
    printk(KERN_INFO "Xa kernel thread module unloaded\n");
}

module_init(my_module_init);
module_exit(my_module_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Xu Ao");
MODULE_DESCRIPTION("A simple example Linux module to schedule two processes");
MODULE_VERSION("0.1");
