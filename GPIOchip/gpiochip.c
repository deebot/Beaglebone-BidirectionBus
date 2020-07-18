#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>      /* For platform devices */
#include <linux/of.h>                   /* For DTd d*/
#include <linux/rpmsg.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kfifo.h>
#include <linux/uaccess.h>
#include <linux/mutex.h>
#include <linux/poll.h>
#include <linux/rpmsg/virtio_rpmsg.h>
#include <linux/stat.h>
#define PRU_MAX_DEVICES				(16)
/* Matches the definition in virtio_rpmsg_bus.c */
#define RPMSG_BUF_SIZE				(512)
#define MAX_FIFO_MSG				(32)
#define FIFO_MSG_SIZE				RPMSG_BUF_SIZE
static DEFINE_MUTEX(rpmsg_pru_lock);
struct rpmsg_pru_dev {
	struct rpmsg_device *rpdev;
	struct device *dev;
	bool locked;
	dev_t devt;
	struct kfifo msg_fifo;
	u32 msg_len[MAX_FIFO_MSG];
	int msg_idx_rd;
	int msg_idx_wr;
	wait_queue_head_t wait_list;
};

#define GPIO_NUM 16
static struct gpio_chip chip;

static int mygpio_get_value(struct gpio_chip *gc, unsigned offset)
{
	printk("My Debugger isget_value\n");
	return 0;
}

static void mygpio_set_value(struct gpio_chip *gc, unsigned offset, int val)
{
	int ret;
	printk("My Debugger is set_value\n");
	struct rpmsg_device *rpdev= container_of(gc->parent, struct rpmsg_device, dev);
	ret=rpmsg_send(rpdev->ept, (void *)"345", 4);
	if (ret)
			dev_err(gc->parent, "rpmsg_send failed: %d\n", ret);

		//return ret ? ret : count;

}

static int mygpio_direction_output(struct gpio_chip *gc,
				       unsigned offset, int val)
{
	printk("My Debugger is direction_outputk\n");
	return 0;
}

static int mygpio_direction_input(struct gpio_chip *gc,
				       unsigned offset)
{
	printk("My Debugger is direction_input\n");
    return 0;
}

static const struct of_device_id fake_gpiochip_ids[] = {
    { .compatible = "packt,fake-gpio-chip", },
    { /* sentinel */ }
};

static int gpio_rpmsg_probe (struct rpmsg_device *rpdev)
{
	int ret;
	//struct rpmsg_device *rpdev;
	struct rpmsg_pru_dev *prudev;
	prudev = devm_kzalloc(&rpdev->dev, sizeof(*prudev), GFP_KERNEL);
	if (!prudev)
		return -ENOMEM;

	//mutex_lock(&rpmsg_pru_lock);
	prudev->rpdev = rpdev;

		ret = kfifo_alloc(&prudev->msg_fifo, MAX_FIFO_MSG * FIFO_MSG_SIZE,
				  GFP_KERNEL);

	init_waitqueue_head(&prudev->wait_list);
	dev_set_drvdata(&rpdev->dev, prudev);
	chip.label = rpdev->desc;
	chip.base = -1;
	chip.parent = &rpdev->dev;
	chip.owner = THIS_MODULE;
	chip.ngpio = GPIO_NUM;
	chip.can_sleep = 1;
	chip.get = mygpio_get_value;
	chip.set = mygpio_set_value;
	chip.direction_output = mygpio_direction_output;
	chip.direction_input = mygpio_direction_input;
	return gpiochip_add(&chip);
}

static int gpio_rpmsg_cb(struct rpmsg_device *rpdev, void *data, int len,
			void *priv, u32 src)
{
	u32 length;
	struct rpmsg_pru_dev *prudev;

	prudev = dev_get_drvdata(&rpdev->dev);

	if (kfifo_avail(&prudev->msg_fifo) < len) {
		dev_err(&rpdev->dev, "Not enough space on the FIFO\n");
		return -ENOSPC;
	}

	if ((prudev->msg_idx_wr + 1) % MAX_FIFO_MSG ==
		prudev->msg_idx_rd) {
		dev_err(&rpdev->dev, "Message length table is full\n");
		return -ENOSPC;
	}

	length = kfifo_in(&prudev->msg_fifo, data, len);
	prudev->msg_len[prudev->msg_idx_wr] = length;
	prudev->msg_idx_wr = (prudev->msg_idx_wr + 1) % MAX_FIFO_MSG;

	wake_up_interruptible(&prudev->wait_list);
	printk(KERN_INFO "data:%x\n ",  *((u8*)data));
	//printk(KERN_INFO "BidirectionalBus: PISO mode on %p \n",data);

	return 0;
}
static void gpio_remove(struct rpmsg_device *rpdev)
{
	struct rpmsg_pru_dev *prudev;
	prudev = dev_get_drvdata(&rpdev->dev);
	kfifo_free(&prudev->msg_fifo);
	gpiochip_remove(&chip);

}

static const struct rpmsg_device_id rpmsg_driver_pru_id_table[] = {
	{ .name	= "rpmsg-pru-gpio" },
	{ },
};
MODULE_DEVICE_TABLE(rpmsg, rpmsg_driver_pru_id_table);

static struct rpmsg_driver rpmsg_pru_driver = {
	.drv.name	= KBUILD_MODNAME,
	.id_table	= rpmsg_driver_pru_id_table,
	.probe		= gpio_rpmsg_probe,
	.callback	= gpio_rpmsg_cb,
	.remove		= gpio_remove,
};




module_rpmsg_driver(rpmsg_pru_driver);



MODULE_AUTHOR("deepankar<xxx@gmail.com>");
MODULE_LICENSE("GPL");
