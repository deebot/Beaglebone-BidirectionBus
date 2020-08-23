#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/gpio.h>
#include <linux/platform_device.h>
#include <linux/of.h>
#include <linux/rpmsg.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kfifo.h>
#include <linux/uaccess.h>
#include <linux/poll.h>
#include <linux/rpmsg/virtio_rpmsg.h>
#include <linux/stat.h>
#include <linux/bitops.h>
#define PRU_MAX_DEVICES				(16)
/* Matches the definition in virtio_rpmsg_bus.c */
#define RPMSG_BUF_SIZE				(512)
#define MAX_FIFO_MSG				(32)
#define FIFO_MSG_SIZE				RPMSG_BUF_SIZE
#define GPIO_NUM 9
static DEFINE_MUTEX(rpmsg_pru_lock);
static char rpmsg_pru_buf[RPMSG_BUF_SIZE];
static struct gpio_chip chip;
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
	uint32_t gpio_state;
	long input_state ;

};

struct rpmsg_pru_dev *prudev;
static int mygpio_get_value(struct gpio_chip *gc, unsigned offset){
	uint32_t value;
	struct rpmsg_device *rpdev= container_of(gc->parent, struct rpmsg_device, dev);
	unsigned int mask = BIT(offset % 8);
	prudev=dev_get_drvdata(&rpdev->dev);
	value=(prudev->input_state& mask)>>offset;
	pr_info("The value is %d and mask is %d, offset is %d,Inputstate is %ld",value,mask,offset,prudev->input_state);
	return value;
}

static void mygpio_set_value(struct gpio_chip *gc, unsigned offset, int val)
{
	int ret;
	struct rpmsg_device *rpdev= container_of(gc->parent, struct rpmsg_device, dev);
	pr_info("set_value function triggered");
	pr_info("The bit number %d is set to value: %d",offset,val);

	prudev = dev_get_drvdata(&rpdev->dev);
	if (val==0){
		prudev->gpio_state &= ~(1<<offset);
		//pr_info("A when value 0: %d",~(1<<offset));
	}
	else{
		prudev->gpio_state |= (1<<offset);
		//pr_info("A when value 1: %d",(1<<offset));
	}
	if(offset==GPIO_NUM-1){
		memcpy((void*)&rpmsg_pru_buf,(void*)&(prudev->gpio_state),sizeof(&(prudev->gpio_state)));
		pr_info("A check for checking gpio_state: %d",prudev->gpio_state);
		ret=rpmsg_send(rpdev->ept, (void *)rpmsg_pru_buf, 2);
		if (ret)
			dev_err(gc->parent, "rpmsg_send failed: %d\n", ret);
	}

}


static int mygpio_direction_output(struct gpio_chip *gc,
				       unsigned offset, int val)
{
	printk("Direction of GPIO set to: out\n");
	return 0;
}

static int mygpio_direction_input(struct gpio_chip *gc,
				       unsigned offset)
{

	printk("Direction of GPIO set to: in \n");
    return 0;
}

static const struct of_device_id fake_gpiochip_ids[] = {
    { .compatible = "packt,fake-gpio-chip", },
    { /* sentinel */ }
};

static int mygpio_rpmsg_pru_probe (struct rpmsg_device *rpdev)
{
	int ret;
	struct rpmsg_pru_dev *prudev;
	prudev = devm_kzalloc(&rpdev->dev, sizeof(*prudev), GFP_KERNEL);
	if (!prudev)
		return -ENOMEM;
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

static int mygpio_rpmsg_pru_cb(struct rpmsg_device *rpdev, void *data, int len,
			void *priv, u32 src)
{  int ret;
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
	//prudev->input_state=0b00000000;
	ret =kstrtol( (char*) data,10,&prudev->input_state);
	pr_info("The shift register port state is: %ld" ,prudev->input_state);


	return 0;
}
static void mygpio_rpmsg_pru_remove(struct rpmsg_device *rpdev)
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
	.probe		= mygpio_rpmsg_pru_probe,
	.callback	= mygpio_rpmsg_pru_cb,
	.remove		= mygpio_rpmsg_pru_remove,
};

module_rpmsg_driver(rpmsg_pru_driver);
MODULE_AUTHOR("DeepankarMaithani <deepankar19910@gmail.com>");

MODULE_DESCRIPTION("A Button/LED test driver for the Beagle");
MODULE_VERSION("0.1");
MODULE_LICENSE("GPL");;
