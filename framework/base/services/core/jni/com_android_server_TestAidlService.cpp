#define LOG_TAG "BreathLedsService"  

#include "jni.h"  
#include "JNIHelp.h"  
#include "android_runtime/AndroidRuntime.h"  
#include <hardware/hw_breath_leds.h>  
#include <hardware/hardware.h>  
namespace android {  

    struct breath_leds_device_t* leds_dev = NULL;  

    static void leds_ctl_open(const struct hw_module_t* module, struct breath_leds_device_t** dev)  
    {  
        module->methods->open(module, BREATH_LEDS_HW_MODULE_ID, (struct hw_device_t**) dev);  
    }  
    
    static void init_leds(JNIEnv* env, jobject thiz)  
    {  
        ALOGE("zrj init_leds!!!!!!!!!!!!");
    
        breath_leds_module_t* leds_module = NULL;  
        if (hw_get_module(BREATH_LEDS_HW_MODULE_ID, (const hw_module_t**) &leds_module) == 0)  
        {  
            ALOGE("zrj init_leds!!!!!!!!!!!! success");
            leds_ctl_open(&(leds_module->breath_module), &leds_dev);   
			
        }  
	  else
	  {
            ALOGE("zrj init_leds!!!!!!!!!!!! failed");

	  }	
    }  
    
    static void set_brightness_leds(JNIEnv* env, jobject thiz, jint level)  
    {  
        ALOGE("zrj set_brightness_leds");
        leds_dev->set_breath_value(leds_dev, level);  
    }  
    
    static const JNINativeMethod method_tab[] = {  
        {"init_native", "()V", (void*) init_leds},  
        {"set_brightness_native", "(I)V", (void*) set_brightness_leds},  
    };  
    
    int register_android_server_BreathLedsService(JNIEnv *env)  
    {  
        return AndroidRuntime::registerNativeMethods(env,   
        "android/os/BreathLedsService", method_tab, NELEM(method_tab));  
    }  

}  /*  namespace android  */  

