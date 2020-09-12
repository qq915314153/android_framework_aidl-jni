#ifndef ANDROID_INCLUDE_HW_BREATH_LEDS_H
#define ANDROID_INCLUDE_HW_BREATH_LEDS_H

#include <hardware/hardware.h>

__BEGIN_DECLS

//定义模块id,jni层通过该id查找该模块
#define BREATH_LEDS_HW_MODULE_ID  "breath_leds"

//定义硬件模块结构体，HAL规定不能直接使用hw_module_t结构体，需要在该结构体外再套一层结构体，但hw_module_t结构体
//必须是该结构体的第一个成员变量数据类型，以方便两者之间的强制转换
struct breath_leds_module_t
{
    struct hw_module_t breath_module;  //表示HAL模块的相关信息
};

//硬件接口结构体，同上，该结构体第一个成员变量数据类型必须是struct hw_device_t
struct breath_leds_device_t
{
    struct hw_device_t breath_device;

    int (*set_breath_value)(struct breath_leds_device_t *dev, int val);  //对外接口
};

__END_DECLS

#endif 
