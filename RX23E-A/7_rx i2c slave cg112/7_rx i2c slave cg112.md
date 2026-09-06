**七、RX23E-A I2C Slave驱动调试记录——SMC代码生成器的坑与中断配置详解**

[TOC]

# 一、概述/目的
- 使用瑞萨RX23E-A的RIIC0作为I2C Slave，配合STM32 I2C Master进行收发通信。记录使用Smart Configurator（SMC）生成的RIIC Slave驱动过程中遇到的问题、解决方案，以及RIIC中断配置的详细分析。
- 本节使用i2c slave驱动类型是Code Generator 1.12.0,该驱动只是寄存器方式，与上一节使用的FIT的中间件方式不同，驱动会代码会更简洁。但是我们看见Zephyr使用的FIT驱动。
- 瑞萨的i2c slave只能选择riic/r_i2c,不能选择sci，因为sci只支持master模式

# 二、硬件环境
- **Slave**: RX23E-A (R5F523E6AxFL)
- **Master**: STM32 (HAL_I2C)
- **I2C地址**: 0x4A (7-bit), 寄存器写入0x94 (8-bit)
- **通信速率**: 标准模式
- **接线**: SCL=P16, SDA=P17 (已接上拉电阻)

# 三、Smart Configurator配置要点

## 3.1 ICIER中断使能配置

使用Smart Configurator生成RIIC0代码时，**必须在配置界面中勾选START和STOP中断使能**。

ICIER寄存器中与Slave相关的位：

| 位 | 名称 | 说明 | Slave必须? |
|----|------|------|-----------|
| bit0 | TMOIE | 超时中断 | 建议开 |
| bit1 | ALIE | 仲裁丢失中断 | 不需要 |
| bit2 | **STIE** | **START条件检测** | **必须开** |
| bit3 | **SPIE** | **STOP条件检测** | **必须开** |
| bit4 | NAKIE | NACK接收中断 | 可选 |

如果未勾选STIE和SPIE，生成的代码如下：

```c
// 未勾选STIE/SPIE时的生成结果
RIIC0.ICIER.BYTE = _01_IIC_TIMEOUT_INTERRUPT_ENABLE
                 | _02_IIC_ARBITRATION_LOST_INTERRUPT_ENABLE
                 | _00_IIC_START_CONDITION_INTERRUPT_DISABLE  // ← 未勾选
                 | _00_IIC_STOP_CONDITION_INTERRUPT_DISABLE  // ← 未勾选
                 | _10_IIC_NACK_INTERRUPT_ENABLE
                 | _20_IIC_RECEIVE_DATA_INTERRUPT_ENABLE
                 | _40_IIC_TRANSMIT_END_INTERRUPT_ENABLE
                 | _80_IIC_TRANSMIT_EMPTY_INTERRUPT_ENABLE;
```

**不勾选的后果**：EEI0中断处理函数依赖STIE和SPIE来检测START/STOP条件，禁用后slave无法正常通信。

## 3.2 R_Config_RIIC0_Create()调用

`R_Config_RIIC0_Create()` 负责RIIC0模块的完整初始化：
- 取消RIIC0模块停止状态（`MSTP(RIIC0) = 0`）
- 配置从机地址（`SARL0 = 0x94`）
- 配置ICIER中断使能寄存器
- 配置中断优先级
- 配置SCL/SDA引脚功能

**注意**：此函数在SystemInit中已调用，用户 `main()` 中无需重复调用，直接调用 `R_Config_RIIC0_Start()` 即可。

# 四、Slave收发模式切换要点

## 4.1 SMC驱动的核心限制

SMC生成的RIIC Slave驱动**一次只能处于一种模式**：
- `R_Config_RIIC0_Slave_Receive()` → 等待Master Write
- `R_Config_RIIC0_Slave_Send()` → 等待Master Read

**不能同时arm两个方向**。每次transfer完成后必须重新arm。

## 4.2 状态机流程

```
Master Write流程:
  Slave_Receive() → _15_WAIT_START
    → START检测(STIE) → _10_RECEIVES_DATA
    → 数据接收(RXI0) → _14_RECEIVES_STOP
    → STOP检测(SPIE) → callback_receiveend() → g_rx_done=1

Master Read流程:
  Slave_Send() → _15_WAIT_START
    → START检测(STIE) → _11_SENDS_DATA
    → 数据发送(TXI0) → _12_SENDS_END
    → TEI0中断 → _13_SENDS_STOP
    → STOP检测(SPIE) → callback_transmitend() → g_tx_done=1
```

## 4.3 模式切换策略

由于SMC驱动只能arm一个方向，采用**交替模式**：

```
初始化 → Slave_Receive()
  ↓ Master WRITE → g_rx_done=1 → 处理数据 → Slave_Send()
  ↓ Master READ  → g_tx_done=1 → 处理数据 → Slave_Receive()
  ↓ 循环
```

**限制**：如果连续同方向transfer（Write→Write 或 Read→Read），第二次transfer会静默失败（无callback，状态自动复位）。

## 4.4 中断回调设计原则

回调函数（ISR上下文）只设标志位，不阻塞：

```c
// Config_RIIC0_user.c — 回调函数
static void r_Config_RIIC0_callback_receiveend(void)
{
    g_rx_done = 1;  // 只设标志
}

static void r_Config_RIIC0_callback_transmitend(void)
{
    g_tx_done = 1;  // 只设标志
}
```

主循环处理数据和模式切换：

```c
while(1)
{
    if (g_rx_done)
    {
        g_rx_done = 0;
        printf("RX: %02x %02x %02x %02x %02x\n", ...);
        R_Config_RIIC0_Slave_Send(...);  // 切换到发送模式
    }
    if (g_tx_done)
    {
        g_tx_done = 0;
        printf("TX: %02x %02x %02x %02x %02x\n", ...);
        R_Config_RIIC0_Slave_Receive(...);  // 切换到接收模式
    }
}
```

# 五、RIIC中断详解（EEI0相关5个中断）

## 5.1 中断总览

RIIC的EEI0（Error/Event Interrupt0）是一个复合中断源，由5个子中断共用同一个中断向量。通过ICIER寄存器的各个bit独立使能，通过ICSR2寄存器的状态位判断具体触发源。

| 中断名称 | ICIER位 | 功能 | Slave必须? | Master必须? |
|---------|---------|------|-----------|------------|
| STIE | bit2 | START条件检测 | **必须** | 不需要 |
| SPIE | bit3 | STOP条件检测 | **必须** | 不需要 |
| TMOIE | bit0 | 超时检测 | 建议开 | 建议开 |
| ALIE | bit1 | 仲裁丢失 | 不需要 | 多主机时开 |
| NAKIE | bit4 | NACK接收 | 可选 | 按需开 |

## 5.2 STIE — START条件检测中断

**功能**：当RIIC检测到总线上的START条件时触发EEI0中断。

**Slave模式下必须开的原因**：

EEI0处理函数中，只有STIE触发时才能将状态从 `_15_WAIT_START_CONDITION` 转移到活动状态：

```c
// Config_RIIC0_user.c — EEI0处理函数
// Receive模式
else if (_15_IIC_SLAVE_WAIT_START_CONDITION == g_riic0_state)
{
    RIIC0.ICSR2.BIT.START = 0U;
    RIIC0.ICIER.BIT.STIE = 0U;   // 关STIE
    RIIC0.ICIER.BIT.SPIE = 1U;   // 开SPIE，准备检测STOP
    g_riic0_state = _10_IIC_SLAVE_RECEIVES_DATA;  // 进入接收状态
}
```

**不开的后果**：slave永远停在 `_15_WAIT_START` 状态，无法响应任何transfer。

**Master模式**：Master自己产生START条件，硬件自动设置ST标志位，通过轮询或TEI/TXI中断即可知道transfer开始，不需要STIE中断。

## 5.3 SPIE — STOP条件检测中断

**功能**：当RIIC检测到总线上的STOP条件时触发EEI0中断。

**Slave模式下必须开的原因**：

transfer完成后，必须通过SPIE触发EEI0来调用回调函数：

```c
// Receive完成
if (_14_IIC_SLAVE_RECEIVES_STOP == g_riic0_state)
{
    RIIC0.ICSR2.BIT.STOP = 0U;
    RIIC0.ICIER.BIT.SPIE = 0U;   // 关SPIE
    RIIC0.ICIER.BIT.STIE = 1U;   // 开STIE，准备检测下次START
    r_Config_RIIC0_callback_receiveend();  // 触发回调
}

// Transmit完成
if (_13_IIC_SLAVE_SENDS_STOP == g_riic0_state)
{
    RIIC0.ICSR2.BIT.NACKF = 0U;
    RIIC0.ICSR2.BIT.STOP = 0U;
    RIIC0.ICIER.BIT.SPIE = 0U;
    RIIC0.ICIER.BIT.STIE = 1U;
    r_Config_RIIC0_callback_transmitend();  // 触发回调
}
```

**不开的后果**：transfer完成后无法触发回调，状态机卡死，slave无法响应下一次transfer。

**Master模式**：Master自己产生STOP条件，硬件自动设置SP标志位，通过轮询SP位或TEI中断即可，不需要SPIE中断。

## 5.4 TMOIE — 超时检测中断

**功能**：当SCL被拉低超过规定时间（由ICMR2的超时配置决定）时触发。

**用途**：总线死锁保护。如果slave异常把SCL拉低不放（如通信异常导致状态机卡住），超时中断会触发恢复流程。

**不开的后果**：正常通信不受影响，但失去总线死锁保护。如果SCL被异常拉低，总线将永久挂死，只能通过复位恢复。

**建议**：Slave和Master都建议开启，作为安全保护机制。

## 5.5 ALIE — 仲裁丢失中断

**功能**：在多主机系统中，当两个主机同时发起START时，检测仲裁结果。仲裁丢失的一方会收到此中断。

**Slave模式**：Slave不发起START，不会参与仲裁，完全不需要。

**Master模式**：只有在多主机系统中才需要。单主机系统不需要。

## 5.6 NAKIE — NACK接收中断

**功能**：当接收到NACK信号时触发EEI0中断。

**Slave模式下可选的原因**：

在Slave Transmit模式下，即使不开NAKIE，流程也能正常完成：

1. Slave发完所有字节 → `g_riic0_tx_count == 0` → 状态 → `_12_SENDS_END`
2. TEI0中断触发 → 状态 → `_13_SENDS_STOP`
3. Master发NACK → **NAKIE=0，不触发EEI0**
4. Master发STOP → SPIE触发EEI0 → 状态`_13` → `callback_transmitend()` ✓

STOP条件最终还是会触发回调。NAKIE只是提前处理NACK，提高响应速度，但不是必须的。

**Master模式**：Master发完数据后slave可能NACK（如slave buffer满），此时需要NAKIE来检测NACK并决定重传或停止。

# 六、实验测试结果

## 6.1 测试：Master Write + Read交替通信

Master每3秒循环：Write 5字节 → 延时100ms → Read 5字节 → 延时3s

### Slave串口输出

```
helloworld
SCI/SCIF Asynchronous Mode | Code Generator | 1.12.0
RX: aa bb cc dd ee
TX: 00 11 22 33 45
RX: aa bb cc dd ef
TX: 00 11 22 33 46
RX: aa bb cc dd f0
TX: 00 11 22 33 47
```

### Master串口输出

```
helloworld!
hi2c1 write:
aa bb cc dd ee 
hi2c1 read:
00 11 22 33 45 
hi2c1 write:
aa bb cc dd ef 
hi2c1 read:
00 11 22 33 46 
hi2c1 write:
aa bb cc dd f0 
hi2c1 read:
00 11 22 33 47 
```

### 通信流程分析

```
┌─────────────────────────────────────────────────────────────────┐
│  第1轮                                                            │
│  Master Write:  aa bb cc dd ee  →  Slave RX: aa bb cc dd ee      │
│  Master Read:   ← 00 11 22 33 45  ←  Slave TX: 00 11 22 33 45  │
├─────────────────────────────────────────────────────────────────┤
│  第2轮                                                            │
│  Master Write:  aa bb cc dd ef  →  Slave RX: aa bb cc dd ef      │
│  Master Read:   ← 00 11 22 33 46  ←  Slave TX: 00 11 22 33 46  │
├─────────────────────────────────────────────────────────────────┤
│  第3轮                                                            │
│  Master Write:  aa bb cc dd f0  →  Slave RX: aa bb cc dd f0      │
│  Master Read:   ← 00 11 22 33 47  ←  Slave TX: 00 11 22 33 47  │
└─────────────────────────────────────────────────────────────────┘
```

- **TX数据**：Slave收到Write后，将每个字节+1作为Read应答
- **第4字节递增**：Master每次Write的第4字节(0xEE→0xEF→0xF0)递增，Slave应答的第4字节(0x45→0x46→0x47)也对应递增
- **结果**：通过 ✓

# 七、关键代码文件说明

| 文件 | 说明 |
|------|------|
| `rx23e_i2c_slave.c` | 主程序，包含main循环、标志位处理、模式切换 |
| `Config_RIIC0.c` | SMC生成的RIIC0初始化和收发函数 |
| `Config_RIIC0_user.c` | SMC生成的中断处理函数（需修改回调部分） |
| `Config_RIIC0.h` | RIIC0函数声明 |

# 八、总结

1. 使用Smart Configurator生成RIIC Slave代码时，**必须勾选STIE和SPIE中断使能**，否则slave无法正常通信
2. TMOIE建议开启作为总线保护，NAKIE可选，ALIE在单主机slave模式下不需要
