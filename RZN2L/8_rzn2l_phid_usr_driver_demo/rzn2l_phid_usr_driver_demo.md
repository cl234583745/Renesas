
十五、从0开始卷出一个新项目之瑞萨RZN2L之PHID例程
===
[toc]
# 一、概述
- RZN2L未提供PHID的例程，这里提供来逻辑PHID和RTOS PHID例程
- USB知识：[嵌入式科普(24)一张图真正看懂USB通信协议]([嵌入式科普(24)一张图真正看懂USB通信协议](https://mp.weixin.qq.com/s/ht_U2p3R6O4_cwm_D7kXyw?token=1946762107&lang=zh_CN))

# 二、README.txt
```
1.接电源线、调试线、串口线、usb线到pc
2、xspi模式运行，启动后串口输出日志，HID收发工具可以扫描到VID：2024 PID：0117的设备，连接后可以收发数据
3、该例程合并了串口例程、pcdc例程、phid例程等
4、testPhid_thread_entry线程用来测试双向收发通信
5、usr_driver文件夹为usb协议栈从fsp剥离出来，需要增加3个中断见vector
```
```
1.接电源线、调试线、串口线、usb线到pc
2、xspi模式运行，启动后串口输出日志，HID收发工具可以扫描到VID：2024 PID：0117的设备，连接后可以收发数据
3、该例程合并了串口例程、pcdc例程、phid例程等
4、可以使用R_USB_Write后等待完成标志位方式封装api；由于R_USB_Read是阻塞方式，可以通过环形队列方式异步接收数据
5、usr_driver文件夹为usb协议栈从fsp剥离出来，需要增加3个中断见vector

```

# 三、总结
- HID不止是键盘鼠标单向通信，也可以用于双向通信