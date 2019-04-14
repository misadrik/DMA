#include <linux/init.h>
#include <linux/kernel.h> /* min */
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/slab.h> //kmalloc()
#include <asm/page.h>
#include "testmodule2.h"

#define HELLO_MAJOR 231

int size = 0x1000;

static int my_simple_open(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "MISaD: Device has been opened\n");
    filep->private_data = kzalloc(size, GFP_KERNEL);
    if (filep->private_data == NULL)
    {
        printk(KERN_ALERT "MISaD: Space allocte failed!\n");
        return -1;
    }

    printk(KERN_INFO "MISaD: Space allocated %lx\n", filep->private_data);
    return 0;
}

static int my_simple_mmap(struct file *filp, struct vm_area_struct *vma)
{
    printk(KERN_INFO "MISaD: Device mmap\n");
    if (remap_pfn_range(vma, vma->vm_start, virt_to_pfn(filp->private_data), vma->vm_end - vma->vm_start, vma->vm_page_prot))
    {
        printk(KERN_ALERT "MISaD: Device mmap failed\n");
        return -EAGAIN;
    }

    printk(KERN_INFO "MISaD: VIRT_ADDR %lx, PHY_ADDR %lx\n", vma->vm_start, virt_to_pfn(filp->private_data));
    printk(KERN_INFO "MISaD: Device mmap OK\n");
    return 0;
}

static int my_simple_release(struct inode *inodep, struct file *filep)
{
    //kfree(filep->private_data);
    //printk(KERN_INFO "MISaD: Memory release");
    return 0;
}

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = my_simple_open,
    .mmap = my_simple_mmap,
    .release = my_simple_release,
};

static int __init testmodule_init(void)
{
    int ret;
    ret = register_chrdev(HELLO_MAJOR, "testmodule2", &fops);
    if (ret < 0)
    {
        printk(KERN_ALERT "Can't register a module.\n");
        return ret;
    }
    printk(KERN_INFO "testmodule2 registerd.\n");
    return 0;
}

static void __exit testmodule_exit(void)
{
    unregister_chrdev(HELLO_MAJOR, "testmodule2");
    printk(KERN_INFO "testmodule2 removed.\n");
}

module_init(testmodule_init);
module_exit(testmodule_exit);
MODULE_LICENSE("GPL"); // licence