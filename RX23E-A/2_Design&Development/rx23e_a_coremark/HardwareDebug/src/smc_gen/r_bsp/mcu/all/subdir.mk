################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/r_bsp/mcu/all/dbsct.c \
../src/smc_gen/r_bsp/mcu/all/lowlvl.c \
../src/smc_gen/r_bsp/mcu/all/lowsrc.c \
../src/smc_gen/r_bsp/mcu/all/mcu_locks.c \
../src/smc_gen/r_bsp/mcu/all/r_bsp_common.c \
../src/smc_gen/r_bsp/mcu/all/r_bsp_cpu.c \
../src/smc_gen/r_bsp/mcu/all/r_bsp_interrupts.c \
../src/smc_gen/r_bsp/mcu/all/r_bsp_locking.c \
../src/smc_gen/r_bsp/mcu/all/r_bsp_mcu_startup.c \
../src/smc_gen/r_bsp/mcu/all/r_bsp_software_interrupt.c \
../src/smc_gen/r_bsp/mcu/all/r_rx_intrinsic_functions.c \
../src/smc_gen/r_bsp/mcu/all/resetprg.c \
../src/smc_gen/r_bsp/mcu/all/sbrk.c 

COMPILER_OBJS += \
src/smc_gen/r_bsp/mcu/all/dbsct.obj \
src/smc_gen/r_bsp/mcu/all/lowlvl.obj \
src/smc_gen/r_bsp/mcu/all/lowsrc.obj \
src/smc_gen/r_bsp/mcu/all/mcu_locks.obj \
src/smc_gen/r_bsp/mcu/all/r_bsp_common.obj \
src/smc_gen/r_bsp/mcu/all/r_bsp_cpu.obj \
src/smc_gen/r_bsp/mcu/all/r_bsp_interrupts.obj \
src/smc_gen/r_bsp/mcu/all/r_bsp_locking.obj \
src/smc_gen/r_bsp/mcu/all/r_bsp_mcu_startup.obj \
src/smc_gen/r_bsp/mcu/all/r_bsp_software_interrupt.obj \
src/smc_gen/r_bsp/mcu/all/r_rx_intrinsic_functions.obj \
src/smc_gen/r_bsp/mcu/all/resetprg.obj \
src/smc_gen/r_bsp/mcu/all/sbrk.obj 

C_DEPS += \
src/smc_gen/r_bsp/mcu/all/dbsct.d \
src/smc_gen/r_bsp/mcu/all/lowlvl.d \
src/smc_gen/r_bsp/mcu/all/lowsrc.d \
src/smc_gen/r_bsp/mcu/all/mcu_locks.d \
src/smc_gen/r_bsp/mcu/all/r_bsp_common.d \
src/smc_gen/r_bsp/mcu/all/r_bsp_cpu.d \
src/smc_gen/r_bsp/mcu/all/r_bsp_interrupts.d \
src/smc_gen/r_bsp/mcu/all/r_bsp_locking.d \
src/smc_gen/r_bsp/mcu/all/r_bsp_mcu_startup.d \
src/smc_gen/r_bsp/mcu/all/r_bsp_software_interrupt.d \
src/smc_gen/r_bsp/mcu/all/r_rx_intrinsic_functions.d \
src/smc_gen/r_bsp/mcu/all/resetprg.d \
src/smc_gen/r_bsp/mcu/all/sbrk.d 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/r_bsp/mcu/all/%.obj: ../src/smc_gen/r_bsp/mcu/all/%.c 
	@echo 'Scanning and building file: $<'
	ccrx -subcommand="src\smc_gen\r_bsp\mcu\all\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)" -MT="$(@:%.d=%.obj)" -MT="$(@:%.obj=%.d)" "$<"
	ccrx -subcommand="src\smc_gen\r_bsp\mcu\all\cSubCommand.tmp" "$<"


