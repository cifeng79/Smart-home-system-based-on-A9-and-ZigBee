#include <linux/module.h>  
#include <linux/device.h>  
#include <linux/platform_device.h>  
#include <linux/interrupt.h>  
#include <linux/fs.h>  
#include <linux/wait.h>  
#include <linux/sched.h>  
#include <asm/uaccess.h>  
#include <asm/io.h>  
#include <linux/of.h>
#include <linux/device.h>

static wait_queue_head_t wq;  

unsigned int major = 0;
struct class  *cls = NULL;
struct device *dev = NULL;


static int have_data = 0;  
static int fs4412_adc;  
static struct resource *res1;  
static struct resource *res2;  
static void *fs4412_adc_base;  
  
 #define  ADCCON     0x0000  
 #define  ADCDLY     0x0008
 #define  ADCDAT     0x000C  
 #define  CLRINTADC  0x0018  
 #define  ADCMUX     0x001C


int flags = 1;
//#define flags 1 

#define DEBUG_PRINTK(msg,DEBUG_FLAG) \
    do{ \
        if(!!DEBUG_FLAG) { \
            printk("---->%s--->%d\n",__func__,__LINE__);\
            printk(msg);\
		}\
    }while(0)

/* 中断处理 */
static  irqreturn_t fs4412_adc_handler(int irqno, void *dev)  
{  
    have_data = 1;
    writel(0x12,fs4412_adc_base + CLRINTADC); //清除中断
    wake_up_interruptible(&wq);//唤醒等待队列
    return IRQ_HANDLED;
}  

static int fs4412_adc_open (struct inode *inod, struct file *filep)  
{  	
    printk("--->%s--->%d.\n",__func__,__LINE__);
  	DEBUG_PRINTK("fs4412_adc_open",flags);
    return 0;  
}  

static ssize_t fs4412_adc_read(struct file *filep, char __user *buf, size_t len, loff_t *pos)  
{  	
    printk("--->%s--->%d.\n",__func__,__LINE__);
	/* 选择ADC3号引脚 */
    writel(0x3,fs4412_adc_base + ADCMUX);  
    //设置分频系数为256、精度12位、并开启一次转换
	writel(1<<0 | 1<<14 | 0X1<<16 | 0XFF<<6 ,fs4412_adc_base + ADCCON );//转换完成，触发中断，唤醒等待队列
  
    wait_event_interruptible(wq, have_data == 1); //have_data == 1唤醒 
    
	/* 从ADCDAT寄存器读取转换结果*/
    fs4412_adc = readl(fs4412_adc_base+ADCDAT) & 0xfff;  
   
    if(copy_to_user(buf,&fs4412_adc,sizeof(int))) { 
        return -EFAULT;
	}
    have_data = 0;//转换完成，重新置0,等待下次转换
    return len;
}  


static  int fs4412_adc_release(struct inode *inode, struct file *filep)  
{  
    printk("--->%s--->%d.\n",__func__,__LINE__);
	DEBUG_PRINTK("fs4412_adc_release",flags);
    return 0;  
}  

static struct file_operations  fs4412_adc_ops ={  
    .open    = fs4412_adc_open,  
    .release = fs4412_adc_release,  
    .read    = fs4412_adc_read,  
};  
  
  
static int fs4412_adc_probe(struct platform_device *pdev)  
{  
	int ret;
  
    printk("--->%s--->%d.\n",__func__,__LINE__);
    res1 = platform_get_resource(pdev,IORESOURCE_IRQ, 0);  
    res2 = platform_get_resource(pdev,IORESOURCE_MEM, 0);   
	/* ADC虚拟地址映射 */
	fs4412_adc_base = ioremap(res2->start,res2->end - res2->start);   
	printk("res2->start,res2->end - res2->start :%#x.\n",res2->end - res2->start);
	/* 注册快速中断 */
    ret = request_irq(res1->start, fs4412_adc_handler, IRQF_DISABLED, "adc1", NULL);
	if(ret) {
		printk("request_irq failed\n");
	}
	printk("res1->start :%d.\n",res1->start);
	/* 注册字符设备 */
	major = register_chrdev(0, "adc", &fs4412_adc_ops);  
	if(major < 0){
		printk("register_chrdev failed.\n");
	}

	cls = class_create(THIS_MODULE, "fs4412_adc");
	if(cls == NULL){
		printk("class_create failed");
		goto err0;
	}

	dev = device_create(cls, NULL, MKDEV(major, 0),NULL, "fsadc%d",0);
	if(dev == NULL){
		printk("device_create failed.\n");
		return -1;
	}
	/* 初始化等待队列 */
    init_waitqueue_head(&wq);  
      
    return 0;

err0:
    printk("--->%s--->%d.\n",__func__,__LINE__);
	unregister_chrdev(major, "adc");
	return -1;
}  
static int fs4412_adc_remove(struct platform_device *pdev)  
{  
    printk("--->%s--->%d.\n",__func__,__LINE__);
    free_irq(res1->start,NULL);  
    free_irq(res2->start,NULL);    
    unregister_chrdev( major, "adc");  
    return 0;  
}  
  
static struct of_device_id myof_match_table[]=  {  
    {.compatible = "fs4412,adc" },  
};  
  
static struct platform_driver fs4412_adc_driver=  {  
    .probe = fs4412_adc_probe,  
    .remove = fs4412_adc_remove,  
    .driver ={  
        .name = "adc_demo",  
        .of_match_table = of_match_ptr(myof_match_table),  
    },  
};  
  
static int fs4412_adc_init(void)  
{  
    printk("--->%s--->%d.\n",__func__,__LINE__);
    return platform_driver_register(&fs4412_adc_driver);  
}  
static void fs4412_adc_exit(void)  
{ 
    platform_driver_unregister(&fs4412_adc_driver);  
    return;  
}

MODULE_LICENSE("GPL");  
module_init(fs4412_adc_init);  
module_exit(fs4412_adc_exit);  
