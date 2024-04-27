#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>

#define LED_CNT 1
#define LED_NAME "led"

struct LedDev {
    dev_t devId;
    int major;
    int minor;
    struct cdev cdev;
    struct class* class;
    struct device* device;
};

struct LedDev ledDev;

int led_open(struct inode* inode, struct file* filp)
{
    printk("open led\n");
    return 0;
}

int led_release(struct inode* inode, struct file* filp)
{
    printk("close led\n");
    return 0;
}

ssize_t led_write(struct file* filp, const char __user* buf, size_t count, loff_t* ppos)
{
    printk("operate led\n");
    return 0;
}

const struct file_operations led_fops = 
{
    .owner = THIS_MODULE,
    .open = led_open,
    .release = led_release,
    .write = led_write,
};

static int __init led_init(void)
{
    if (ledDev.major) {
        ledDev.devId = MKDEV(ledDev.major, ledDev.minor);
        register_chrdev_region(ledDev.devId, LED_CNT, LED_NAME);
    } else {
        alloc_chrdev_region(&ledDev.devId, 0, LED_CNT, LED_NAME);
        ledDev.major = MAJOR(ledDev.devId);
        ledDev.minor = MINOR(ledDev.devId);
    }

    printk("major = %d, minor = %d\n", ledDev.major, ledDev.minor);

    ledDev.cdev.owner = THIS_MODULE;
    cdev_init(&ledDev.cdev, &led_fops);
    cdev_add(&ledDev.cdev, ledDev.devId, LED_CNT);

    ledDev.class = class_create(THIS_MODULE, LED_NAME);
    if (IS_ERR(ledDev.class)) {
        return PTR_ERR(ledDev.class);
    }

    ledDev.device = device_create(ledDev.class, NULL, ledDev.devId, NULL, LED_NAME);
    if (IS_ERR(ledDev.device)) {
        return PTR_ERR(ledDev.device);
    }

    return 0;
}

static void __exit led_exit(void)
{
    cdev_del(&ledDev.cdev);
    unregister_chrdev_region(ledDev.devId, LED_CNT);

    device_destroy(ledDev.class, ledDev.devId);
    class_destroy(ledDev.class);
}

module_init(led_init);
module_exit(led_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("hanhan blog");
