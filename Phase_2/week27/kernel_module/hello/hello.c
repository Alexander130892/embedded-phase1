/*
 * File:    hello.c
 * Author:  Alexander130892
 * Date:    26-6-2026
 *
 * Description:
 *   This is a simple Linux kernel module that prints initialization
 *   and exit messages to the kernel log, accepting an optional string
 *   parameter that defaults to "Hello there". The module demonstrates
 *   basic kernel module structure with init/exit functions and module
 *   metadata declarations.
 */

#include <linux/module.h>	/* Needed by all modules */
#include <linux/kernel.h>	/* Needed for KERN_INFO */
#include <linux/init.h>		/* Needed for the macros */

#define DRIVER_AUTHOR "Alexander Vindelinckx"
#define DRIVER_DESC   "A sample driver"

static char *string_arg = "Hello there";
module_param(string_arg, charp, 0644);
MODULE_PARM_DESC(string_arg, "A string parameter");

static int  __init my_init(void){
    printk(KERN_INFO "my module: Init\t%s\n", string_arg);
    return 0;
}

static void __exit my_exit(void){
   printk(KERN_INFO "my module: Exit\n");
}

module_init(my_init);
module_exit(my_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);	
MODULE_DESCRIPTION(DRIVER_DESC);	