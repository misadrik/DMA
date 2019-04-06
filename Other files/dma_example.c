// 一个简单的使用DMA 例子

// 示例：下面是一个简单的使用DMA进行传输的驱动程序，它是一个假想的设备，只列出DMA相关的部分来说明驱动程序中如何使用DMA的。

// 函数dad_transfer是设置DMA对内存buffer的传输操作函数，它使用流式映射将buffer的虚拟地址转换到物理地址，设置好DMA控制器，然后开始传输数据。

int dad_transfer(struct dad_dev *dev, int write, void *buffer, size_t count)
{

    dma_addr_t bus_addr;
    unsigned long flags;

    /* Map the buffer for DMA */

    dev->dma_dir = (write ? PCI_DMA_TODEVICE : PCI_DMA_FROMDEVICE);

    dev->dma_size = count;

　   //流式映射，将buffer的虚拟地址转化成物理地址

    bus_addr = pci_map_single(dev->pci_dev, buffer, count, dev->dma_dir);

    dev->dma_addr = bus_addr; //DMA传送的buffer物理地址

    //将操作控制写入到DMA控制器寄存器，从而建立起设备

    writeb(dev->registers.command, DAD_CMD_DISABLEDMA);

　   //设置传输方向–读还是写

    writeb(dev->registers.command, write ? DAD_CMD_WR : DAD_CMD_RD);　

    writel(dev->registers.addr, cpu_to_le32(bus_addr));//buffer物理地址

    writel(dev->registers.len, cpu_to_le32(count)); //传输的字节数

    //开始激活DMA进行数据传输操作
    writeb(dev->registers.command, DAD_CMD_ENABLEDMA);

    return 0;
}

// 函数dad_interrupt是中断处理函数，当DMA传输完时，调用这个中断函数来取消buffer上的DMA映射，从而让内核程序可以访问这个buffer。

void dad_interrupt(int irq, void *dev_id, struct pt_regs *regs)

{

    struct dad_dev *dev = (struct dad_dev *) dev_id;

    /* Make sure it’s really our device interrupting */
    /* Unmap the DMA buffer */

    pci_unmap_single(dev->pci_dev, dev->dma_addr, dev->dma_size, dev->dma_dir);

    /* Only now is it safe to access the buffer, copy to user, etc. */

…

}

// 函数dad_open打开设备，此时应申请中断号及DMA通道。

int dad_open (struct inode *inode, struct file *filp)

{

    struct dad_device *my_device;

    // SA_INTERRUPT表示快速中断处理且不支持共享 IRQ 信号线

    if ( (error = request_irq(my_device.irq, dad_interrupt, SA_INTERRUPT, "dad", NULL)) )

        return error; /* or implement blocking open */

    if ( (error = request_dma(my_device.dma, "dad")) ) {
        
        free_irq(my_device.irq, NULL);

        return error; /* or implement blocking open */

    }

    return 0;

}

// 在与 open 相对应的 close 函数中应该释放DMA及中断号。

void dad_close (struct inode *inode, struct file *filp)

{

    struct dad_device *my_device;

    free_dma(my_device.dma);

    free_irq(my_device.irq, NULL);

    ……

}

// 函数 dad_dma_prepare 初始化DMA控制器，设置DMA控制器的寄存器的值，为 DMA 传输作准备。

int dad_dma_prepare(int channel, int mode, unsigned int buf,

unsigned int count)

{

    unsigned long flags;

    flags = claim_dma_lock();

    disable_dma(channel);

    clear_dma_ff(channel);

    set_dma_mode(channel, mode);

    set_dma_addr(channel, virt_to_bus(buf));

    set_dma_count(channel, count);

    enable_dma(channel);

    release_dma_lock(flags);

    return 0;

}

// 函数dad_dma_isdone用来检查 DMA 传输是否成功结束。

int dad_dma_isdone(int channel)

{

    int residue;

    unsigned long flags = claim_dma_lock ();

    residue = get_dma_residue(channel);

    release_dma_lock(flags);

    return (residue == 0);

}