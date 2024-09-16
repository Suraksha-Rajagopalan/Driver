#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
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

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif


static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xbdfb6dbb, "__fentry__" },
	{ 0x8427cc7b, "_raw_spin_lock_irq" },
	{ 0x4b750f53, "_raw_spin_unlock_irq" },
	{ 0x5b8239ca, "__x86_return_thunk" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0x92997ed8, "_printk" },
	{ 0xbcab6ee6, "sscanf" },
	{ 0x3c3ff9fd, "sprintf" },
	{ 0xba8fbd64, "_raw_spin_lock" },
	{ 0xb5b54b34, "_raw_spin_unlock" },
	{ 0xf9a482f9, "msleep" },
	{ 0xb3f7646e, "kthread_should_stop" },
	{ 0x9d2ab8ac, "__tasklet_schedule" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0xfd205a2d, "cdev_init" },
	{ 0x3fd2f8f8, "cdev_add" },
	{ 0xaee657ee, "__class_create" },
	{ 0xf2d7865d, "device_create" },
	{ 0x22214634, "kernel_kobj" },
	{ 0xc05c1b64, "kobject_create_and_add" },
	{ 0x5e68e530, "sysfs_create_file_ns" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0xc6f46339, "init_timer_key" },
	{ 0x15ba50a6, "jiffies" },
	{ 0x5f540977, "kmalloc_caches" },
	{ 0xfa55b3ee, "kmem_cache_alloc_trace" },
	{ 0x2364c85a, "tasklet_init" },
	{ 0xac13d953, "kthread_create_on_node" },
	{ 0xd9b11890, "wake_up_process" },
	{ 0xc1514a3b, "free_irq" },
	{ 0x5de78793, "kobject_put" },
	{ 0x590d8343, "sysfs_remove_file_ns" },
	{ 0x645620c0, "class_destroy" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x5708ebf2, "cdev_del" },
	{ 0xf709cb89, "kthread_stop" },
	{ 0xea3c74e, "tasklet_kill" },
	{ 0x2b68bd2f, "del_timer" },
	{ 0x48931e20, "device_destroy" },
	{ 0x541a6db8, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "1558804BD7894A4E2ED9687");
