#include <hardware/hw_breath_leds.h>
#include <cutils/log.h>
#include <fcntl.h>

#include <malloc.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <math.h>

#define DEV_NAME "/dev/breath_leds" // "/sys/class/leds/breathlight/brightness"  //
#define HAL_NAME "Breath Leds HAL Stub"   
#define HAL_AUTHOR  "The Android Open Source Project"
#define LOG_TAG "hw_breath_leds"  
static int fd = 0;

static int hw_set_breath_value(struct breath_leds_device_t *dev, int val)
{
    unsigned char buf = 0;

    ALOGE("zrj hw_set_breath_value val = %d",val);	
    buf = val;    
    write(fd, &buf, 1);  

    return 0;
}
static int hw_breath_device_close(struct hw_device_t* device)
{
    struct breath_leds_device_t* dev = (struct breath_leds_device_t*) device;
    if (dev)
    {
        free(dev); 
    }

    
    //close(dev);  
    return 0;
}
static int hw_breath_leds_open(const struct hw_module_t* module, const char* name,
                    struct hw_device_t** device)
{
    struct breath_leds_device_t *dev;
    dev = (struct breath_leds_device_t *)malloc(sizeof(*dev)); 
    memset(dev, 0, sizeof(*dev));  

    ALOGE("BREATH LEDS Stub: hw_breath_leds_open.");	

    dev->breath_device.tag = HARDWARE_DEVICE_TAG; 
    dev->breath_device.version = 0;     
    dev->breath_device.module = (struct hw_module_t*) module;
    dev->breath_device.close = hw_breath_device_close; 
    dev->set_breath_value = hw_set_breath_value;
    
    *device = &(dev->breath_device); 
 
    fd = TEMP_FAILURE_RETRY(open(DEV_NAME, O_RDWR));	
    if(fd < 0) //if((fd = open(DEV_NAME, O_RDWR)) < 0)
    {
        ALOGE("BREATH LEDS Stub: open /dev/breath_leds fail.");
        
		
    }
     else
     {
        ALOGE("BREATH LEDS Stub: open /dev/breath_leds success." );
     }

    return 0;
}
static struct hw_module_methods_t breath_module_methods =
{
    open: hw_breath_leds_open
};

struct breath_leds_module_t HAL_MODULE_INFO_SYM =
{
    breath_module:
    {
        tag: HARDWARE_MODULE_TAG,  
        version_major: 1,          
        version_minor: 0,          
        id: BREATH_LEDS_HW_MODULE_ID, 
        name: HAL_NAME,
        author: HAL_AUTHOR,
        methods: &breath_module_methods,  
    }
};

