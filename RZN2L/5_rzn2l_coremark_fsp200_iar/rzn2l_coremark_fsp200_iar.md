十二、从0开始卷出一个新项目之瑞萨RZN2L 基于IAR的coremark fsp200工程构建和iar .icf链接文件的修改方法
===
[toc]

# 一、概述
- 在[十一、从0开始卷出一个新项目之瑞萨RZN2L](https://mp.weixin.qq.com/s?__biz=MzkxNDQyMTU4Mg==&mid=2247485293&idx=1&sn=dab3ac3d4144489e598472ca9ea362d8&chksm=c16fe54cf6186c5aa655143fc9ce6a6ecaf081a5cf89e8c0566595988c5f92bb0d895ac9b512&token=30647263&lang=zh_CN#rd)介绍过GCC编译器的工程构建
- 基于IAR的coremark fsp200工程构建和iar .icf链接文件的修改方法
- 介绍基于IAR的工程构建过程碰到的一些问题（弄了很久）
- 借此重点介绍iar .icf文件的几种修改方法（为什么要修改优化等级和链接文件：[嵌入式科普(22)c代码究竟是如何一步步运行的之链接](https://mp.weixin.qq.com/s?__biz=MzkxNDQyMTU4Mg==&mid=2247485205&idx=1&sn=43ecb1082fda68391c2c907115796c35&chksm=c16fe534f6186c221419e1298e9657d93e9af084e93a63c0a39fa7e27c2bd6899344cfa49434&cur_album_id=3188272463181365248&scene=189#wechat_redirect)）
  
  
# 二、rzn2l_fsp2.0.0_coremark工程构建

## 2.1 目录结构
详见[十一、从0开始卷出一个新项目之瑞萨RZN2L](https://mp.weixin.qq.com/s?__biz=MzkxNDQyMTU4Mg==&mid=2247485293&idx=1&sn=dab3ac3d4144489e598472ca9ea362d8&chksm=c16fe54cf6186c5aa655143fc9ce6a6ecaf081a5cf89e8c0566595988c5f92bb0d895ac9b512&token=30647263&lang=zh_CN#rd)

## 2.2 项目一览
略


## 2.3 iar工程打开报错如何处理
- iar版本不向下兼容，iar9.40.1
- build action中2个cmd可以取消
![](./images/20.png)
- 升级rzsc
- 上传临时文件夹，rzsc重新生成代码和配置

# 三、代码优化的问题
## 3.1 system.c中复制内存
问题：内存复制相关代码被优化，导致无法初始化
- **iar不支持gcc的“范围单独优化”：**
```
#pragma GCC optimize ("O1")
#pragma GCC reset_options
```
- iar只支持“单独优化函数”或通过配置优化单个文件
```
#pragma optimize=low
```
深入浅出编译优化选项（上）:https://www.iar.com/zh/knowledge/learn/programming/compiler-optimization-1
![](./images/10.png)

## 3.2 iar代码优化等级与volatile关键字
问题：printf重定向的串口发送标志位，iar9.40.1错误的优化，volatile无效
- **IAR：**
```
volatile bool uart_send_complete_flag = false;
void user_uart_callback (uart_callback_args_t * p_args)
{
    if(p_args->event == UART_EVENT_TX_COMPLETE)
    {
        uart_send_complete_flag = true;
    }
}
void IoPutchar (int32_t buffer)
{
      R_SCI_UART_Write(&g_uart0_ctrl, (uint8_t*) &buffer, TRANSFER_LENGTH);
      while (!g_transfer_complete)
      {
      }
      g_transfer_complete = 0;
}
```

| IAR优化等级 | 不使用 `volatile` | 使用 `volatile` |
| --- | --- | --- |
| `Low` | ![](./images/6.png) | ![](./images/6.png) |
| `Medium` | ![](./images/5.png) | ![](./images/5.png) |
| `DataTable1_2`|![](./images/7.png)|

-  **GCC：** 
```
volatile bool uart_send_complete_flag = false;
void user_uart_callback (uart_callback_args_t * p_args)
{
    if(p_args->event == UART_EVENT_TX_COMPLETE)
    {
        uart_send_complete_flag = true;
    }
}

_write(int fd, char *pBuffer, int size)
{
    (void)fd;
    fsp_err_t err = R_SCI_UART_Write(&g_uart0_ctrl, (uint8_t *)pBuffer, (uint32_t)size);
    if(FSP_SUCCESS != err) __BKPT();

    while(uart_send_complete_flag == false);
    uart_send_complete_flag = false;
    
    return size;
}
```
| gcc优化等级 | 不使用 `volatile` | 使用 `volatile` |
| --- | --- | --- |
| `-O1` | ![](./images/1.png) | ![](./images/4.png) |
| `-Ofast` | ![](./images/2.png) | ![](./images/3.png) |

- **iar9.40.1测试cubemx生成的103c8t6正常**
略

## 3.3 iar配置优化单个文件与预编译宏的范围
需要单独优化的文件，需要预编译宏控制的文件，是同一个文件。那么预编译宏也要单独定义
![](./images/11.png)

# 四、iar .icf链接文件修改方法
    iar icf与gcc ld文件对比：
        语法较为复杂，整体逻辑不清晰；
        不能自动递增地址，需要手动设置各个内存地址；
        只支持文件的通配符，不支持文件夹目录的通配符；
        工具生成的icf文件中符号命名不便于理解
## 4.1 .icf文件的构成
### 4.1.1 包含另一个icf文件
    例如：
```
    include "memory_regions.icf"
```

### 4.1.2 符号/变量symbol
    例如：
```
define symbol SYSTEM_RAM_END_OFFSET        = 0x00048000;
define symbol FLASH_ADDRESS                = xSPI0_CS0_SPACE_PRV_START;
```

### 4.1.3 region 定义内存区域
    例如：
    
```
define region ATCM_region              = mem:[from __region_ATCM_start__  to __region_ATCM_end__ ];
define region BTCM_region              = mem:[from __region_BTCM_start__  to __region_BTCM_end__ ];
```

### 4.1.4 定义段到内存的映射
    例如：
    
```
place in ROM_region   { block USER_PRG_RBLOCK, readonly };
place in S_RAM_region { block USER_DATA_RBLOCK, block USER_DATA_NONCACHE_RBLOCK };

define block USER_PRG_RBLOCK with alignment = 4 { ro code };
define block USER_PRG_WBLOCK with alignment = 4 { rw code };
define block USER_DATA_ZBLOCK with alignment = 4 { section .bss };
```

### 4.1.5 initialize

```
initialize manually

do not initialize
```

## 4.2 .icf文件的重要语法

[嵌入式科普(22)c代码究竟是如何一步步运行的之链接---4.2 IAR .icf](https://mp.weixin.qq.com/s?__biz=MzkxNDQyMTU4Mg==&mid=2247485205&idx=1&sn=43ecb1082fda68391c2c907115796c35&chksm=c16fe534f6186c221419e1298e9657d93e9af084e93a63c0a39fa7e27c2bd6899344cfa49434&token=30647263&lang=zh_CN#rd)
略

## 4.3 ram执行：fsp_ram_execution.icf
- fsp-BSP-Board-(RAM execution without flash memory)
- 未下载到flash，掉电/复位无法继续执行；boot+app
- btcm(loader&stack)+atcm(text&data&stack&heap)

## 4.4 flash启动：fsp_xspi0_boot.icf
- fsp-BSP-Board-(xSPI0 x1 boot mode)
- 下载到flash，掉电/复位可执行，需等待写flash
- btcm(loader&stack)+atcm(text&data&stack&heap)

## 4.5 app全部运行在sram：fsp_xspi0_boot_sram_all.icf
- fsp-BSP-Board-(xSPI0 x1 boot mode)
- 下载到flash，掉电/复位可执行，需等待写flash
- btcm(loader&stack)+sram(text&data&stack&heap)
- dma、usb等需要注意处理，使用nocache

## 4.6 仅text使用sram：fsp_xspi0_boot_sram_text.icf
- fsp-BSP-Board-(xSPI0 x1 boot mode)
- 下载到flash，掉电/复位可执行，需等待写flash
- btcm(loader&stack)+sram(text)+atcm(data、stack、heap)

## 4.7 优先使用sram特殊使用atcm：fsp_xspi0_boot_sram_atcm.icf
- fsp-BSP-Board-(xSPI0 x1 boot mode)
- 下载到flash，掉电/复位可执行，需等待写flash
- btcm(loader&stack)+sram(优先使用-text&data&stack&heap)+atcm(*/coremark文件夹-text&data)
- 不改变原有分区结构，只增加单独文件夹使用atcm
- 需要修改system.c，增加复制过程。通过宏控制FSP_XSPI0_BOOT_SRAM_ATCM



## 4.8 参考separating-loader-and-application
略

## 4.9 总结
\rzn2l_coremark_fsp200_iar\script\readme.txt

block PRG_RBLOCK    ro code
block PRG_WBLOCK    rw code

block DATA_ZBLOCK   bss
block DATA_RBLOCK   data
block DATA_WBLOCK   data_init

initialize manually
do not initialize


# 五、测试结果


```

hello word!
date:Jul  9 2024
time:10:12:22
file:C:\Users\jerry.chen\RASmartConfigurator\rzn2l_coremark_fsp200_iar\src\hal_entry.c
func:hal_entry,line:37
hello world!
PI=3.141593
check script ld first!!!
start coremain!!!

[10:16:25.284]收←◆2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 18050
Total time (secs): 18.050000
Iterations/Sec   : 1108.033241
Iterations       : 20000
Compiler version : Please put compiler version here (e.g. gcc 4.1)
Compiler flags   : Please put compiler flags here (e.g. -o3)
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x382f
Correct operation validated. See README.md for run and reporting rules.
CoreMark 1.0 : 1108.033241 / Please put compiler version here (e.g. gcc 4.1) Please put compiler flags here (e.g. -o3) / STACK
FSP_PRIV_CLOCK_CPU0=400000000
running!!!

```
```
[10:20:18.593]收←◆hello word!
date:Jul  9 2024
time:10:17:53
file:C:\Users\jerry.chen\RASmartConfigurator\rzn2l_coremark_fsp200_iar\src\hal_entry.c
func:hal_entry,line:37
hello world!
PI=3.141593
check script ld first!!!
start coremain!!!

[10:20:45.072]收←◆2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 26459
Total time (secs): 26.459000
Iterations/Sec   : 755.886466
Iterations       : 20000
Compiler version : Please put compiler version here (e.g. gcc 4.1)
Compiler flags   : Please put compiler flags here (e.g. -o3)
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x382f
Correct operation validated. See README.md for run and reporting rules.
CoreMark 1.0 : 755.886466 / Please put compiler version here (e.g. gcc 4.1) Please put compiler flags here (e.g. -o3) / STACK
FSP_PRIV_CLOCK_CPU0=400000000
running!!!

```
```
[10:23:06.511]收←◆hello word!
date:Jul  9 2024
time:10:21:49
file:C:\Users\jerry.chen\RASmartConfigurator\rzn2l_coremark_fsp200_iar\src\hal_entry.c
func:hal_entry,line:37
hello world!
PI=3.141593
check script ld first!!!
start coremain!!!

[10:23:40.199]收←◆2K performance run parameters for coremark.
CoreMark Size    : 666
Total ticks      : 33667
Total time (secs): 33.667000
Iterations/Sec   : 594.053524
Iterations       : 20000
Compiler version : Please put compiler version here (e.g. gcc 4.1)
Compiler flags   : Please put compiler flags here (e.g. -o3)
Memory location  : STACK
seedcrc          : 0xe9f5
[0]crclist       : 0xe714
[0]crcmatrix     : 0x1fd7
[0]crcstate      : 0x8e3a
[0]crcfinal      : 0x382f
Correct operation validated. See README.md for run and reporting rules.
CoreMark 1.0 : 594.053524 / Please put compiler version here (e.g. gcc 4.1) Please put compiler flags here (e.g. -o3) / STACK
FSP_PRIV_CLOCK_CPU0=400000000
running!!!



```