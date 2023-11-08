################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/coremark/core_list_join.c \
../src/coremark/core_main.c \
../src/coremark/core_matrix.c \
../src/coremark/core_portme.c \
../src/coremark/core_state.c \
../src/coremark/core_util.c 

COMPILER_OBJS += \
src/coremark/core_list_join.obj \
src/coremark/core_main.obj \
src/coremark/core_matrix.obj \
src/coremark/core_portme.obj \
src/coremark/core_state.obj \
src/coremark/core_util.obj 

C_DEPS += \
src/coremark/core_list_join.d \
src/coremark/core_main.d \
src/coremark/core_matrix.d \
src/coremark/core_portme.d \
src/coremark/core_state.d \
src/coremark/core_util.d 

# Each subdirectory must supply rules for building sources it contributes
src/coremark/%.obj: ../src/coremark/%.c 
	@echo 'Scanning and building file: $<'
	ccrx -subcommand="src\coremark\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)" -MT="$(@:%.d=%.obj)" -MT="$(@:%.obj=%.d)" "$<"
	ccrx -subcommand="src\coremark\cSubCommand.tmp" "$<"


