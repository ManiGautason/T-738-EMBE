
#include <linux/init.h> 
#include <linux/module.h> 
#include <linux/kernel.h> 
#include <linux/gpio.h>                       // for the GPIO functions 
#include <linux/interrupt.h>                  // for the IRQ code

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Group 3"); 
MODULE_DESCRIPTION("A Encoder test driver for the RPi"); 
MODULE_VERSION("0.1");

static unsigned int Output_pin = 21;             
static unsigned int Encoder_pin = 20;          
static unsigned int irqNumber;                      // share IRQ num within file 
static unsigned int numberEdges = 0;                // store number of presses 
static bool         OutputState = 0;                // used to invert state of LED

// prototype for the custom IRQ handler function, function below 
static irq_handler_t  erpi_gpio_irq_handler(unsigned int irq, 
                                            void *dev_id, struct pt_regs *regs);

static int __init erpi_gpio_init(void) 
{
    int result = 0;
    printk(KERN_INFO "ENCODER_TEST: Initializing the ENCODER_TEST LKM\n");

    if (!gpio_is_valid(Output_pin)) 
    {
        printk(KERN_INFO "ENCODER_TEST: invalid OUTPUT GPIO\n");
        return -ENODEV;
    }   

    OutputState = true;

    gpio_request(Output_pin, "sysfs");                  // request OUTPUT GPIO
    gpio_direction_output(Output_pin, OutputState);     // set in output mode and on 
    // gpio_set_value(Output_pin, OutputState);         // not reqd - see line above
    gpio_export(Output_pin, false);                     // appears in /sys/class/gpio
                                                        // false prevents in/out change   
    gpio_request(Encoder_pin, "sysfs");                 // set up Encoder_pin   
    gpio_direction_input(Encoder_pin);                  // set up as input   
    //gpio_set_debounce(Encoder_pin, 200);              // debounce delay of 200ms
    gpio_export(Encoder_pin, false);                    // appears in /sys/class/gpio

    printk(KERN_INFO "ENCODER_TEST: button value is currently: %d\n", 
           gpio_get_value(Encoder_pin));

    irqNumber = gpio_to_irq(Encoder_pin);               // map GPIO to IRQ number
    printk(KERN_INFO "ENCODER_TEST: button mapped to IRQ: %d\n", irqNumber);

    // This next call requests an interrupt line   
    result = request_irq(irqNumber,                     // interrupt number requested            
        (irq_handler_t) erpi_gpio_irq_handler,          // handler function            
        IRQF_TRIGGER_RISING,                            // on rising edge (press, not release)            
        "erpi_gpio_handler",                            // used in /proc/interrupts
        NULL);                                          // *dev_id for shared interrupt lines
    printk(KERN_INFO "ENCODER_TEST: IRQ request result is: %d\n", result);
    return result;
}

static void __exit erpi_gpio_exit(void) 
{   
    printk(KERN_INFO "ENCODER_TEST: button value is currently: %d\n", 
           gpio_get_value(Encoder_pin));

    printk(KERN_INFO "ENCODER_TEST: pressed %d times\n", numberEdges);
    gpio_set_value(Output_pin, 0);                      // turn the OUTPUT off
    gpio_unexport(Output_pin);                          // unexport the OUTPUT GPIO   
    free_irq(irqNumber, NULL);                          // free the IRQ number, no *dev_id   
    gpio_unexport(Encoder_pin);                         // unexport the ENCODER GPIO   
    gpio_free(Output_pin);                              // free the OUTPUT GPIO
    gpio_free(Encoder_pin);                             // free the ENCODER GPIO
    printk(KERN_INFO "ENCODER_TEST: Goodbye from the LKM!\n"); 
}

static irq_handler_t erpi_gpio_irq_handler(unsigned int irq, 
                                           void *dev_id, struct pt_regs *regs) 
{   
    OutputState = !OutputState;                         // toggle the LED state   
    gpio_set_value(Output_pin, OutputState);            // set LED accordingly  
    printk(KERN_INFO "ENCODER_TEST: Interrupt! (button is %d)\n", 
           gpio_get_value(Encoder_pin));
    numberEdges++;                                      // global counter
    return (irq_handler_t) IRQ_HANDLED;                 // announce IRQ handled 
}

module_init(erpi_gpio_init);
module_exit(erpi_gpio_exit);


