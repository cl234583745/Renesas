/*
 * 这个例程是基于rzn2l rsk开发板的phid例程，展示rtos phid通讯 
 * v0.2	2025-1-7	fsp1.3	jerry
 */
1.接电源线、调试线、串口线、usb线到pc
2、xspi模式运行，启动后串口输出日志，HID收发工具可以扫描到VID：2024 PID：0117的设备，连接后可以收发数据
3、该例程合并了串口例程、pcdc例程、phid例程等
4、testPhid_thread_entry线程用来测试双向收发通信
5、usr_driver文件夹为usb协议栈从fsp剥离出来，需要增加3个中断见vector
