**六、RX MCU I2C Slave使用FIT 3.11驱动指南**

[TOC]

# 一、概述/目的

本文档介绍如何使用瑞萨RX MCU（RX23E-A）作为I2C Slave，配合FIT 3.11驱动实现非阻塞式收发。包含驱动原理分析、常见问题解决方案及封装后的易用API。

# 二、实验环境

- MCU: RX23E-A
- I2C驱动: FIT 3.11 (r_riic_rx)
- Master: STM32U5 HAL I2C
- I2C速率: 100kHz (Standard Mode)
- Slave地址: 0x4A (7-bit)

# 三、FIT 3.11 RIIC驱动原理

## 3.1 驱动架构

FIT 3.11的RIIC驱动采用中断驱动架构，主要包含以下层级：

```
┌─────────────────────────────────────┐
│         用户应用层 (User App)        │
├─────────────────────────────────────┤
│       接口层 (r_riic_rx_if.h)        │
├─────────────────────────────────────┤
│       核心层 (r_riic_rx.c)           │
│  ┌─────────────────────────────┐    │
│  │   状态机 (riic_advance)     │    │
│  │   事件处理 (riic_func_table)│    │
│  └─────────────────────────────┘    │
├─────────────────────────────────────┤
│       目标层 (r_riic_rx23e_a.c)      │
│       (ISR封装、硬件寄存器操作)       │
├─────────────────────────────────────┤
│       硬件 (RIIC0/RIIC1)            │
└─────────────────────────────────────┘
```

## 3.2 关键数据结构

```c
/* 用户可见的I2C信息结构 */
typedef volatile struct {
    uint8_t              rsv2;        /* 保留 */
    uint8_t              rsv1;        /* 保留（可用于存储方向） */
    riic_ch_dev_status_t dev_sts;     /* 设备状态 */
    uint8_t              ch_no;       /* 通道号 */
    riic_callback        callbackfunc;/* 回调函数 */
    uint32_t             cnt2nd;      /* 接收计数器 */
    uint32_t             cnt1st;      /* 发送计数器 */
    uint8_t            * p_data2nd;   /* 接收缓冲区指针 */
    uint8_t            * p_data1st;   /* 发送缓冲区指针 */
    uint8_t            * p_slv_adr;   /* 从机地址指针 */
} riic_info_t;
```

## 3.3 内部状态机

驱动内部维护一个状态机，主要状态包括：

```c
typedef enum {
    RIIC_MODE_NONE = 0,      /* 非通信模式 */
    RIIC_MODE_M_SEND,        /* 主机发送 */
    RIIC_MODE_M_RECEIVE,     /* 主机接收 */
    RIIC_MODE_M_SEND_RECEIVE,/* 主机收发 */
    RIIC_MODE_S_READY,       /* 从机就绪 */
    RIIC_MODE_S_SEND,        /* 从机发送（Master读） */
    RIIC_MODE_S_RECEIVE      /* 从机接收（Master写） */
} riic_api_mode_t;
```

## 3.4 从机传输流程

```
Master写 (Slave收):
  START → 地址(W) → ACK → 数据 → ACK → ... → STOP
                                            ↓
                                    dev_sts = RIIC_FINISH (0x02)

Master读 (Slave发):
  START → 地址(R) → ACK → 数据 → ACK → ... → NACK → STOP
                                            ↓
                                    dev_sts = RIIC_NACK (0x03)
```

**注意**: NACK是I2C协议的正常行为，表示Master读取完成。

## 3.5 关键中断处理

| 中断 | 处理函数 | 作用 |
|------|----------|------|
| TXI | riic0_txi_sub | 从机发送数据（Master读时触发） |
| RXI | riic0_rxi_sub | 从机接收数据（Master写时触发） |
| EEI | riic0_eei_sub | 错误/事件（START/STOP/NACK等） |

# 四、常见问题与解决方案

## 4.1 问题1: Write后Read失败

**现象**: Master先Write再Read，Write成功但Read失败（NACK）。

**原因**: 回调函数中的`printf`阻塞ISR过长（~3.9ms），导致SAR0E（从机地址匹配）在Master发READ START前未恢复。

**解决**:
```c
/* 错误：回调中做printf（耗时~3.9ms） */
static void bad_callback(void) {
    printf("status=%d\n", status);  /* 阻塞ISR！ */
}

/* 正确：回调中只设标志 */
static volatile bool transfer_done = false;
static void good_callback(void) {
    transfer_done = true;  /* 极快，不阻塞ISR */
}
```

## 4.2 问题2: 如何判断Write/Read方向

**背景**: `ICCR2.TRS`位在STOP后被硬件清零，无法在回调中读取。

**解决方案对比**:

| 方案 | 修改位置 | 优点 | 缺点 |
|------|----------|------|------|
| Method 0 | 无 | 无需改驱动 | 依赖dev_sts |
| Method 1 | r_riic_rx.c | 更健壮 | 需改生成代码 |

### Method 0: 使用dev_sts（默认）

```c
if (RIIC_FINISH == dev_sts)     /* Master写 */
{
    /* 处理接收数据 */
}
else if (RIIC_NACK == dev_sts)  /* Master读 */
{
    /* 更新发送数据 */
}
```

### Method 1: 使用rsv1字段

需修改`r_riic_rx.c`的`riic_advance()`函数：

```c
/* 在 riic_set_ch_status() 之后，回调之前添加 */
if (RIIC_MODE_S_SEND == riic_api_info[p_riic_info->ch_no].N_Mode)
{
    p_riic_info->rsv1 = 1;  /* 从机发送（Master读） */
}
else if (RIIC_MODE_S_RECEIVE == riic_api_info[p_riic_info->ch_no].N_Mode)
{
    p_riic_info->rsv1 = 0;  /* 从机接收（Master写） */
}
```

**修改位置**: `r_riic_rx.c` → `riic_advance()` → `riic_set_ch_status(p_riic_info, RIIC_FINISH)` 之后

# 五、封装API: rx_fit_i2c_slave

## 5.1 文件结构

```
src/
├── rx_fit_i2c_slave.h    /* API头文件 */
├── rx_fit_i2c_slave.c    /* 实现 */
└── smc_gen/
    └── r_riic_rx/        /* FIT驱动（自动生成） */
```

## 5.2 API说明

### 初始化

```c
#include "rx_fit_i2c_slave.h"

/* 事件回调 */
static void on_event(rx_fit_i2c_slave_event_t event)
{
    if (event == RX_FIT_I2C_SLAVE_EVT_WRITE)
    {
        uint8_t *rx = rx_fit_i2c_slave_get_rx_buf();
        printf("Master写: %02x %02x %02x\n", rx[0], rx[1], rx[2]);
    }
    else
    {
        uint8_t *tx = rx_fit_i2c_slave_get_tx_buf();
        printf("Master读: %02x %02x %02x\n", tx[0], tx[1], tx[2]);
        tx[2]++;  /* 更新数据供下次读取 */
    }
}

void main(void)
{
    /* 初始化：从机地址在Smart Configurator配置 */
    rx_fit_i2c_slave_init(on_event);

    while (1)
    {
        rx_fit_i2c_slave_process();  /* 非阻塞处理 */
    }
}
```

### API列表

| 函数 | 说明 |
|------|------|
| `rx_fit_i2c_slave_init(cb)` | 初始化，注册回调 |
| `rx_fit_i2c_slave_process()` | 主循环调用，非阻塞 |
| `rx_fit_i2c_slave_is_busy()` | 查询忙状态 |
| `rx_fit_i2c_slave_get_rx_buf()` | 获取接收缓冲区 |
| `rx_fit_i2c_slave_get_tx_buf()` | 获取发送缓冲区 |

### 宏配置

```c
/* 在rx_fit_i2c_slave.h中配置 */
#define RX_FIT_I2C_SLAVE_DIR_METHOD  0  /* 方向检测方法 */

/* 0 = 使用dev_sts（默认，无需改驱动） */
/* 1 = 使用rsv1字段（需改r_riic_rx.c） */
```

## 5.3 测试结果

**Slave日志**:
```
RX: aa bb cc dd cf
TX: ab bc 00 00 14
RX: aa bb cc dd d0
TX: ab bc 00 00 15
RX: aa bb cc dd d1
TX: ab bc 00 00 16
```

**Master日志**:
```
hi2c1 write:
aa bb cc dd cf 
hi2c1 read:
ab bc 00 00 14 
hi2c1 write:
aa bb cc dd d0 
hi2c1 read:
ab bc 00 00 15 
hi2c1 write:
aa bb cc dd d1 
hi2c1 read:
ab bc 00 00 16 
```

# 六、注意事项

1. **回调函数必须快速**: 不要在ISR回调中做printf、延时等耗时操作
2. **地址配置**: 从机地址在Smart Configurator (`r_riic_rx_pin_config.h`) 中配置，不在代码中设置
3. **Buffer大小**: 发送/接收缓冲区大小需匹配实际数据长度
4. **生成代码管理**: 修改`r_riic_rx.c`后，Smart Configurator重新生成会覆盖修改，建议做好备份
5. **dev_sts含义**:
   - `RIIC_FINISH (0x02)` = Master写完成
   - `RIIC_NACK (0x03)` = Master读完成（正常I2C行为）

# 七、参考资料

- [R01AN1692EJ0311 - RIIC Application Note](./src/smc_gen/r_riic_rx/doc/en/r01an1692ej0311-rx-riic.pdf)
- [RX23E-A Group User's Manual](https://www.renesas.com/us/en/document/man/rx23e-a-group-users-manual)
- [FIT RX Series RIIC Driver](https://www.renesas.com/us/en/software-tool/fit-interface-software-modules/fit-rx-series-riic-driver)
