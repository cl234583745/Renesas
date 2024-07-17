#include "bsp_led.h"

/* LED初始化函数 */
void LED_Init(void)
{
    /* 初始化配置引脚（这里重复初始化了，可以注释掉） */
    R_IOPORT_Open (&g_ioport_ctrl, g_ioport.p_cfg);
}
