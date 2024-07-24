rzn2l 串口0 dma接收不定长

2023/7/15；fsp2.0.0 e2s2024-04；jerry.chen@wtmec.com

说明：使用gpt0 elc等检测串口0的空闲时间，TEXT_LENGTH是接收最大字节长度

原理：
g_timer0_ctrl.p_reg->GTSSR = GPT_SOURCE_GPT_A;//SCI RX p16-6--->ELC single port0--->LLPP GPT event A(GPT_SOURCE_GPT_A)--->GTSSR(Start Source)
g_timer0_ctrl.p_reg->GTCSR = GPT_SOURCE_GPT_B;//SCI RX p16-6--->ELC single port0--->LLPP GPT event B(GPT_SOURCE_GPT_B)--->GTCSR(clear Source)


xspi0启动:
#if BSP_CFG_C_RUNTIME_INIT

    /* Initialize static constructors */
    bsp_static_constructor_init();
#endif

#if 1 // Software loops are only needed when debugging.
     __asm volatile (
         " mov r0, #0 \n"
         " movw r1, #0xf07f \n"
         " movt r1, #0x2fa \n"
         "software_loop: \n"
         " adds r0, #1 \n"
         " cmp r0, r1 \n"
         " bne software_loop \n"
         ::: "memory");
#endif

