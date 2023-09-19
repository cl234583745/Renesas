**瑞萨RZN2L工业以太网MPU视频笔记** 
三、瑞萨RZN2L开发工具e2studio安装
[toc]
# 一、开发工具总览

| 开发软件 | [1.e2studio 2023-07 ](https://www.renesas.cn/cn/zh/software-tool/e-studio)<br>[2.e² studio and RZ/N2L FSP Installer](https://www.renesas.cn/cn/zh/products/microcontrollers-microprocessors/rz-mpus/rzn2l-integrated-tsn-compliant-3-port-gigabit-ethernet-switch-enables-various-industrial-applications?software-type-filter=ide-and-coding-tool#tab-software-tools) |[1.setup_rznfsp_v1_2_0_e2s](https://github.com/renesas/rzn-fsp/releases/download/v1.2.0/setup_rznfsp_v1_2_0_e2s_v2023-04.exe)<br>[2.RZN_FSP_Packs_v1.2.0](https://github.com/renesas/rzn-fsp/releases/download/v1.2.0/RZN_FSP_Packs_v1.2.0.zip)<br>[3.setup_rznfsp_v1_2_0_rzsc](https://github.com/renesas/rzn-fsp/releases/download/v1.2.0/setup_rznfsp_v1_2_0_rzsc_v2023-04.exe)| [IAR:<br>9.32.2+rzsc](https://www.iar.com/products#!?architecture=Arm) |
|:-:|:-:|:-:|:-:|
|下载地址|瑞萨官网|Github|IAR+Github|
|软件包|Github下载|Github下载|Github下载|
|编译工具链<br>Toolchain|GCC在线下载|整合|IAR Compiler|
|烧写软硬件|SEGGER J-Link: 7.80b later<br>J-Link V11|SEGGER J-Link: 7.80b later<br>J-Link V11|IAR I-jet|
 


## 1.1 下载
### 1.1.1 e2studio官网下载
![](./images/download2.png)
![](./images/download3.png)

### 1.1.2 Github下载FSP
![](./images/download4.png)


## 1.2 安装
**e2s安装目录** *\Renesas\e2_studio
例如：D:\Renesas\e2_studio

**FSP安装**
v1.0.0,v1.1.0,v1.2.0安装包安装在根目录\Renesas\e2_studio
文件包复制到\Renesas\e2_studio\internal
![](./images/fsp1.png)
![](./images/fsp2.png)

**编译工具安装**
建议安装e2s中在线安装，添加环境变量

# 二、开发板
## 2.1 资料文档
![](./images/download1.png)

1、[Renesas Starter Kit+ for RZ/N2L User's Manual](https://www.renesas.cn/cn/zh/document/mat/renesas-starter-kit-rzn2l-users-manual?r=1622401)
2、[RZ/T2, RZ/N2 Getting Started with Flexible Software Package](https://www.renesas.cn/cn/zh/document/qsg/rzt2-rzn2-getting-started-flexible-software-package?r=1622401)
3、[Renesas Starter Kit+ for RZ/N2L Design Package 原理图](https://www.renesas.cn/cn/zh/products/microcontrollers-microprocessors/rz-mpus/rzn2l-rsk-renesas-starter-kit-rzn2l?samplecodes-title-filter=Design+Guide&documents-type-filter=%E6%89%8B%E5%86%8C-%E7%A1%AC%E4%BB%B6)
4、[RZ/T2M, RZ/N2L Group Hardware Design Guide 手册 - 硬件](https://www.renesas.cn/cn/zh/document/mah/rzt2m-rzn2l-group-hardware-design-guide?r=1622401)

## 2.2 RSK开发板
1、debug连接：usb-type c + jlink OB + USB to serial
2、USB口在背面
3、sw4 boot mode： sw4-1~3 on|on|on: xspi boot mode
![](./images/rsk1.png)

#三、注意事项
## 3.1 e2studio窗口主要功能
1、configuration.xml：启动FSP
2、FSP主要功能：BSP、Pins、Stacks、属性、引脚图
3、生成代码：rzn_gen、rzn_cfg、script不可修改
![](./images/src.png)
## 3.2 Debug视图前进后退
**e2studio 2023-07默认隐藏工具栏按钮**
![](./images/note2.png)
**C视图/FSP视图/Debug视图工具栏按钮设置**
![](./images/shitu.png)
![](./images/shitu2.png)


## 3.3 修改CPSR脚本
**e2studio 2023-04自动修改RZN2L CPSR**
![](./images/note1.png)

**e2studio 2023-04以下版本修改CPSR脚本**

**保存下面为change_cpsr.py，放到工程根目录**
```
# --- --- stop handler ---
def stop_handler (event):
    pc = gdb.parse_and_eval('$pc');
    address = gdb.parse_and_eval('system_init');
    if pc == address:
        gdb.execute('set $cpsr = $cpsr & 0xffffffdf');
        gdb.events.stop.disconnect (stop_handler)
# --- stop handler ---

gdb.events.stop.connect (stop_handler)
```
**debug configurations启动change_cpsr.py**
![](./images/cpsr.jpg)

## 3.4 configuration.xml打不开
**e2s相关依赖必须安装**
![](./images/install1.png)

# 四、调试RSK开发板，输出hello world
**1、下载例程包**
RZ/N2L Group Example program
[Toolchains=GNU ARM Embedded|v9.3.1,20200408;IAR Embedded Workbench for ARM v9.30.1]

**2、导入串口例程**
\r01an6538ej0120-rzn2l-example-program-package\RZN2L_RSK_sci_uart_Rev110\basic\gcc\RZN2L_RSK_sci_uart_Rev110a

**3、编译debug到RSK**
printf("Hello World\n");
串口输出。

**4、新建工程向导**
移植RA（ARM M33 M4） 的串口重定向功能，RZN2L（ARM R52）内核依然好用