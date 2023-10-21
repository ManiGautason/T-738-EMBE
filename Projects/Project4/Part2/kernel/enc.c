#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>       
#include <linux/device.h>   
#include <linux/uaccess.h>  
#include <linux/gpio.h>
#include <linux/interrupt.h>

#define DEVICE_MAJOR 0
#define DEVICE_NAME "enc"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Group 3");
MODULE_DESCRIPTION("Encoder Driver for the RPi");
MODULE_VERSION("0.1");

static int majorNumber;
static unsigned int EncoderPinA = 20;
static unsigned int EncoderPinB = 21;
static unsigned int irqNumberA;
static int pulseCount = 0;

static struct class *encoder_class;
static struct device *encoder_device;

static irq_handler_t erpi_gpio_irq_handlerA(unsigned int irq, void *dev_id, struct pt_regs *regs);

static int encoder_open(struct inode *inode, struct file *file);
static ssize_t encoder_read(struct file *file, char __user *user_buffer, size_t count, loff_t *offset);
static int encoder_release(struct inode *inode, struct file *file);

static struct file_operations fops = {
    .owner = THIS_MODULE,
    .open = encoder_open,
    .read = encoder_read,
    .release = encoder_release,
};

static int __init erpi_gpio_init(void)
{
    int resultA, resultB;

    printk(KERN_INFO "ENCODER_DRIVER: Initializing the ENCODER_DRIVER LKM\n");

    if (!gpio_is_valid(EncoderPinA) || !gpio_is_valid(EncoderPinB))
    {
        printk(KERN_INFO "ENCODER_DRIVER: invalid Encoder GPIO\n");
        return -ENODEV;
    }

    gpio_request(EncoderPinA, "sysfs");
    gpio_request(EncoderPinB, "sysfs");
    gpio_direction_input(EncoderPinA);
    gpio_direction_input(EncoderPinB);
    gpio_export(EncoderPinA, false);
    gpio_export(EncoderPinB, false);

    irqNumberA = gpio_to_irq(EncoderPinA);
    resultA = request_irq(irqNumberA, (irq_handler_t)erpi_gpio_irq_handlerA, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "erpi_gpio_handlerA", NULL);

    if (resultA)
    {
        printk(KERN_INFO "ENCODER_DRIVER: IRQ request result is: %d (A) %d (B)\n", resultA);
        return -EBUSY;
    }

    printk(KERN_INFO "ENCODER_DRIVER: Encoder pins A and B initialized.\n");

    // Register the character device
    majorNumber = register_chrdev(DEVICE_MAJOR, DEVICE_NAME, &fops);
    if (majorNumber < 0) {
        printk(KERN_ALERT "myencoder failed to register a major number\n");
        return majorNumber;
    }
    printk(KERN_INFO "myencoder: Device registered correctly with major number %d\n", majorNumber);

    // Create a class and device for sysfs
    encoder_class = class_create(THIS_MODULE, "enc");
    if (IS_ERR(encoder_class)) {
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create device class\n");
        return PTR_ERR(encoder_class);
    }

    encoder_device = device_create(encoder_class, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(encoder_device)) {
        class_destroy(encoder_class);
        unregister_chrdev(majorNumber, DEVICE_NAME);
        printk(KERN_ALERT "Failed to create device\n");
        return PTR_ERR(encoder_device);
    }

    return 0;
}

static void __exit erpi_gpio_exit(void)
{
    device_destroy(encoder_class, MKDEV(0, 0));
    class_unregister(encoder_class);
    class_destroy(encoder_class);
    
    free_irq(irqNumberA, NULL);
    gpio_unexport(EncoderPinA);
    gpio_unexport(EncoderPinB);
    gpio_free(EncoderPinA);
    gpio_free(EncoderPinB);
    
    printk(KERN_INFO "ENCODER_DRIVER: Goodbye from the LKM!\n");
}

static irq_handler_t erpi_gpio_irq_handlerA(unsigned int irq, void *dev_id, struct pt_regs *regs)
{
    bool stateA = gpio_get_value(EncoderPinA);
    bool stateB = gpio_get_value(EncoderPinB);

    if(stateB != stateA) {
            pulseCount++;
        }
        else {
            pulseCount--;
        }

    printk(KERN_INFO "ENCODER_DRIVER: Pulse count: %d\n", pulseCount);
    return (irq_handler_t)IRQ_HANDLED;
}


static int encoder_open(struct inode *inode, struct file *file)
{
    return 0;
}

static ssize_t encoder_read(struct file *file, char __user *user_buffer, size_t count, loff_t *offset)
{
    char buffer[10];
    int len;

    len = snprintf(buffer, sizeof(buffer), "%d\n", pulseCount);
    if (copy_to_user(user_buffer, buffer, len) != 0)
    {
        return -EFAULT;
    }

    return len;
}

static int encoder_release(struct inode *inode, struct file *file)
{
    return 0;
}


module_init(erpi_gpio_init);
module_exit(erpi_gpio_exit);
