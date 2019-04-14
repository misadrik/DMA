#include <linux/init.h>
#include <linux/kernel.h> /* min */
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/fs.h> //file_operations
#include <linux/slab.h> //kmalloc()
#include <asm/page.h> //virt_to_pfn()
#include "testmodule2.h" //virt_to_pfn()

#define HELLO_MAJOR 231

static unsigned char* buffer;

int size = 0x1000;

// static int my_simple_open(struct inode *inodep, struct file *filep)
// {
//     printk(KERN_INFO "MISaD: Device has been opened\n");
//     filep->private_data = kzalloc(size, GFP_KERNEL);
//     if (filep->private_data == NULL)
//     {
//         printk(KERN_ALERT "MISaD: Space allocte failed!\n");
//         return -1;
//     }
//     memset(filep->private_data,'b',100);
//     printk(KERN_INFO "MISaD: Space allocated %lx\n", filep->private_data);
//     return 0;
// }
static int my_simple_open(struct inode *inodep, struct file *filep)
{
    printk(KERN_INFO "MISaD: Device has been opened\n");
    return 0;
}

static int my_simple_mmap(struct file *filp, struct vm_area_struct *vma)
{
    printk(KERN_INFO "MISaD: Device mmap\n");
    //if (remap_pfn_range(vma, vma->vm_start, virt_to_pfn(filp->private_data), vma->vm_end - vma->vm_start, vma->vm_page_prot))
    if (remap_pfn_range(vma, vma->vm_start, virt_to_pfn(buffer), vma->vm_end - vma->vm_start, vma->vm_page_prot))
    {
        printk(KERN_ALERT "MISaD: Device mmap failed\n");
        return -EAGAIN;
    }

    printk(KERN_INFO "MISaD: VIRT_ADDR %lx, PHY_ADDR %lx\n", vma->vm_start, virt_to_pfn(buffer));
    printk(KERN_INFO "MISaD: Device mmap OK\n");
    return 0;
}

static int my_simple_release(struct inode *inodep, struct file *filep)
{
    //kfree(filep->private_data);
    printk(KERN_INFO "MISaD: Memory release");
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
    ret = register_chrdev(HELLO_MAJOR, "testmodule", &fops);//old way of register a char device before 2.6 kernal
    //void cdev_init(struct cdev *cdev, struct file_operactions *fops);
    //after structure setup, tell the kernel about it with a call
    //int cdev_add(struct cdev* dev, dev_t num, unsigned int count); //<0 fail

    if (ret < 0)
    {
      printk(KERN_ALERT "MISaD: Can't register a module.\n");
      return ret;
    }
    printk(KERN_INFO "MISaD: testmodule registerd.\n");
    buffer = (unsigned char *)kzalloc(size, GFP_KERNEL);
    SetPageReserved(virt_to_page(buffer));
    memset(buffer, 'b', 100);
    printk(KERN_INFO "MISaD: Space allocated %lx\n", buffer);

    return 0;
}

static void __exit testmodule_exit(void)
{
  unregister_chrdev(HELLO_MAJOR, "testmodule");
  //remove a char device 
  //void cdev_del(struct cdev* dev);
  ClearPageReserved(virt_to_page(buffer));
  printk(KERN_INFO "MISaD: PageReserved removed.\n");
  kfree(buffer);
  printk(KERN_INFO "MISaD: PageSpace released.\n");
  printk(KERN_INFO "MISaD: testmodule removed.\n");
}

module_init(testmodule_init);
module_exit(testmodule_exit);
MODULE_LICENSE("GPL"); // licence