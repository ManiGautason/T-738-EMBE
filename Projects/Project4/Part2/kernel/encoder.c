#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>       // Required for file_operations
#include <linux/device.h>   // Required for class_create and device_create
#include <linux/uaccess.h>  // Required for copy_to_user
#include <linux/gpio.h>
#include <linux/interrupt.h>

#define DEVICE_MAJOR 0
#define DEVICE_NAME "encoder"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Group 3");
MODULE_DESCRIPTION("Encoder Driver for the RPi");
MODULE_VERSION("0.1");

static int majorNumber;
static unsigned int EncoderPinA = 20;
static unsigned int EncoderPinB = 21;
static unsigned int irqNumberA;
static unsigned int irqNumberB;
static int pulseCount = 0;
static bool lastStateA = false;
static bool lastStateB = false;

static struct class *encoder_class;
static struct device *encoder_device;

static irq_handler_t erpi_gpio_irq_handlerA(unsigned int irq, void *dev_id, struct pt_regs *regs);
static irq_handler_t erpi_gpio_irq_handlerB(unsigned int irq, void *dev_id, struct pt_regs *regs);

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
    irqNumberB = gpio_to_irq(EncoderPinB);

    resultA = request_irq(irqNumberA, (irq_handler_t)erpi_gpio_irq_handlerA, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "erpi_gpio_handlerA", NULL);
    resultB = request_irq(irqNumberB, (irq_handler_t)erpi_gpio_irq_handlerB, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "erpi_gpio_handlerB", NULL);

    if (resultA || resultB)
    {
        printk(KERN_INFO "ENCODER_DRIVER: IRQ request result is: %d (A) %d (B)\n", resultA, resultB);
        return -EBUSY;
    }

    printk(KERN_INFO "ENCODER_DRIVER: Encoder pins A and B initialized.\n");

    // // Create a class and device for the encoder driver
    // encoder_class = class_create(THIS_MODULE, "encoder");
    // encoder_device = device_create(encoder_class, NULL, MKDEV(0, 0), NULL, "encoder");

    // return 0;

     encoder_class = class_create(THIS_MODULE, "mydev");
    if (IS_ERR(encoder_class)) {
        printk(KERN_ALERT "Failed to create device class\n");
        return PTR_ERR(encoder_class);
    }

    encoder_device = device_create(encoder_class, NULL, MKDEV(majorNumber, 0), NULL, DEVICE_NAME);
    if (IS_ERR(encoder_device)) {
        class_destroy(encoder_class);
        printk(KERN_ALERT "Failed to create device\n");
        return PTR_ERR(encoder_device);
    }

    majorNumber = register_chrdev(DEVICE_MAJOR, DEVICE_NAME, &fops);
    if (majorNumber < 0) {
        printk(KERN_ALERT "mydev failed to register a major number\n");
        return majorNumber;
    }
    printk(KERN_INFO "mydev: Device registered correctly with major number %d\n", majorNumber);

    return 0;
}

static void __exit erpi_gpio_exit(void)
{
    device_destroy(encoder_class, MKDEV(0, 0));
    class_unregister(encoder_class);
    class_destroy(encoder_class);
    
    free_irq(irqNumberA, NULL);
    free_irq(irqNumberB, NULL);
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

    if ((lastStateA && !stateA) || (!lastStateA && stateA))
    {

        if (stateB == stateA)
        {
            pulseCount++;
        }
        else
        {
            pulseCount--;
        }
    }

    lastStateA = stateA;

    return (irq_handler_t)IRQ_HANDLED;
}

static irq_handler_t erpi_gpio_irq_handlerB(unsigned int irq, void *dev_id, struct pt_regs *regs)
{
    bool stateA = gpio_get_value(EncoderPinA);
    bool stateB = gpio_get_value(EncoderPinB);

    if ((lastStateB && !stateB) || (!lastStateB && stateB))
    {
        printk(KERN_INFO "ENCODER_DRIVER: Pulse count: %d\n", pulseCount);
        if (stateA == stateB)
        {
            pulseCount--;
        }
        else
        {
            pulseCount++;
        }
    }

    lastStateB = stateB;

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



// #include <linux/init.h>
// #include <linux/module.h>
// #include <linux/kernel.h>
// #include <linux/gpio.h>
// #include <linux/interrupt.h>

// MODULE_LICENSE("GPL");
// MODULE_AUTHOR("Group 3");
// MODULE_DESCRIPTION("Encoder Driver for the RPi");
// MODULE_VERSION("0.1");

// static unsigned int EncoderPinA = 20;
// static unsigned int EncoderPinB = 21;
// static unsigned int irqNumberA;
// static unsigned int irqNumberB;
// static int pulseCount = 0;
// static bool lastStateA = false;
// static bool lastStateB = false;

// static irq_handler_t erpi_gpio_irq_handlerA(unsigned int irq, void *dev_id, struct pt_regs *regs);
// static irq_handler_t erpi_gpio_irq_handlerB(unsigned int irq, void *dev_id, struct pt_regs *regs);

// static int __init erpi_gpio_init(void)
// {
//     int resultA, resultB;

//     printk(KERN_INFO "ENCODER_DRIVER: Initializing the ENCODER_DRIVER LKM\n");

//     if (!gpio_is_valid(EncoderPinA) || !gpio_is_valid(EncoderPinB))
//     {
//         printk(KERN_INFO "ENCODER_DRIVER: invalid Encoder GPIO\n");
//         return -ENODEV;
//     }

//     gpio_request(EncoderPinA, "sysfs");
//     gpio_request(EncoderPinB, "sysfs");
//     gpio_direction_input(EncoderPinA);
//     gpio_direction_input(EncoderPinB);
//     gpio_export(EncoderPinA, false);
//     gpio_export(EncoderPinB, false);

//     irqNumberA = gpio_to_irq(EncoderPinA);
//     irqNumberB = gpio_to_irq(EncoderPinB);

//     resultA = request_irq(irqNumberA, (irq_handler_t)erpi_gpio_irq_handlerA, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "erpi_gpio_handlerA", NULL);
//     resultB = request_irq(irqNumberB, (irq_handler_t)erpi_gpio_irq_handlerB, IRQF_TRIGGER_RISING | IRQF_TRIGGER_FALLING, "erpi_gpio_handlerB", NULL);

//     if (resultA || resultB)
//     {
//         printk(KERN_INFO "ENCODER_DRIVER: IRQ request result is: %d (A) %d (B)\n", resultA, resultB);
//         return -EBUSY;
//     }

//     printk(KERN_INFO "ENCODER_DRIVER: Encoder pins A and B initialized.\n");
//     return 0;
// }

// static void __exit erpi_gpio_exit(void)
// {
//     free_irq(irqNumberA, NULL);
//     free_irq(irqNumberB, NULL);
//     gpio_unexport(EncoderPinA);
//     gpio_unexport(EncoderPinB);
//     gpio_free(EncoderPinA);
//     gpio_free(EncoderPinB);
//     printk(KERN_INFO "ENCODER_DRIVER: Goodbye from the LKM!\n");
// }

// static irq_handler_t erpi_gpio_irq_handlerA(unsigned int irq, void *dev_id, struct pt_regs *regs)
// {
//     bool stateA = gpio_get_value(EncoderPinA);
//     bool stateB = gpio_get_value(EncoderPinB);

//     if ((lastStateA && !stateA) || (!lastStateA && stateA))
//     {
//         // Determine the direction of rotation based on the quadrature signals
//         if (stateB == stateA)
//         {
//             pulseCount++; // Clockwise rotation
//         }
//         else
//         {
//             pulseCount--; // Counterclockwise rotation
//         }
//     }

//     lastStateA = stateA;

//     return (irq_handler_t)IRQ_HANDLED;
// }

// static irq_handler_t erpi_gpio_irq_handlerB(unsigned int irq, void *dev_id, struct pt_regs *regs)
// {
//     bool stateA = gpio_get_value(EncoderPinA);
//     bool stateB = gpio_get_value(EncoderPinB);

//     if ((lastStateB && !stateB) || (!lastStateB && stateB))
//     {
//         printk(KERN_INFO "ENCODER_DRIVER: Pulse count: %d\n", pulseCount);
//         // Determine the direction of rotation based on the quadrature signals
//         if (stateA == stateB)
//         {
//             pulseCount--; // Counterclockwise rotation
//         }
//         else
//         {
//             pulseCount++; // Clockwise rotation
//         }
//     }

//     lastStateB = stateB;

//     return (irq_handler_t)IRQ_HANDLED;
// }

// module_init(erpi_gpio_init);
// module_exit(erpi_gpio_exit);
