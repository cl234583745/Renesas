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
- xspi boot---qspi---000
- sci（uart）boot
- usb boot
![](./images/boot%20mode.png)
![](./images/boot1.png)

- flash qspi：AT25，W25，mx25
![](./images/qspi.png)
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
### 3.1.1 IDE下载、安装和设置

### 3.1.2 FSP软件包安装和更新

### 3.1.3 FSP各项功能简介

### 3.1.4 FSP的复制与差分

## 3.2 向导新建工程

## 3.3 导入工程

## 3.4 debug调试和下载

## 3.5 启动模式与链接脚本

## 3.6 TCM与SRAM
