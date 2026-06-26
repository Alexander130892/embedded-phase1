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