/*
 * File:    hello.mod.c
 * Author:  Alexander130892
 * Date:    26-6-2026
 *
 * Description:
 *   This is an auto-generated Linux kernel module metadata file that
 *   defines the module's initialization structure, exported symbol
 *   versions, and module information like name and dependencies. It
 *   serves as the glue between the actual module code and the kernel's
 *   module loading system.
 */

#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x92997ed8, "_printk" },
	{ 0xc4290c11, "param_ops_charp" },
	{ 0x91d66ee9, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "F7D39E407AEEF2E744D5ADE");
