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
	{ 0xb1ad28e0, "__gnu_mcount_nc" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xc358aaf8, "snprintf" },
	{ 0x51a910c0, "arm_copy_to_user" },
	{ 0x7682ba4e, "__copy_overflow" },
	{ 0x3ea1b6e4, "__stack_chk_fail" },
	{ 0x8f678b07, "__stack_chk_guard" },
	{ 0x74665ecb, "gpio_to_desc" },
	{ 0x987f8d1e, "gpiod_get_raw_value" },
	{ 0x92997ed8, "_printk" },
	{ 0xeba1d604, "device_destroy" },
	{ 0xc7bca38c, "class_unregister" },
	{ 0x773cd21a, "class_destroy" },
	{ 0x6bc3fbc0, "__unregister_chrdev" },
	{ 0xc1514a3b, "free_irq" },
	{ 0x8b48754a, "gpiod_unexport" },
	{ 0xfe990052, "gpio_free" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xe83ceef0, "gpiod_direction_input" },
	{ 0xee8cf108, "gpiod_export" },
	{ 0xbe7f428e, "gpiod_to_irq" },
	{ 0x92d5838e, "request_threaded_irq" },
	{ 0xa6ab768, "__register_chrdev" },
	{ 0xa8743c5b, "__class_create" },
	{ 0x8336a5f1, "device_create" },
	{ 0xcaa72ffd, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "F1269629A5C29F44AEAE430");
