**从0开始卷出一个新项目之瑞萨RZN2L** 

[toc]

# 一、RZN2L和工业以太网介绍
## 1.1 范围
- 熟悉mcu，未使用过mpu
- 熟悉cortex M3，未使用过cortex R52
- 熟悉工业现场总线和工业以太网


https://developer.arm.com/documentation/102199/0001/?lang=en

## 1.2、目的
在企业和开发者的角度，从介绍芯片、介绍工业以太网、硬件设计、软件设计、生产流程、升级固件共6个方面分享RZN2L相关经营。
**☆快速上手，少走弯路**

## 1.3、瑞萨RZN2L简介
### 1.3.1 外观与封装
![](./images/bga.png)
### 1.3.2 官方介绍
![](./images/rzn2l.png)
### 1.3.3 芯片框图
![](./images/rzn2l-block_1.png)
![](./images/rzn2l-block_2.png)
### 1.3.4 目标应用
#### 1.3.4.1 工业
![](./images/application.png)
#### 1.3.4.2 Building Automation (BA)
![](./images/BACnet.png)

## 1.4 工业以太网简介
![](./images/iiot1.png)
![](./images/rz%20iiot3.png)
![](./images/product%20line.png)
![](./images/industry%20Ethernet.png)

## 1.5 MCU与MPU的区别简介
### 1.5.1 电源和启动时序
![](./images/power%20on.png)

### 1.5.2 外挂存贮和启动模式
- Nor Flash
- Nand Flash
- SDRAM
- DDR
![](./images/ssbl.png)

### 1.5.3 内核
![](./images/M3.png)
![](./images/R52.png)
![](./images/A9.png)

- R52:[arm_cortex_r52plus_processor_trm_102199_0001_02_en.pdf](./DOC/arm_cortex_r52plus_processor_trm_102199_0001_02_en.pdf)


### 1.5.4 操作系统
- 裸跑
- RTOS
- Linux

# 二、硬件设计

## 2.1 参考资料文档
- [rzn2l-group-datasheet.pdf](./DOC/rzn2l-group-datasheet.pdf)
- [rzn2l-group-users-manual-hardware.pdf](./DOC/rzn2l-group-users-manual-hardware.pdf)
- [rzn2l-easy-download-guide.pdf](./DOC/rzn2l-easy-download-guide.pdf)
- [renesas-rzn2l-group.pdf](./DOC/renesas-rzn2l-group.pdf)
- [rzt2m-rzn2l-group-hardware-design-guide.pdf](./DOC/rzt2m-rzn2l-group-hardware-design-guide.pdf)

- **RSK:**
- [rskplus-rzn2l-v1-schematic.pdf](./DOC/Kit/RSK/rskplus-rzn2l-v1-schematic.pdf)

- **CN032:**
- [CN032-1 Industry EtherCAT Servo Controller-RZN2L_SCH.pdf](./DOC/Kit/CN032/CN032-1%20Industry%20EtherCAT%20Servo%20Controller-RZN2L_SCH.pdf)
## 2.2 硬件设计关键点
### 2.2.1 电源和复位
- 上电时序：PMIC greenpak cpld 阻容
- MDVn：3.3v
- AVCC
![](./images/power%20on.png)

![](./images/power1.png)
![](./images/power2.png)
![](./images/power6.png)
![](./images/power3.png)

![](./images/power4.png)
![](./images/power5.png)

### 2.2.2 启动模式和flash
#### 2.2.2.1 主要模式（软硬件一致）
- ram boot
- xspi boot---qspi---000
- sci（uart）boot
- usb boot
#### 2.2.2.2 硬件拨码
![](./images/boot1.png)

#### 2.2.2.3 软件设置
![](./images/flash-fsp.png)

#### 2.2.2.4 flash选项和硬件设计
- flash qspi：AT25，W25，mx25 
- 电源1.8v/3.3v，软硬件要一致
- 不同flash的寄存器要检查，iar boot是否支持
![](./images/flash-rsk.png)
![](./images/qspi.png)
![](./images/boot%20mode.png)

#### 2.2.2.5 flash保存参数功能
- 确认xspi的真正协议（spi、dspi、qspi、ospi....）
- 核对flash的寄存器地址
- 优化的xspi_qspi例程（如何验证参数100%保存成功了）
- 给出优化的例程和测试日志
![](./images/flash-xspipins.png)
![](./images/flash-addr.png)
![](./images/flash-qspidemo.png)
- 原例程ram启动；优化为xspi flash启动断电验证保存参数
- 原例程参数地址会被固件覆盖，无法连续测试；优化为固件后安全底座
- 原例程ram debug测试；优化为串口日志，方便理解整个流程，保证断电保存参数
```
Hello World
========================FLASH===========================
0x6000 0000*********************************************
0x6000 1000*************Frameware***********************
0x6000 2000*********************************************
********************************************************
********************************************************
********************************************************
0x6001 0000*************Test Addr***********************
0x6001 1000*************Test Addr***********************
********************************************************
********************************************************
********************************************************
========================================================
[QSPI_Open]:successed!
[Quad_Mode_enable]:successed!
--------------------------------------------------------
Power On Read!
[Pointer Read]:0x4000 0000 + 0x10234
12
[Pointer Read]:0x4000 0000 + 0x10000
00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 
--------------------------------------------------------
[Erase] successed!: addr:0x60010000, len:4096
[DirectTransfe WRITE] successed!: cmd:0x06
[DirectTransfe WRITE] successed!: cmd:0x02, addr:0x10234, data:0x12
[DirectTransfe READ] successed!: cmd:0xEB, addr:0x10234, data:0x12
[Verify-1] successed! line:154
--------------------------------------------------------
[Read] successed!: addr:0x6000 0000-0x2000 0000+0x10234, read_buffer:0x12
[Verify-2] successed! line:168
--------------------------------------------------------
[QSPI_Write] addr:0x6000 0000-0x2000 0000+0x10000:
00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F 10 11 12 13 14 15 16 17 18 19 1A 1B 1C 1D 1E 1F 20 21 22 23 24 25 26 27 28 29 2A 2B 2C 2D 2E 2F 30 31 32 33 34 35 36 37 38 39 3A 3B 3C 3D 3E 3F 
[QSPI_Write] successed!
[QSPI_Read] addr:0x6000 0000-0x2000 0000+0x10000:
00 01 02 03 0
```

### 2.2.3 以太网和phy
- RGMII
- MII 兼容


![](./images/phy1.png)
![](./images/phy3.png)
![](./images/phy2.png)


### 2.2.4 ECAT EEPROM
- phylink
- phyact
- ecatled
![](./images/phy3.png)
![](./images/eeprom.png)
![](./images/ecat.png)
### 2.2.5 时钟
- 25M无源晶振
- 有源晶振
- EXTCLKIN接地
![](./images/clock.png)
![](./images/clock1.png)
### 2.2.6 debug
- jtag/swd
- BSCANP接地
- MDD jtag非加密
![](./images/debug1.png)
![](./images/debug2.png)
![](./images/debug3.png)
### 2.2.7 SDRAM
- pn预留
### 2.2.8 其他通讯接口
- log uart1 p16-5
![](./images/log.png)
- RS485
- CAN
- Encoder
- spi
- xspi

- adc
- usb
- pwm

34一组 67一组 互补PWM电机控制输出接口
MTU34			
    MTIOC3B, MTIOC3D		
    MTIOC4A, MTIOC4C		
    MTIOC4B, MTIOC4D		
MTU67			
    MTIOC6B, MTIOC6D		
    MTIOC7A, MTIOC7C		
    MTIOC7B, and MTIOC7D		

### 2.2.9 未使用引脚
![](./images/unused.png)

# 三、软件设计
## 3.1 开发环境相关
- 适用范围：适合未使用过eclipse的同学
- 本章只讲e2s，不讲IAR（没有W）
### 3.1.1 IDE下载、安装和设置
- **强烈推荐e2studio 2023-10 + RZN_FSP_Packs_v1.2.0**
- e2s安装lite
- 安装简体中文，可切换英文
- RZ不支持在线安装FSP
- 编译工具添加到环境变量
- 注意安装失败提示
- release note
- e2s工作空间
- 显示隐藏的前进后退按钮
![](./images/ide.png)
![](./images/download2.png)

![](./images/install1.png)
![](./images/install2.png)
![](./images/install3.png)
![](./images/install4.png)
![](./images/install5.png)
![](./images/install6.png)
![](./images/install7.png)
![](./images/install8.png)
![](./images/install9.png)
![](./images/install10.png)
![](./images/install11.png)
![](./images/install12.png)
![](./images/shitu2.png.png)

### 3.1.2 FSP软件包安装和更新
- **再次强烈推荐e2studio 2023-10 + RZN_FSP_Packs_v1.2.0**
- 安装多个版本fsp
![](./images/download4.png)
### 3.1.3 FSP各项功能简介

![](./images/src.png)

### 3.1.4 FSP的复制与差分
- 导入其他工程stack
- pin导出csv
- 多套pins配置
- pins导入导出
- configuration.xml差分
![](./images/fsp1.png)
![](./images/fsp2.png)
![](./images/fsp3.png)
![](./images/fsp4.png)

### 3.1.5 向导新建工程和常用设置
![](./images/projectset1.png)
![](./images/projectset2.png)
### 3.1.6 导入工程
- 单个工程文件夹（文件夹或压缩包），建议勾选复制到工作空间
- 多个工厂文件夹（可能需要复制到工作空间下）
![](./images/import1.png)
### 3.1.7 debug调试和下载
#### 3.1.7.1 官方介绍
- 官网：
![](./images/debug-official%20website%20doc.png)
- datasheet：
![](./images/debug-datasheet.png)
- user manual：
![](./images/debug-usermanual.png)
- coresight：
  1.coresight---source insight
2.[arm_cortex_r52_processor_trm_100026_0104_00_en.pdf](./DOC/arm_cortex_r52_processor_trm_100026_0104_00_en.pdf)
3.https://developer.arm.com/Processors/Cortex-R52#Technical-Specifications
4.https://wiki.segger.com/DCC
![](./images/debug-coresight.png)
5.https://zhuanlan.zhihu.com/p/149519501
- 快速开始指南
[rzt2rzn2-getting-started-flexible-software-package.pdf](./DOC/rzt2rzn2-getting-started-flexible-software-package.pdf)
![](./images/debug-startguide1.png)
![](./images/debug-xspi-systeminit.png)
- debug configuration修改cpsr
![](./images/install13.png)

#### 3.1.7.2 e2studio debug变量实时监控
![](./images/debug-e2s.png)

#### 3.1.7.3 Iar debug变量实时监控
- 瑞萨嵌入式小百科[如何在IAR IDE中使用基于Cortex-R52的RZ/T&N MPU的变量实时监控](https://mp.weixin.qq.com/s/GPN6CGonTs6Cy75KHg_wPg)
- https://www.bilibili.com/video/BV1MN4y1D7WP/?spm_id_from=333.999.0.0&vd_source=21efa952a2d53fd4f04dc76ba0278b18
#### 3.1.7.4 debug经验总结
![](./images/debug-tool.png)

## 3.2 base project构建
- 目标：构建一个通用的、排除各种“小毛刺”的、TCM SRAM可同时使用的、可用于产品开发级的base project。以备后续移植日志系统、中间层、公司平台产品。这里只是分享经营，抛砖引玉。
### 3.2.1 systick定时器
 **本节之后，通过移植coremark来分析和理解RZN2L(R52)不同于cortex M3的一些要点，最终的目的是创建一个通用的base project，这样我们的一个适用于产品级的RZN2L平台就创建出来了。**

**移植cormark关键点：**
 - 时钟：只有cortex M才有systick timer
 - 迭代次数：#define ITERATIONS 20000
 - 编译器和优化等级：COMPILER_VERSION，COMPILER_FLAGS
 - printf重定向一样
 - coremark结果只用于理解芯片和对比ATCM SRAM

 ```
 //void SysTick_Handler(void)
//{
//    cnt++;
//}
 ```
 ```
 /*
 * g_timer0CB
 * rzn2l core R52 havenot systick, so init g_timer0 cnt
 */
extern volatile clock_t cnt;
void g_timer0CB(timer_callback_args_t *p_args)
{
    if (TIMER_EVENT_CYCLE_END == p_args->event)
    {
        cnt++;
    }
}
 ```

### 3.2.2 多个stack
- coremark结果失败，但未提示stack溢出
![](./images/stack1.png)
![](./images/stack2.png)
```
[16:09:06.088]收←◆2K performance run parameters for coremark.
[0]ERROR! list crc 0x7271 - should be 0xe714
[0]ERROR! matrix crc 0x7fc1 - should be 0x1fd7
[0]ERROR! state crc 0x44f1 - should be 0x8e3a
CoreMark Size    : 666
Total ticks      : 30024
Total time (secs): 30.024000
Iterations/Sec   : 666.133760
Iterations       : 20000
Compiler version : GCC10.3.1 20210824 (release)
Compiler flags   : Please put compiler flags here (e.g. -o3)
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0x7271
[0]crcmatrix     : 0x7fc1
[0]crcstate      : 0x44f1
[0]crcfinal      : 0xf1c1
Errors detected
```
```
[12:37:01.637]收←◆hello word!
date:Oct 17 2023
time:12:36:15
file:../src/hal_entry.c
func:hal_entry,line:126
hello world!
PI=3.141593
start coremain!!!

[12:37:16.656]收←◆2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 15006
Total time (secs): 15.006000
Iterations/Sec   : 1332.800213
Iterations       : 20000
Compiler version : GCC10.3.1 20210824 (release)
Compiler flags   : Please put compiler flags here (e.g. -o3)
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x382f
Correct operation validated. See README.md for run and reporting rules.
CoreMark 1.0 : 1332.800213 / GCC10.3.1 20210824 (release) Please put compiler flags here (e.g. -o3) / STACK
FSP_PRIV_CLOCK_CPU0=400000000
running!!!
```
### 3.2.3 TCM与SRAM
- 目标对比TCM与SRAM执行速度
- coremark结果处理编译器、优化等级外，主要受text data，次要stack heap
- 所以需要修改ld

### 3.2.4 链接脚本与启动模式

- 参考例程修改ld ATCM->sram mirror
- ./script/fsp_xspi0_boot.ld fsp_ram_execution.ld自动更新
- xspi0 boot流程
- startup.c复制初始化内存数据
![](./images/boot%20process.png)
![](./images/boot%20process1.png)
```
hello word!
date:Oct 17 2023
time:12:38:17
file:../src/hal_entry.c
func:hal_entry,line:126
hello world!
PI=3.141593
start coremain!!!

[12:39:30.771]收←◆2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 34391
Total time (secs): 34.391000
Iterations/Sec   : 581.547498
Iterations       : 20000
Compiler version : GCC10.3.1 20210824 (release)
Compiler flags   : Please put compiler flags here (e.g. -o3)
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x382f
Correct operation validated. See README.md for run and reporting rules.
CoreMark 1.0 : 581.547498 / GCC10.3.1 20210824 (release) Please put compiler flags here (e.g. -o3) / STACK
FSP_PRIV_CLOCK_CPU0=400000000
running!!!
```
### 3.2.5 SRAM首选和ATCM次选
- 修改ld文件和startup.c文件
```
.atcm_text  : AT (_matcm_text)
    {
        _atcm_text_start = .;
        ./src/coremark/*.o(.text*)
        _atcm_text_end = .;
    } > ATCM
    
    .atcm_data  : AT (_matcm_data)
    {
        _atcm_data_start = .;
        
        __atcm_data_start = .;
        ./src/coremark/*.o(.data*)
        __atcm_data_end = .;   
      
        __atcm_bss_start = .;
        ./src/coremark/*.o(.bss*)
        ./src/coremark/*.o(COMMON)
        __atcm_bss_end = . ;    
        
        _atcm_data_end = .;   
    } > ATCM
```
```
#if FSP_XSPI0_BOOT_SRAM_ATCM//fsp_xspi0_boot_SRAM_ATCM.ld
            "    ldr  r0, =_matcm_text                      \n"
            "    ldr  r1, =_atcm_text_start                 \n"
            "    ldr  r2, =_atcm_text_end                   \n"
            "    cmp  r2, r1                           \n"
            "    beq  copy_ATCM_APP_TEXT_end                \n"

            "copy_to_ATCM_APP_TEXT:                         \n"
            "    ldrb  r3, [r0], #0                    \n"
            "    strb  r3, [r1], #0                    \n"
            "    add   r0, r0, #1                      \n"
            "    add   r1, r1, #1                      \n"
            "    cmp   r2, r1                          \n"
            "    bne   copy_to_ATCM_APP_TEXT                \n"
            "copy_ATCM_APP_TEXT_end:                        \n"
            "    dsb                                   \n" /* Ensuring data-changing */

            "    ldr  r0, =_matcm_data                      \n"
            "    ldr  r1, =_atcm_data_start                 \n"
            "    ldr  r2, =_atcm_data_end                   \n"
            "    cmp  r2, r1                           \n"
            "    beq  copy_ATCM_APP_DATA_end                \n"

            "copy_to_ATCM_APP_DATA:                         \n"
            "    ldrb  r3, [r0], #0                    \n"
            "    strb  r3, [r1], #0                    \n"
            "    add   r0, r0, #1                      \n"
            "    add   r1, r1, #1                      \n"
            "    cmp   r2, r1                          \n"
            "    bne   copy_to_ATCM_APP_DATA                \n"
            "copy_ATCM_APP_DATA_end:                        \n"
            "    dsb                                   \n" /* Ensuring data-changing */
#endif
```
```
hello word!
date:Oct 17 2023
time:12:40:42
file:../src/hal_entry.c
func:hal_entry,line:126
hello world!
PI=3.141593
start coremain!!!

[12:41:37.160]收←◆2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 14924
Total time (secs): 14.924000
Iterations/Sec   : 1340.123291
Iterations       : 20000
Compiler version : GCC10.3.1 20210824 (release)
Compiler flags   : Please put compiler flags here (e.g. -o3)
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x382f
Correct operation validated. See README.md for run and reporting rules.
CoreMark 1.0 : 1340.123291 / GCC10.3.1 20210824 (release) Please put compiler flags here (e.g. -o3) / STACK
FSP_PRIV_CLOCK_CPU0=400000000
running!!!
```
### 3.2.6 串口5和DMA
- 串口5不支持dma功能

![](./images/safety1.png)
![](./images/safety2.png)

### 3.2.7 bus master
![](./images/bus1.png)
```
/* region 0 (ATCM) */
#define EL1_MPU_REGION00_BASE_L     (0x0000 & 0xFFC0) + OUTER_SHAREABLE + EL1RW_EL0RW + EXECUTE_ENABLE
#define EL1_MPU_REGION00_BASE_H     (0x0000 & 0xFFFF)
#define EL1_MPU_REGION00_LIMIT_L    (0xFFFF & 0xFFC0) + ATTRINDEX3 + REGION_ENABLE
#define EL1_MPU_REGION00_LIMIT_H    (0x0001 & 0xFFFF)

/* region 1 (BTCM) */
#define EL1_MPU_REGION01_BASE_L     (0x0000 & 0xFFC0) + OUTER_SHAREABLE + EL1RW_EL0RW + EXECUTE_ENABLE
#define EL1_MPU_REGION01_BASE_H     (0x0010 & 0xFFFF)
#define EL1_MPU_REGION01_LIMIT_L    (0xFFFF & 0xFFC0) + ATTRINDEX3 + REGION_ENABLE
#define EL1_MPU_REGION01_LIMIT_H    (0x0011 & 0xFFFF)

/* region 2 (System RAM) */
#define EL1_MPU_REGION02_BASE_L     (0x0000 & 0xFFC0) + NON_SHAREABLE + EL1RW_EL0RW + EXECUTE_ENABLE
#define EL1_MPU_REGION02_BASE_H     (0x1000 & 0xFFFF)
#define EL1_MPU_REGION02_LIMIT_L    (0xFFFF & 0xFFC0) + ATTRINDEX1 + REGION_ENABLE
#define EL1_MPU_REGION02_LIMIT_H    (0x1017 & 0xFFFF)

/* region 3 (System RAM mirror) */
#define EL1_MPU_REGION03_BASE_L     (0x0000 & 0xFFC0) + OUTER_SHAREABLE + EL1RW_EL0RW + EXECUTE_ENABLE
#define EL1_MPU_REGION03_BASE_H     (0x3000 & 0xFFFF)
#define EL1_MPU_REGION03_LIMIT_L    (0xFFFF & 0xFFC0) + ATTRINDEX3 + REGION_ENABLE
#define EL1_MPU_REGION03_LIMIT_H    (0x3017 & 0xFFFF)
```

### 3.2.8 segger rtt
- INIT()失败，不支持rtt？
```
#define INIT()  {                                                                                    \
                  volatile SEGGER_RTT_CB* pRTTCBInit;                                                \
                  pRTTCBInit = (volatile SEGGER_RTT_CB*)((char*)&_SEGGER_RTT + SEGGER_RTT_UNCACHED_OFF); \
                  do {                                                                               \
                    if (pRTTCBInit->acID[0] == '\0') {                                               \
                      _DoInit();                                                                     \
                    }                                                                                \
                  } while (0);                                                                       \
                }
```

### 3.2.9 总结
#### 3.2.9.1 单工程
- 在starup.c中引导norflash到sram和atcm，参考coremark工程
#### 3.2.9.2 load+app


[r01an6737jj0110-rzn2l-separating-loader-and-application](./DOC/r01an6737jj0110-rzn2l-separating-loader-and-application.pdf)
- 在loader中引导，在app中初始化
- 修改app ld sram->sram mirror
- 编译前手动clean loader或修改配置自动clean
![](./images/clean1.jpg)
![](./images/clean2.jpg)

## 3.3 日志系统
- 请参考从0卷出一个新项目之RA6M5系列。其中的支持同步异步打印，支持printf、littlefs、rtt等，并且其中的bug在fsp4.6.0已经修复了，可以参考移植。

## 3.4 中间层构建
### 3.4.1 了解e2s和fsp
- 因为fsp（Flexible Software Package）实质是大系列（RA RZN等）整合软件包+代码生成工具。
- 精通编译器当然也可以不用e2s，甚至IDE，但个人感觉fsp是绕不开的。
- https://github.com/renesas/rzn-fsp：
- e2s的帮助功能

Flexible Software Package (FSP) for Renesas RZ/N series.

FSP is the next generation Arm® MCU software package from Renesas, that enables secure devices and IoT connectivity through production ready peripheral drivers, FreeRTOS, and portable middleware stacks. FSP includes best-in-class HAL drivers with high performance and low memory footprint. Middleware stacks with FreeRTOS integration are included to ease implementation of complex modules like communication and security. The e² studio provides support with intuitive configurators and intelligent code generation to make programming and debugging easier and faster.

FSP uses an open software ecosystem and provides flexibility in using your preferred RTOS, legacy code, and third-party ecosystem solutions.

Related Links
FSP Releases : https://github.com/renesas/rzn-fsp/releases

FSP Documentation : https://renesas.github.io/rzn-fsp

RZ/N2L Product Information : https://www.renesas.com/rzn2l

e² studio : https://www.renesas.com/e2studio

Support : https://www.renesas.com/support

![](./images/help.png)

### 3.4.2 fsp工程目录结构简介

![](./images/xxx_prj.png)


# 五、 RZ/T2L-基于官方开发套件的BaseProject-coremark移植

[跳转](./coremark%20for%20T2L/Baseproject%20for%20RZT2L.md)

# 六、loader app分离工程
## 6.1 概述
 对比“3.2 base project构建”中的单个工程，本章节介绍loader app分离的双工程。可以实现更为灵活和复杂的功能，例如多协议支持、固件升级等等。

## 6.2 官方资料与不足
![](./images/loaderapp9.png)
 - 例程源码
    RZN2L_bsp_xspi0bootx1_app
    RZN2L_bsp_xspi0bootx1_loader
 - 文档
 [r01an6737ej0110-rzn2l-separating-loader-and-application.pdf](./DOC/r01an6737ej0110-rzn2l-separating-loader-and-application.pdf)
 - 仅展示
![](./images/loaderapp0.png) 
 以官方loader app双工程为base去移植应用代码，会碰到很多问题

## 6.3 loader app分离工程的优化
### 6.3.1 自动调节合并appsection
![](./images/loaderapp3.png)
![](./images/loaderapp1.png)
![](./images/loaderapp4.png)
![](./images/loaderapp7.png)

### 6.3.2 loader中使用外设
![](./images/loaderapp5.png)
![](./images/loaderapp6.png)
![](./images/loaderapp2.png)
![](./images/loaderapp8.png)

```
//正确完整的编译loader+app
// 1: 必须首先clean loader，构建前增加make -r -j8 clean
// 2: 使能startu的初始化mpu代码 #if 1 // Original program
    /* Invalid these settings for loader project.
    * These settings are done in the application program.
    * Settings can also be made in the loader program if necessary. */

#if 1
    __enable_irq();
    g_uart0.p_api->open(g_uart0.p_ctrl, g_uart0.p_cfg);
    g_uart0.p_api->write(g_uart0.p_ctrl, (uint8_t const *)"Loader start!\n*****\nReady to Jump to the app!\n\n", strlen("Loader start!\n*****\nReady to Jump to the app!\n\n"));
    while(!uartTxCompleteFlg);
    uartTxCompleteFlg = 0;
    g_uart0.p_api->close(g_uart0.p_ctrl);
    __disable_irq();
#endif
```

### 6.3.3 app使用sram mirror
参考[fsp_xspi0_boot_SRAM_ATCM.ld](./rzn2l_coremark/script/fsp_xspi0_boot_SRAM_MIRROR.ld)
### 6.3.4 sram atcm同时使用
- 参考[fsp_xspi0_boot_SRAM_ATCM.ld](./rzn2l_coremark/script/fsp_xspi0_boot_SRAM_MIRROR.ld)
- 在loader中复制atcm和sram


