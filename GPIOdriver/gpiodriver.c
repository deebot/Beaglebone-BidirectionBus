
/* copyright 2020 Deepankar Maithani
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 	* Redistributions of source code must retain the above copyright
 * 	  notice, this list of conditions and the following disclaimer.
 *
 * 	* Redistributions in binary form must reproduce the above copyright
 * 	  notice, this list of conditions and the following disclaimer in the
 * 	  documentation and/or other materials provided with the
 * 	  distribution.
 *
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* The sysfs entry appears at  /sys/GPIOchipBus/BidirectionalBus1 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/gpio.h>       // Required for the GPIO functions
#include <linux/interrupt.h>  // Required for the IRQ code
#include <linux/kobject.h>    // Using kobjects for the sysfs bindings

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Deepankar Maithani");
MODULE_DESCRIPTION("A GPIO chip driver for Bidirectional Bus");
MODULE_VERSION("0.1");
static unsigned int BusNo = 1;      // A number assigned to bus
static char   gpioName[18] = "BidirectionalBus";    // null terminated default string
static int    busDataVal = 255;          // variable to send or reveive data
static bool   modeVal = 0;             // set mode here



/** @brief A callback function to output the busDataVal variable
 *  @param kobj a kernel object device that appears in the sysfs filesystem
 *  @param attr the pointer to the kobj_attribute struct
 *  @param buf the buffer to which to write the value
 *  @return return the total number of characters written to the buffer
 */
static ssize_t value_show(struct kobject *kobj,
                                  struct kobj_attribute *attr, char *buf) {
   return sprintf(buf, "%d\n", busDataVal);
}

/** @brief A callback function to read data */
static ssize_t value_send(struct kobject *kobj, struct
                    kobj_attribute *attr, const char *buf, size_t count) {
   sscanf(buf, "%du", &busDataVal);
   return count;
}



/* @brief Displays if SIPO or PISO is set  0: SIPO 1: PISO */
static ssize_t modeis_show(struct kobject *kobj,
                               struct kobj_attribute *attr, char *buf){
   return sprintf(buf, "%d\n", modeVal);
}

/** @brief sets the Mode */
static ssize_t modeis_set(struct kobject *kobj, struct kobj_attribute
                                *attr, const char *buf, size_t count){
   unsigned int temp;
   sscanf(buf, "%du", &temp);       // use temp var for correct int->bool
   modeVal = temp;
   if(modeVal) {

      printk(KERN_INFO "Bidirectional Bus: SIPO mode on\n");
   }
   else {
      printk(KERN_INFO "BidirectionalBus: PISO mode on\n");
   }
   return count;
}

/**  Use these helper macros to define the name and access levels of the
 * kobj_attributes. The kobj_attribute has an attribute attr (name and mode),
 * show and store function pointers.
 */
static struct kobj_attribute data_attr = __ATTR(busDataVal, 0664, value_show, value_send);
static struct kobj_attribute mode_attr = __ATTR(mode, 0664, modeis_show, modeis_set);



/**  The bidirecgpio_attrs[] is an array of attributes that is used to create the
 * attribute group below. The attr property of the kobj_attribute is used
 * to extract the attribute struct
 */
static struct attribute *bidirecgpio_attrs[] = {
      &data_attr.attr,
      &mode_attr.attr,
      NULL,
};

/**  The attribute group uses the attribute array and a name, which is
 * exposed on sysfs -- in this case it is gpio46, which is automatically
 * defined in the ebb_button_init() function below using the custom kernel
 * parameter that can be passed when the module is loaded.
 */
static struct attribute_group attr_group = {
      .name  = gpioName,
      .attrs = bidirecgpio_attrs,
};

static struct kobject *mybidirec_kobj;


static int __init gpiochip_init(void){
   int result = 0;
   sprintf(gpioName, "BidirectionBus%d", BusNo);

   // create the kobject sysfs entry at /sys
   mybidirec_kobj = kobject_create_and_add("GPIOchipBus", kernel_kobj->parent);
   if(!mybidirec_kobj){
      printk(KERN_ALERT "BidirectionalBus: failed to create kobject mapping\n");
      return -ENOMEM;
   }

   result = sysfs_create_group(mybidirec_kobj, &attr_group);
   if(result) {
      printk(KERN_ALERT "BidirectionalBus: failed to create sysfs group\n");
      kobject_put(mybidirec_kobj);               // clean up remove entry
      return result;
   }

   return result;
}

static void __exit gpiochip_exit(void){

   kobject_put(mybidirec_kobj);           // clean up, remove kobject sysfs entry
   printk(KERN_INFO "BidirectionalBus: Goodbye from the LKM!\n");
}

/* Initialise and exit the module*/
module_init(gpiochip_init);
module_exit(gpiochip_exit);
