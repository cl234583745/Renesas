# Renesas
Renesas
瑞萨MCU/MPU快速上手视频笔记 

分享自己通过瑞萨官网RA资料的学习，快速上手RA MCU。主要引用官方资料和例程，注明出处，创建自己的git仓库，更加“接地气”的分享RA&e2studio的学习过程。

RA2L1官方文档和例程、软件打包下载
链接：https://pan.baidu.com/s/1SbEbdG1g8sx4PwC8b0RMMA 
提取码：jfrm

bilibili：嵌入式科普
https://space.bilibili.com/417060922/video

公众号：嵌入式科普

Qqun：33718742

西瓜shipin：嵌入式科普
https://www.ixigua.com/home/818516314816996

gitee：
https://gitee.com/292812832/Renesas

CSDN：
https://blog.csdn.net/cl234583745?type=blog



理解了。下面是一个更完整的Mermaid流程图，展示了在不同开发环境下各种编译工具的逻辑和包含关系：

```mermaid
graph LR
A[ARM架构] -- 软件开发 --> B{GNU}
B -- 维护 --> C{GNU工具链}
C -- 包含 --> D[Arm GNU Toolchain]
C -- 包含 --> E{GCC}
D -- 包含 --> F[arm-none-eabi-gcc: 用于编译C代码]
D -- 包含 --> G[arm-none-eabi-g++: 用于编译C++代码]
D -- 包含 --> H[arm-none-eabi-as: 用于汇编代码]
D -- 包含 --> I[arm-none-eabi-ld: 用于链接目标文件]
D -- 包含 --> J[arm-none-eabi-objcopy: 用于目标文件转换]
D -- 包含 --> K[arm-none-eabi-objdump: 用于目标文件反汇编]
D -- 包含 --> L[arm-none-eabi-size: 用于计算目标文件大小]
D -- 包含 --> M[arm-none-eabi-nm: 用于显示目标文件符号表]
D -- 包含 --> N[arm-none-eabi-addr2line: 用于地址转换]
D -- 包含 --> O[arm-none-eabi-strings: 用于打印目标文件中的字符串]
D -- 包含 --> P[arm-none-eabi-ar: 用于创建、修改和提取静态库成员]
D -- 包含 --> Q[arm-none-eabi-ranlib: 用于生成静态库的索引]
E -- 包含 --> R[gcc: GNU编译器，用于编译C代码]
E -- 包含 --> S[g++: GNU编译器，用于编译C++代码]
E -- 包含 --> T[gcc-ar: 用于创建、修改和提取静态库成员]
E -- 包含 --> U[gcc-nm: 用于显示目标文件符号表]
E -- 包含 --> V[gcc-ranlib: 用于生成静态库的索引]
E -- 包含 --> W[gcc-objcopy: 用于目标文件转换]
E -- 包含 --> X[gcc-objdump: 用于目标文件反汇编]
E -- 包含 --> Y[gcc-size: 用于计算目标文件大小]
E -- 包含 --> Z[gcc-nm: 用于显示目标文件符号表]

A[ARM架构] -- 软件开发 --> AA{Eclipse}
AA -- 集成 --> AB{MDK}
AA -- 集成 --> AC{IAR}
AA -- 集成 --> AD{GNU工具链}
AB -- 包含 --> AE[Arm Compiler: Keil MDK中的编译器]
AB -- 包含 --> AF[Arm Linker: Keil MDK中的链接器]
AB -- 包含 --> AG[Arm Assembler: Keil MDK中的汇编器]
AB -- 包含 --> AH[Arm librarian: Keil MDK中的库管理器]
AC -- 包含 --> AI[IAR C/C++ Compiler: IAR中的编译器]
AC -- 包含 --> AJ[IAR Assembler: IAR中的汇编器]
AC -- 包含 --> AK[IAR Linker: IAR中的链接器]
AC -- 包含 --> AL[IAR librarian: IAR中的库管理器]
AD -- 包含 --> AM{Arm GNU Toolchain}
AD -- 包含 --> AN{GCC}

A[ARM架构] -- 软件开发 --> BA{Windows}
BA -- 集成 --> BB{Keil}
BA -- 集成 --> BC{IAR}
BA -- 集成 --> BD{Visual Studio}
BB -- 包含 --> BE[Arm Compiler: Keil中的编译器]
BB -- 包含 --> BF[Arm Linker: Keil中的链接器]
BB -- 包含 --> BG[Arm Assembler: Keil中的汇编器]
BB -- 包含 --> BH[Arm librarian: Keil中的库管理器]
BC -- 包含 --> BI[IAR C/C++ Compiler: IAR中的编译器]
BC -- 包含 --> BJ[IAR Assembler: IAR中的汇编器]
BC -- 包含 --> BK[IAR Linker: IAR中的链接器]
BC -- 包含 --> BL[IAR librarian: IAR中的库管理器]
BD -- 包含 --> BM[MSVC: Visual Studio中的编译器]
BD -- 包含 --> BN[MSVC Linker: Visual Studio中的链接器]
BD -- 包含 --> BO[MSVC librarian: Visual Studio中的库管理器]
```

以上示例中，我们展示了在不同开发环境下各种编译工具的逻辑和包含关系，包括Eclipse集成的MDK、IAR和GNU工具链，以及Windows环境下的Keil、IAR和Visual Studio。这样的流程图可以帮助您更清晰地了解各种环境下的编译工具关系。