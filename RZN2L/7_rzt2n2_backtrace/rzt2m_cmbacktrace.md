
十四、从0开始卷出一个新项目之瑞萨RZN2L之栈回溯(Default_Handler/hartfault)
===
[toc]
# 一、概述
- 软件开发中常见的比较棘手的问题就是hartfault/Default_Handler/dump，俗称跑飞了。
- 参考cmbacktrace，在瑞萨RZN2L/T2M实现栈回溯，串口打印栈回溯信息。
- 使用add2line或Python打印发生错误函数位置。
- 循环打印栈回溯信息和编译日期，便于追溯bug。

# 二、参考资料
https://github.com/armink/CmBacktrace/tree/master
https://cloud.tencent.com/developer/article/1736680
https://www.segger.com/downloads/application-notes/AN00016
https://stackoverflow.com/questions/47331426/stack-backtrace-for-arm-core-using-gcc-compiler-when-there-is-a-msp-to-psp-swit/50923698#50923698
https://blog.csdn.net/laviolette/article/details/51376751
https://blog.csdn.net/Teminator_/article/details/143177973?ops_request_misc=&request_id=&biz_id=102&utm_term=cmbacktrace&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-0-143177973.142^v100^pc_search_result_base9&spm=1018.2226.3001.4187
https://github.com/RT-Thread-Studio/sdk-bsp-rzn2l-etherkit

# 三、代码

- 1、加入rzn2_rzt2_backtrace.c
- 2、void Default_Handler (void)
{
    extern void rzt2_rzn2_backtrace(void);
    rzt2_rzn2_backtrace();
    while(1);
    /** A error has occurred. The user will need to investigate the cause. Common problems are stack corruption
     *  or use of an invalid pointer. Use the Fault Status window in e2 studio or manually check the fault status
     *  registers for more information.
     */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR(0);
}

# 四、日志
- 循环打印栈回溯信息和编译日期
```
date:Dec 17 2024
time:11:47:45
file:../src/rzn2_rzt2_backtrace.c
func:backtrace_fault,line:110
hello world!
current:lr=807,sp=106480
====== Main stack information ======
main_stack_start_addr:105510, main_stack_size:1000
====== code addr information ======
code_start_addr=100, code_size=84c4
====== sp information ======
stack_pointer: 106480
====== stack data information ======
line	address	value
1	106510	92043202
2	10650c	106510
3	106508	0
4	106504	1020e4
5	106500	78787a8
6	1064fc	1023b5
7	1064f8	106500
8	1064f4	921
9	1064f0	1064f8
10	1064ec	2
11	1064e8	f8
12	1064e4	3e8
13	1064e0	2
14	1064dc	1f4
15	1064d8	40490fda
16	1064d4	9d54
17	1064d0	2
18	1064cc	86
19	1064c8	8d20
20	1064c4	3ef
21	1064c0	1064d0
22	1064bc	1
23	1064b8	8d18
24	1064b4	311
25	1064b0	1064b8
26	1064ac	1
27	1064a8	f8
28	1064a4	2f3
29	1064a0	1064a8
30	10649c	1
31	106498	78787a8
32	106494	2d5
33	106490	106498
34	10648c	e000ed14
35	106488	78fb261
36	106484	1
37	106480	9d6c

====== result information ======
addr2line.exe -e rzt2m_cmbacktrace.elf -afpiC 8cd 889 703 6e5 6c7
python -u backtrace.py rzt2m_cmbacktrace.elf 8cd 889 703 6e5 6c7
921 3ef 311 2f3 2d5 
```

# 五、定位函数调用
```
Jerry.Chen@WTNB-6719 MINGW64 /e/RS_workspace/rzt2m_cmbacktrace/Debug
$ python -u backtrace.py rzt2m_cmbacktrace.elf 8ef 3ef 311 2f3 2d5
elf: rzt2m_cmbacktrace.elf
main --> hal_entry --> func_a --> func_b --> func_c

Jerry.Chen@WTNB-6719 MINGW64 /e/RS_workspace/rzt2m_cmbacktrace/Debug
$ ./addr2line.exe -e rzt2m_cmbacktrace.elf -afpiC 8ef 3ef 311 2f3 2d5
0x000008ef: main at E:\RS_workspace\rzt2m_cmbacktrace\Debug/../rzt_gen/main.c:6
0x000003ef: hal_entry at E:\RS_workspace\rzt2m_cmbacktrace\Debug/../src/hal_entry.c:144
0x00000311: func_a at E:\RS_workspace\rzt2m_cmbacktrace\Debug/../src/hal_entry.c:93
0x000002f3: func_b at E:\RS_workspace\rzt2m_cmbacktrace\Debug/../src/hal_entry.c:87
0x000002d5: func_c at E:\RS_workspace\rzt2m_cmbacktrace\Debug/../src/hal_entry.c:81
```
# 六、README和工具
```
#rzt2/n2 栈回溯
1、加入rzn2_rzt2_backtrace.c
2、void Default_Handler (void)
{
    extern void rzt2_rzn2_backtrace(void);
    rzt2_rzn2_backtrace();
    while(1);
    /** A error has occurred. The user will need to investigate the cause. Common problems are stack corruption
     *  or use of an invalid pointer. Use the Fault Status window in e2 studio or manually check the fault status
     *  registers for more information.
     */
    BSP_CFG_HANDLE_UNRECOVERABLE_ERROR(0);
}
3、addr2line.exe -e rzt2m_cmbacktrace.elf -afpiC 8cd 889 703 6e5 6c7
或python -u backtrace.py rzt2m_cmbacktrace.elf 8cd 889 703 6e5 6c7
4、工具在工程根目录
```