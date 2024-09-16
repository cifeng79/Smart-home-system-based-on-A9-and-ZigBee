#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/io.h>
#include <asm/uaccess.h>
#include <asm/ioctl.h>


unsigned int major = 0;
const char * modulename="pwmbeeper";
dev_t devnum = 0;
struct class  *cls = NULL;
struct device *dev = NULL;

#define GPD0CON       0x114000A0  
#define TIMER_BASE    0x139D0000             
#define TCFG0         0x0000                 
#define TCFG1         0x0004                              
#define TCON          0x0008               
#define TCNTB0        0x000C            
#define TCMPB0        0x0010
#define DAT           0x0004
#define PUD           0x0008


#define GPD0DAT 0x114000A4


static unsigned int *gpd0con;
static unsigned int *gpd0dat;
static void *timer_base;  

typedef struct beep_desc{
	int beep;    //2 3 4 5
	int beep_state;  //0 or 1
	int tcnt;  //占空比
	int tcmp;  //调节占空比
}beep_desc_t;

#define mytype 'f'
#define BEEP_ON 	_IOW(mytype,0,beep_desc_t)
#define BEEP_OFF 	_IOW(mytype,1,beep_desc_t)
#define BEEP_FREQ 	_IOW(mytype,2,beep_desc_t)


char kbuf[] = {'1','2','3','4'};


void fs4412_beep_on(void)  
{
	writel((readl(gpd0dat) & ~(0x1 << 0)) | 0x1 << 0, gpd0dat);
}

void fs4412_beep_off(void)  
{
	writel(readl(gpd0dat) & (~(0x1 << 0)), gpd0dat);
}

static void beep_freq(int beep_tcnt,int beep_tcmp)  
{  
	//tcnt决定了周期  tcnt 取值范围：
    writel (beep_tcnt, timer_base +TCNTB0  );  //计数值 100次
    //tcmp设置占空比  50 / 100 = 50%   综述：占空比 = tcmp / tcnt;  //决定声音的大小
    writel (beep_tcmp, timer_base +TCMPB0 );  //比较值 50次 
}  


int beep_open(struct inode *inode, struct file *filp)
{
	printk("--->%s--->%d.\n",__FUNCTION__,__LINE__);
	return 0;
}

int beep_release(struct inode *inode, struct file *filp)
{
	printk("--->%s--->%d.\n",__FUNCTION__,__LINE__);
	fs4412_beep_off();
	return 0;
}

ssize_t beep_read(struct file *filp, char __user *usrbuf, size_t size, loff_t *offset)
{
	int bytes = 0;
	printk("---->%s--->%d\n",__func__,__LINE__);

	bytes =	copy_to_user(usrbuf,kbuf,4);
	if(bytes > 0){
		printk("copy_to_user failed!\n");
	}
	
	return 0;
}

ssize_t beep_write (struct file *filp, const char __user *usrbuf, size_t size, loff_t *offset)
{
	int bytes = 0;
	printk("---->%s--->%d\n",__func__,__LINE__);

	bytes = copy_from_user(kbuf,usrbuf,4);
	if(bytes > 0){
		printk("copy_from_user failed\n");
		return -1;
	}
	printk("copy_from_user usrbuf:%c\n",kbuf[0]);

	return 0;
}

long beep_ioctl(struct file *filp, unsigned int cmd, unsigned long args)

{
	beep_desc_t *beeper = (beep_desc_t *)args;
	printk("---->%s--->%d\n",__func__,__LINE__);

    switch(cmd)  
    {  
        case BEEP_ON:  
            fs4412_beep_on();  
            break;  
		case BEEP_OFF:
            fs4412_beep_off();
            break;  
        case BEEP_FREQ:  
            beep_freq(beeper->tcnt,beeper->tcmp);  
            break;  
        default :  
            return -EINVAL;  
    }  
	
	return 0;
}


const struct file_operations fops={
	.open = beep_open,
	.read = beep_read,
	.write = beep_write,
	.unlocked_ioctl = beep_ioctl,
	.release = beep_release,
};

void fs4412_beep_init(void)
{
	gpd0con = ioremap(GPD0CON, 4);
	gpd0dat = ioremap(GPD0DAT, 4);

	writel((readl(gpd0con) & ~(0xF << 0)) | 0x1 << 0, gpd0con);
	
	writel((readl(gpd0dat) & ~(0x1 << 0)), gpd0dat);
}	

static int __init  beep_init(void)
{
	printk("--->%s--->%d.\n",__FUNCTION__,__LINE__);

	major = register_chrdev(0, modulename, &fops);
	if(major <= 0){
		printk("register_chrdev failed.\n");
		return -1;
	}

	devnum = MKDEV(major, 0);

	printk(KERN_INFO "major :%d.\n",major);

	cls = class_create(THIS_MODULE, "fsbeeper");
	if(cls == NULL){
		printk("class_create failed");
		return -1;
	}

	dev = device_create(cls, NULL, devnum,NULL, "fsbeeper%d",0);
	if(dev == NULL){
		printk("device_create failed.\n");
		return -1;
	}
	
	fs4412_beep_init();
	return 0;
}


void fs4412_beep_unioremap(void)
{
	iounmap(gpd0con);
	iounmap(timer_base);
}


static void __exit  beep_exit(void)
{
	printk("--->%s--->%d.\n",__FUNCTION__,__LINE__);

	fs4412_beep_unioremap();
	
	device_destroy(cls,devnum);
	class_destroy(cls);
	unregister_chrdev(major, modulename);
	
}

module_init(beep_init);
module_exit(beep_exit);
MODULE_LICENSE("GPL");
