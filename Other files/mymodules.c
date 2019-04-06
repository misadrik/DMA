#include <linux/module.h> //引入模块相关的宏
#include <linux/int.h> //引入module_init(), module_exit()函数
#include <linux/moduleparam.h> //引入module_param()

MODULE_AUTHOR("MISaD");
MODULE_LICENSE("GPL");

static int nbr = 10;
module_param(nbr, int, S_IRUGO);

static int_int testmodule_init(void)
{
    int i;
    for(i=0; i<nbr;i++)
    {
        printk(KERN_ALLERT "Hello, World! %d/n", i)
    }
    return 0;
}

static void_exit testmodule_exit(void)
{
    printk(KERN_ALLERT "Goodbye World!")
}

module_init(testmodule_init);
module_exit(yuer_exit);