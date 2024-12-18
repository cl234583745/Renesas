/*
 * rzn2_rzt2_backtrace.c
 *
 *  Created on: 2024年12月17日
 *      Author: Jerry.Chen jingjun Mo
 */
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>

#define BSP_MODE_MASK                       (0x1F)       /* Bit mask for mode bits in CPSR */
#define BSP_SVC_MODE                        (0x13)       /* Supervisor mode */
#define cmb_println(...)               printf(__VA_ARGS__);
#define MAX_DEPTH                      32

extern const int __SvcStackBase;
extern const int __SvcStackLimit;
extern const int _text_start;
extern const int _text_end;

static bool disassembly_ins_is_bl_blx(uint32_t addr);
static void print_backtrace(uint32_t sp);
static uint32_t read_cpsr(void);
static void backtrace_fault(uint32_t fault_handler_lr, uint32_t fault_handler_sp);

static uint32_t main_stack_start_addr = 0;
static uint32_t main_stack_size = 0;
static uint32_t code_start_addr = 0;
static uint32_t code_size = 0;



/* check the disassembly instruction is 'BL' or 'BLX' */
static bool disassembly_ins_is_bl_blx(uint32_t addr) {
    uint16_t ins1 = *((uint16_t *)addr);
    uint16_t ins2 = *((uint16_t *)(addr + 2));

#define BL_INS_MASK         0xF800
#define BL_INS_HIGH         0xF800
#define BL_INS_LOW          0xF000
#define BLX_INX_MASK        0xFF00
#define BLX_INX             0x4700

    if ((ins2 & BL_INS_MASK) == BL_INS_HIGH && (ins1 & BL_INS_MASK) == BL_INS_LOW) {
        return true;
    } else if ((ins2 & BLX_INX_MASK) == BLX_INX) {
        return true;
    } else {
        return false;
    }
}
/*
 * print_backtrace
 */
static void print_backtrace(uint32_t sp)
{
    static int deep = 0;
    static uint32_t pc, lr;
    static uint32_t addr[MAX_DEPTH];
    uint32_t stack_start_addr = main_stack_start_addr;
    uint32_t stack_size = main_stack_size;

    cmb_println("====== stack data information ======\n");
    cmb_println("line\taddress\tvalue\n");
    uint8_t line = 1;
    for (uint32_t temp = stack_start_addr + stack_size; temp >= sp; temp -= sizeof(uint32_t))
    //for (uint32_t temp = sp; temp <= stack_start_addr + stack_size; temp += sizeof(uint32_t))
    {
        cmb_println("%d\t%lx\t%lx\n", line++, temp, *((uint32_t *)temp));
    }cmb_println("\n");

    for (; sp < stack_start_addr + stack_size; sp += sizeof(uint32_t)) {

        lr = *((uint32_t *)sp);
        pc = lr - sizeof(uint32_t);
        //cmb_println("lr=%lx, pc=%lx", lr, pc);
        if (pc % 2 == 0)
            continue;

        pc--;

        if (pc >= code_start_addr && pc <= code_start_addr + code_size) {
            if (disassembly_ins_is_bl_blx(pc)) {
                if (deep >= MAX_DEPTH)
                    break;
                addr[deep] = lr;
                //cmb_println("addr[%d]=%lx, lr=%lx", deep, addr[deep], lr);
                deep++;

            }
        }
    }

    //./addr2line.exe -e rzt2m_cmbacktrace.elf -afpiC 8cd 889 703 6e5 6c7
    //python -u backtrace.py rzt2m_cmbacktrace.elf 8cd 889 703 6e5 6c7
    cmb_println("====== result information ======\n");
    cmb_println("addr2line.exe -e rzt2m_cmbacktrace.elf -afpiC 8cd 889 703 6e5 6c7\n");
    cmb_println("python -u backtrace.py rzt2m_cmbacktrace.elf 8cd 889 703 6e5 6c7\n");
    while (deep > 0) {
        deep--;
        cmb_println("%lx ", addr[deep]);
    }cmb_println("\n\n\n");
}
/*
 * backtrace_fault
 */
static void backtrace_fault(uint32_t fault_handler_lr, uint32_t fault_handler_sp)
{
    cmb_println("date:%s\ntime:%s\nfile:%s\nfunc:%s,line:%d\nhello world!\n", __DATE__, __TIME__, __FILE__, __FUNCTION__, __LINE__);
    cmb_println("current:lr=%lx,sp=%lx\n", fault_handler_lr, fault_handler_sp);
    (void)fault_handler_lr;
    uint32_t stack_pointer = fault_handler_sp;

    main_stack_start_addr = (uint32_t)((uint32_t *) &__SvcStackBase);
    main_stack_size = (uint32_t)((uint32_t *) &__SvcStackLimit) - main_stack_start_addr;
    code_start_addr = (uint32_t)((uint32_t *) &_text_start);
    code_size = (uint32_t)((uint32_t *) &_text_end) - code_start_addr;

    cmb_println("====== Main stack information ======\n");
    cmb_println("main_stack_start_addr:%lx, main_stack_size:%lx\n", main_stack_start_addr, main_stack_size);
    cmb_println("====== code addr information ======\n");
    cmb_println("code_start_addr=%lx, code_size=%lx\n", code_start_addr, code_size);

    cmb_println("====== sp information ======\n");
    cmb_println("stack_pointer: %lx\n", stack_pointer);
    print_backtrace(stack_pointer);
}
/*
 *
 */
static uint32_t read_cpsr(void) {
    uint32_t cpsr;
    __asm__ volatile (
        "mrs %0, cpsr\n"
        : "=r" (cpsr)
    );
    return cpsr;
}
/*
 * rzt2_rzn2_backtrace
 */
void rzt2_rzn2_backtrace(void);
void rzt2_rzn2_backtrace(void)
{
    uint32_t reg_cpsr = read_cpsr();
    cmb_println("====== rzt2_rzn2_backtrace ======\n");
    cmb_println("Default_Handler:reg_cpsr=%lx\n", reg_cpsr);

    __asm volatile (
        "cpsr_save2:                               \n"
        "    MRS   r0, CPSR                       \n" /* Original PSR value */
        "    BIC   r0, r0, %[bsp_mode_mask]       \n" /* Clear the mode bits */
        "    ORR   r0, r0, %[bsp_svc_mode]        \n" /* Set SVC mode bits */
        "    MSR   CPSR, r0                   \n"
        ::[bsp_mode_mask] "i" (BSP_MODE_MASK), [bsp_svc_mode] "i" (BSP_SVC_MODE) : "memory");

    reg_cpsr = read_cpsr();
    cmb_println("Set SVC mode bits:reg_cpsr=%lx\n", reg_cpsr);


    volatile uint32_t lr_val, sp_val;

    __asm__ volatile (
        "mov %[lr], lr \n"
        "mov %[sp], sp \n"
        : [lr] "=r" (lr_val), [sp] "=r" (sp_val) // Using named outputs for better readability
        : // No inputs
        : "memory" // Only declare 'memory' since 'lr' and 'sp' are not modified
    );

    while(1)
    {
        backtrace_fault(lr_val, sp_val);

#if 1 // Software loops are only needed 3s
     __asm volatile (
         " mov r0, #0 \n"
         " movw r1, #0xf07f \n"
         " movt r1, #0x5fa \n"
         "software_loop: \n"
         " adds r0, #1 \n"
         " cmp r0, r1 \n"
         " bne software_loop \n"
         ::: "memory");
#endif
    }

}
