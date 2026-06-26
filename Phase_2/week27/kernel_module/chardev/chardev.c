/*
 * File:    chardev.c
 * Author:  Alexander130892
 * Date:    26-6-2026
 *
 * Description:
 *   This is a Linux kernel character device driver module that
 *   implements a simple counter device; reading from the device
 *   returns the current counter value, and writing to it increments
 *   the counter.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/errno.h>

#define DRIVER_AUTHOR "Alexander Vindelinckx"
#define DRIVER_DESC   "A sample driver"
#define CHAR_DRIVER_BUFFER_SIZE 32

static int major_nr = 0;
static int counter = 0;

static int char_dev_open(struct inode * inode, struct file * file){
    return 0;
}
static ssize_t char_dev_read(struct file *file, char __user * user, size_t size, loff_t * loff){
    if (*loff > 0)
        return 0;  /* EOF */
    char buffer[CHAR_DRIVER_BUFFER_SIZE];
    int len = snprintf(buffer, CHAR_DRIVER_BUFFER_SIZE, "%d",counter);
    int ret_val = copy_to_user(user, buffer, len );
    if(ret_val > 0){
        printk(KERN_ERR "ERROR: Not all bytes send to user\n");
        return -EFAULT;
    }
    *loff+= len;
    return len;
}
static ssize_t char_dev_write(struct file * file, const char __user *user, size_t size, loff_t * loff){
    counter++;
    return size;
}
static int char_dev_release(struct inode * inode, struct file * file){
    return 0;
}
static struct file_operations char_dev_ops = {
    .owner   = THIS_MODULE,
    .open    = char_dev_open,
    .read    = char_dev_read,
    .write   = char_dev_write,
    .release = char_dev_release,
};


static int  __init chardev_init(void){
    major_nr = register_chrdev(0,          // major — pass 0 to ask the kernel to assign one dynamically
                                "mydev",  // string identifying your device in /proc/devices
                                &char_dev_ops   // pointer to your file_operations struct
                                    );
    if (major_nr < 0) {
        printk(KERN_ERR "ERROR: Char_dev failed to register\n");
        return major_nr;  /* already negative */
    }
    printk(KERN_INFO "INIT: Char_dev major=%d\n", major_nr);
    return 0;
}

static void __exit chardev_exit(void){
    unregister_chrdev(major_nr,"mydev");
    printk(KERN_INFO "EXIT: Char_dev\n");
}

module_init(chardev_init);
module_exit(chardev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);	
MODULE_DESCRIPTION(DRIVER_DESC);	