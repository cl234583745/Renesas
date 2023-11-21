################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/main.c \
../src/r_communication_control_api.c \
../src/r_ring_buffer_control_api.c \
../src/r_rtd_api.c \
../src/r_sensor_common_api.c 

COMPILER_OBJS += \
src/main.obj \
src/r_communication_control_api.obj \
src/r_ring_buffer_control_api.obj \
src/r_rtd_api.obj \
src/r_sensor_common_api.obj 

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c 
	@echo 'Building file: $<'
	@echo src\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\cSubCommand.tmp"
	ccrx -subcommand="src\cSubCommand.tmp" "$<"


