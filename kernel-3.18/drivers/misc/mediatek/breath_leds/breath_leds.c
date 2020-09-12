#include <linux/module.h>
#include <linux/init.h>

#include <linux/fs.h>  
#include <linux/cdev.h>
#include <linux/device.h>   
#include <linux/pci.h>
#include <asm/uaccess.h>   
#include <linux/kernel.h>

#include <linux/delay.h>
#include <linux/gpio.h>
#include <mt-plat/mt_gpio.h>


#define DEV_NAME "breath_leds"   
#define DEV_COUNT 1  
#define BREATH_LEDS_MAJOR 0  
#define BREATH_LEDS_MINOR 1 
#define GPIO_NUMBER 	(1 | 0x80000000)////根据硬件设计定义
static int major = BREATH_LEDS_MAJOR; 
static int minor = BREATH_LEDS_MINOR;

static dev_t dev_num; 
static struct class *leds_class = NULL;  


static int param_level = 0xff;

static unsigned char rec_data;

static ssize_t breath_leds_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos)
{
	rec_data = 0;
	
	if (copy_from_user(&rec_data, buf, 1))
	{
		return -EFAULT;
	}

        printk("zrj breath_leds_write rec_data == %d",rec_data);

	if(rec_data == 1)
	{
            mt_set_gpio_out(GPIO_NUMBER, 1);
	}
	else
	{
            mt_set_gpio_out(GPIO_NUMBER, 0);
	}
	return count;
}
static struct file_operations dev_fops =
{
    .owner = THIS_MODULE,
    .write = breath_leds_write,
};

static struct cdev leds_cdev;

static int leds_create_device(void)
{
    int ret = 0;
    int err = 0;


    cdev_init(&leds_cdev, &dev_fops);
    leds_cdev.owner = dev_fops.owner; 

    if (major > 0)   
    {
        dev_num = MKDEV(major, minor);   
        err = register_chrdev_region(dev_num, DEV_COUNT, DEV_NAME);
        if (err < 0)
        {
            printk("wming : register_chrdev_region() failed\n");
            return err;
        }
    }
    else
    {
        err = alloc_chrdev_region(&leds_cdev.dev, minor, DEV_COUNT, DEV_NAME); 
        if (err < 0)
        {
            printk("wming : alloc_chrdev_region() failed\n");
            return err;
        }
        major = MAJOR(leds_cdev.dev);  
        minor = MINOR(leds_cdev.dev);  
        dev_num = leds_cdev.dev;       
    }
    ret = cdev_add(&leds_cdev, dev_num, DEV_COUNT);
    leds_class = class_create(THIS_MODULE, DEV_NAME);
    device_create(leds_class, NULL, dev_num, NULL, DEV_NAME);
    printk("leds_create_device");
    return ret;
}
void breath_led_gpio_init(void)
{
    //mt_set_gpio_mode(GPIO_NUMBER, 0);
    //mt_set_gpio_dir(GPIO_NUMBER, 1);
    //mt_set_gpio_out(GPIO_NUMBER, 1);
}
static int __init breath_leds_init(void)
{
    int ret;
    printk("breath_leds_init");
    ret = leds_create_device();
    breath_led_gpio_init();
    return ret;
}

static void __exit breath_leds_exit(void)
{
    device_destroy(leds_class, dev_num);  
    if (leds_class)
    {
        class_destroy(leds_class);   
    }
    unregister_chrdev_region(dev_num, DEV_COUNT); 
}

module_init(breath_leds_init);
module_exit(breath_leds_exit);
module_param(param_level, int, S_IRUGO | S_IWUSR);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("wming");
MODULE_ALIAS("breath_leds");
MODULE_DESCRIPTION("breathing leds");
