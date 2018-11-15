#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include <linux/miscdevice.h>


static int ring_buffer_size = 3;
module_param(ring_buffer_size, int, 0);


static ssize_t ring_buffer_read(
	struct file *f, char __user *ubuf, size_t usize, loff_t *off)
{
	printk("ring_buffer_read %ld\n", usize);
	return usize;
}


static ssize_t ring_buffer_write(
	struct file *f, const char __user *ubuf, size_t usize, loff_t *off)
{
	char *buf;

	buf = kzalloc(usize + 1, GFP_KERNEL);

	if (copy_from_user(buf, ubuf, usize))
		return -EFAULT;

	printk("ring_buffer_write %s\n", buf);
	return usize;
}


static long ring_buffer_ioctl(
	struct file *file, unsigned int cmd, unsigned long arg)
{
	//ring_buffer_size = arg;
	printk("ring_buffer_ioctl %u %lu\n", cmd, arg);
	return 0;
}


static const struct file_operations ring_buffer_fops = {
	.read = ring_buffer_read,
	.write = ring_buffer_write,
	.unlocked_ioctl = ring_buffer_ioctl,
	.owner = THIS_MODULE,
	.llseek = noop_llseek,
};


static struct miscdevice ring_buffer_device = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "ring_buffer",
	.fops = &ring_buffer_fops,
};


static int __init ring_init(void)
{
	printk("ring_buffer_size: %d.\n", ring_buffer_size);
	return misc_register(&ring_buffer_device);
}


static void __exit ring_exit(void)
{
	misc_deregister(&ring_buffer_device);
}


module_init(ring_init);
module_exit(ring_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Ring buffer");
MODULE_AUTHOR("Smirnov Maxim");
