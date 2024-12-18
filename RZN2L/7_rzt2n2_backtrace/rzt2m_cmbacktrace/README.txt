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