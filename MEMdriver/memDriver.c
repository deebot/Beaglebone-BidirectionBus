#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <asm/io.h>

#define OCMCBase 0x40300000  //
#define OCMCOffset 0x2000 // used by kernel
#define OCMCsize 0xe000   //56kb is left

static void __iomem *osmcrams;

static int __init Reader__init_module(void)


{
    if ((osmcrams = ioremap(OCMCBase+OCMCOffset, OCMCsize)) == NULL)
    {
        printk(KERN_ERR "Mapping for OCMC RAM failed\n");
        return -1;
    }
    iowrite8(0xF1,osmcrams);
    pr_info( "Initmodule called and message written \n");
    pr_info("Data at 0x040302000: %#x\n", ioread8(osmcrams));
    //ioread8(osmcrams);

    return 0;

}


static void __exit Reader_cleanup_module(void)

{
	iounmap(osmcrams);
	 pr_info("Exit module called \n");


}



module_init(Reader__init_module);
module_exit(Reader_cleanup_module);

MODULE_AUTHOR("deepankar <deepankar@gmail.com>");
MODULE_DESCRIPTION("OCMC RAM Driver");
MODULE_LICENSE("GPL");

