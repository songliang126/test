#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/io.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/of_device.h>
#include <linux/slab.h>
#include <linux/interrupt.h>
#include <linux/of_irq.h>
#include <linux/gfp.h>

#define IRQNAME "key_irq"
int irqno, major;
struct gpio_desc* desc;
struct class* cls;
struct device* dev;
wait_queue_head_t wq;
int condition=0;

struct global_dev {
    char* name;
    int status;
    struct cdev cdev;
};

struct global_dev* g_dev;

irqreturn_t key_irq_handle(int irq, void* dev)
{
    struct global_dev* g_dev = (struct global_dev*)dev;
    int status = g_dev->status;
    //1.设置status和led1
    //status = gpiod_get_value(desc);
    status = !status;
    //gpiod_set_value(desc,status);

    //2唤醒
    condition=1;
    wake_up_interruptible(&wq);
     
    return IRQ_HANDLED;

}

int pdrv_open(struct inode* inode, struct file* file)
{
    struct global_dev* dev = container_of(inode->i_cdev, struct global_dev, cdev);
    file->private_data = dev;
    return 0;
}

ssize_t pdrv_read(struct file*file,
     char __user*ubuf, size_t size, loff_t*offs)
{
    int ret;
    struct global_dev* dev;
    if(file->f_flags & O_NONBLOCK){
        return -EINVAL;
    }else{
        ret = wait_event_interruptible(wq,condition);
    }
    
    dev = (struct global_dev *)(file->private_data);
    ret = copy_to_user(ubuf, &dev->status, size);
   condition = 0;
     
    return size;

}

ssize_t pdrv_write(struct file*file,
     const char __user*ubuf, size_t size, loff_t*offs)
{
    int ret;
    struct global_dev* dev;
    if(file->f_flags & O_NONBLOCK){
        return -EINVAL;
    }else{
        ret = wait_event_interruptible(wq,condition);
    }
    
    dev = (struct global_dev *)(file->private_data);
    ret = copy_from_user(&dev->status, ubuf, size);
     
    return size;

}

int pdrv_close(struct inode* inode, struct file* file)
{
    return 0;
}

static void dts_parse(struct platform_device* pdev)
{
    struct device_node* devnode;
    int ret;
    int node_id;
    unsigned int irq_num;
    unsigned long long reg[2];
    struct device_node* subdevnode;
	for_each_compatible_node(devnode, NULL, "level0") {
		ret = of_property_read_u32(devnode, "node0", &node_id);
		for_each_child_of_node(devnode, subdevnode) {
			if (of_device_is_compatible(subdevnode, "level1")) {
				continue;
			}
			irq_num = irq_of_parse_and_map(subdevnode, 0);
			of_property_read_u64_array(subdevnode, "reg", reg, 2);
		}
	}
}

struct file_operations fops = {
    .open = pdrv_open,
    .read = pdrv_read,
    .write = pdrv_write,
    .release = pdrv_close,
};
int pdrv_probe(struct platform_device* pdev)
{
    int ret;
    dev_t devno;
    // 1.获取设备树中的设备信息
    irqno = platform_get_irq(pdev, 0);
    //desc = gpiod_get_from_of_node(pdev->dev.of_node, "led1", 0, GPIOD_OUT_LOW, NULL);

    // 2.注册中断
    ret = request_irq(irqno, key_irq_handle, IRQF_TRIGGER_FALLING, IRQNAME, NULL);
     
    // 3.注册字符设备驱动
    g_dev = kmalloc(sizeof(struct global_dev), GFP_KERNEL);
    // major = register_chrdev(0, IRQNAME, &fops);
    ret = alloc_chrdev_region(&devno, 0, 1, "gloaldevice");
    cdev_init(&g_dev->cdev, &fops);
    g_dev->cdev.owner = THIS_MODULE;
    if (cdev_add(&g_dev->cdev, devno, 1)) {
    	printk(KERN_ERR "cdev_init fail\n");
    }

    cls = class_create(THIS_MODULE, IRQNAME);
    dev = device_create(cls, NULL, MKDEV(major, 0), NULL, IRQNAME);
     
    //4.初始化等待队列头
    init_waitqueue_head(&wq);
    
    //5.解析dts
    dts_parse(pdev);
    
    return 0;

}
int pdrv_remove(struct platform_device* pdev)
{
    device_destroy(cls, MKDEV(major, 0));
    class_destroy(cls);
    unregister_chrdev(major, IRQNAME);
    free_irq(irqno, NULL);
    //gpiod_put(desc);
    return 0;
}
const struct of_device_id oftable[] = {
    {
        .compatible = "aaa,aaa",
    },
    { /*end*/ }
};
struct platform_driver pdrv = {
    .probe = pdrv_probe,
    .remove = pdrv_remove,
    .driver = {
        .name = "bbb", //虽然用不到，但是一定要写
        .of_match_table = oftable,
    },

};
//一键注册
module_platform_driver(pdrv);
