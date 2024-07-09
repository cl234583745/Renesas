十一、从0开始卷出一个新项目之瑞萨RZN2L rzn2l_coremark_fsp200工程构建和gcc .ld链接文件的修改方法
===
[toc]

# 一、概述
- 在[四、从0开始卷出一个新项目瑞萨RZN2L之RZT2L BaseProject coremark的移植](https://mp.weixin.qq.com/s?__biz=MzkxNDQyMTU4Mg==&mid=2247484416&idx=1&sn=b3d2ea6af5ac5dd3a41b76e2e6757c82&chksm=c16fe621f6186f3721bd4221f6f5ad7f26fa1ad39227122f87930eb88c6ea5e8b5b8f7653d16&scene=178&cur_album_id=3167963498191110153#rd)介绍过rzn2l coremark工程是基于rzn-fsp v1.2.0
- 在上一节【】介绍了rzn-fsp v2.0.0 release note
- 介绍rzn2l_fsp2.0.0_coremark工程构建过程碰到的一些问题
- 借此重点介绍gcc .ld文件的几种修改方法（为什么要修改优化等级和链接文件：[嵌入式科普(22)c代码究竟是如何一步步运行的之链接](https://mp.weixin.qq.com/s?__biz=MzkxNDQyMTU4Mg==&mid=2247485205&idx=1&sn=43ecb1082fda68391c2c907115796c35&chksm=c16fe534f6186c221419e1298e9657d93e9af084e93a63c0a39fa7e27c2bd6899344cfa49434&cur_album_id=3188272463181365248&scene=189#wechat_redirect)）
  
  
# 二、rzn2l_fsp2.0.0_coremark工程构建

## 2.1 目录结构
- coremark文件夹（用户代码）放在项目根目录下，需要增加源文件夹；
  
![](./images/1.png)
![](./images/2.png)
![](./images/3.png)

## 2.2 项目一览
- 增加syscall文件，解决gcc 10.3以上版本newlib 相关警告
  [三、e2studio VS STM32CubeIDE之瑞萨RA FSP5.0.0裸机printf重定向](https://mp.weixin.qq.com/s?__biz=MzkxNDQyMTU4Mg==&mid=2247484386&idx=1&sn=849038da2ff1d004b09e990a501db7e0&chksm=c16fe1c3f61868d59cc04e46b9e6c9ce5b2b2b454099f114ab3321aac34ff06e6181e8456abe&cur_album_id=3243283741519757319&scene=190#rd)
  
- 项目汇总一览，scrip readme.txt

![](./images/4.png)


# 三、代码优化的问题
- rzn-fsp v1.2.0版本及之前版本，可以选择多种优化等级，并且可以通过coremark看到优化和结果，优化等级越高结果越高；
- rzn-fsp v1.3.0版本及之后版本，优化等级只能选择-O1和-Og，其他等级无法运行；

![](./images/5.png)

- [嵌入式科普(21)c代码究竟是如何一步步运行的之编译](https://mp.weixin.qq.com/s?__biz=MzkxNDQyMTU4Mg==&mid=2247485188&idx=1&sn=cb933568c11d077905639ff28ec38836&chksm=c16fe525f6186c3393ce244b67e6d86c55c2eb7289ff233ba89d58948cd6519f855a68b6f559&cur_album_id=3188272463181365248&scene=190#rd) 启发
- system.c 单独优化#pragma GCC optimize ("O1") （[嵌入式科普(19)typedef与#define的区别和典型应用](https://mp.weixin.qq.com/s?__biz=MzkxNDQyMTU4Mg==&mid=2247485066&idx=1&sn=100e38d1797a1d6e09a6943690139cdf&chksm=c16fe4abf6186dbde662dbbb327a01369ab9510c3869d93b37b02a78e416be988895c2575d47&cur_album_id=3188272463181365248&scene=190#rd)提到过3.4 #pragma：）

![](./images/6.png)


# 四、gcc .ld链接文件修改方法

## 4.1 .ld文件的构成
### 4.1.1 包含另一个ld文件
    例如：
    INCLUDE memory_regions.ld

### 4.1.2 符号/变量
    例如：
    ATCM_START = 0x00000000;
    ATCM_LENGTH = 0x20000;
    ATCM_PRV_START = DEFINED(ATCM_START) ? ATCM_START : 0;
    ATCM_PRV_LENGTH = DEFINED(ATCM_LENGTH) ? ATCM_LENGTH : 0;

### 4.1.3 MEMORY 定义内存区域
    例如：
    MEMORY
    {
        ATCM : ORIGIN = ATCM_PRV_START, LENGTH = ATCM_PRV_LENGTH
        BTCM : ORIGIN = BTCM_PRV_START, LENGTH = BTCM_PRV_LENGTH
        SYSTEM_RAM : ORIGIN = SYSTEM_RAM_PRV_START, LENGTH = SYSTEM_RAM_PRV_LENGTH
        ....
    ｝

### 4.1.4 SECTIONS 定义段到内存的映射
    例如：
    SECTIONS
    {
        .loader_param LOADER_PARAM_ADDRESS : AT (LOADER_PARAM_ADDRESS)
        {
            KEEP(*(.loader_param))
        } > xSPI0_CS0_SPACE
        ....
        .text TEXT_ADDRESS : AT (_mtext)
        {
            _text_start = .;
            *(.text*)

            KEEP(*(.reset_handler))
            KEEP(*(.init))
            KEEP(*(.fini))

            /* .ctors */
            *crtbegin.o(.ctors)
            *crtbegin?.o(.ctors)
            *(EXCLUDE_FILE(*crtend?.o *crtend.o) .ctors)
            *(SORT(.ctors.*))
            *(.ctors)
            _ctor_end = .;

            /* .dtors */
            *crtbegin.o(.dtors)
            *crtbegin?.o(.dtors)
            *(EXCLUDE_FILE(*crtend?.o *crtend.o) .dtors)
            *(SORT(.dtors.*))
            *(.dtors)
            _dtor_end = .;

            KEEP(*(.eh_frame*))
        } > RAM
        ....
    }


### 4.1.5 ENTRY VERSIONS等
    略

## 4.2 .ld文件的重要语法

- VERSIONS：定义符号版本。
- ORIGIN：定义内存区域的起始地址。
- LENGTH：定义内存区域的长度。
- AT：指定加载地址（对于非连续内存模型）。
- FILL：用于填充未使用的内存空间。
- SORT：用于对段进行排序。
- ALIGN：用于对齐段的起始地址。
- PROVIDE：为链接器提供一个符号定义，但不创建实际的代码或数据。
- ASSERT：用于在链接时检查条件是否满足。
- 通配符

## 4.3 ram执行：fsp_ram_execution.ld
- fsp-BSP-Board-(RAM execution without flash memory)
- 未下载到flash，掉电/复位无法继续执行；boot+app
- btcm(loader&stack)+atcm(text&data&stack&heap)

## 4.4 flash启动：fsp_xspi0_boot.ld
- fsp-BSP-Board-(xSPI0 x1 boot mode)
- 下载到flash，掉电/复位可执行，需等待写flash
- btcm(loader&stack)+atcm(text&data&stack&heap)

## 4.5 app全部运行在sram：fsp_xspi0_boot_sram_all.ld
- fsp-BSP-Board-(xSPI0 x1 boot mode)
- 下载到flash，掉电/复位可执行，需等待写flash
- btcm(loader&stack)+sram(text&data&stack&heap)
- dma、usb等需要注意处理，使用nocache

## 4.6 仅text使用sram：fsp_xspi0_boot_sram_text.ld
- fsp-BSP-Board-(xSPI0 x1 boot mode)
- 下载到flash，掉电/复位可执行，需等待写flash
- btcm(loader&stack)+sram(text)+atcm(data、stack、heap)

## 4.7 优先使用sram特殊使用atcm：fsp_xspi0_boot_sram_atcm.ld
- fsp-BSP-Board-(xSPI0 x1 boot mode)
- 下载到flash，掉电/复位可执行，需等待写flash
- btcm(loader&stack)+sram(优先使用-text&data&stack&heap)+atcm(*/coremark文件夹-text&data)
- 不改变原有分区结构，只增加单独文件夹使用atcm
- 需要修改system.c，增加复制过程。通过宏控制FSP_XSPI0_BOOT_SRAM_ATCM

![](./images/7.png)

## 4.8 参考separating-loader-and-application
略

# 五、gcc -fno-strict-aliasing(禁用严格别名规则)
- -fno-strict-aliasing 是一个 GCC（GNU Compiler Collection）的编译选项，用于禁用严格别名规则（strict aliasing rules）
- https://so.csdn.net/so/search?spm=1001.2101.3001.4498&q=-fno-strict-aliasing&t=&u=
- rzn-fsp新版本开始使用，什么作用？
  
![](./images/8.png)  


# 六、测试结果

```
-Og
hello word!
date:Jun 20 2024
time:11:16:16
file:../src/hal_entry.c
func:hal_entry,line:81
hello world!
PI=3.141593
start coremain!!!

[11:17:14.072]收←◆2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 24856
Total time (secs): 24.856000
Iterations/Sec   : 804.634696
Iterations       : 20000
Compiler version : GCC12.2.1 20230214
Compiler flags   : Please put compiler flags here (e.g. -o3)
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x382f
Correct operation validated. See README.md for run and reporting rules.
CoreMark 1.0 : 804.634696 / GCC12.2.1 20230214 Please put compiler flags here (e.g. -o3) / STACK
FSP_PRIV_CLOCK_CPU0=400000000
running!!!
```
```

-O1
hello word!
date:Jun 20 2024
time:11:10:55
file:../src/hal_entry.c
func:hal_entry,line:81
hello world!
PI=3.141593
start coremain!!!

[11:11:59.520]收←◆2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 19502
Total time (secs): 19.502000
Iterations/Sec   : 1025.535842
Iterations       : 20000
Compiler version : GCC12.2.1 20230214
Compiler flags   : Please put compiler flags here (e.g. -o3)
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x382f
Correct operation validated. See README.md for run and reporting rules.
CoreMark 1.0 : 1025.535842 / GCC12.2.1 20230214 Please put compiler flags here (e.g. -o3) / STACK
FSP_PRIV_CLOCK_CPU0=400000000
running!!!
```
```
-Ofast
hello word!
date:Jun 20 2024
time:10:23:02
file:../src/hal_entry.c
func:hal_entry,line:81
hello world!
PI=3.141593
start coremain!!!

[11:08:26.840]收←◆2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 15781
Total time (secs): 15.781000
Iterations/Sec   : 1267.346809
Iterations       : 20000
Compiler version : GCC12.2.1 20230214
Compiler flags   : Please put compiler flags here (e.g. -o3)
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x382f
Correct operation validated. See README.md for run and reporting rules.
CoreMark 1.0 : 1267.346809 / GCC12.2.1 20230214 Please put compiler flags here (e.g. -o3) / STACK
FSP_PRIV_CLOCK_CPU0=400000000
running!!!
```
```
-O3
hello word!
date:Jun 20 2024
time:11:13:40
file:../src/hal_entry.c
func:hal_entry,line:81
hello world!
PI=3.141593
start coremain!!!

[11:14:34.311]收←◆2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 15781
Total time (secs): 15.781000
Iterations/Sec   : 1267.346809
Iterations       : 20000
Compiler version : GCC12.2.1 20230214
Compiler flags   : Please put compiler flags here (e.g. -o3)
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x382f
Correct operation validated. See README.md for run and reporting rules.
CoreMark 1.0 : 1267.346809 / GCC12.2.1 20230214 Please put compiler flags here (e.g. -o3) / STACK
FSP_PRIV_CLOCK_CPU0=400000000
running!!!
```


