################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/r_rtd_api.c \
../src/r_sensor_common_api.c \
../src/rx23ea_local_board_lab.c 

COMPILER_OBJS += \
src/r_rtd_api.obj \
src/r_sensor_common_api.obj \
src/rx23ea_local_board_lab.obj 

C_DEPS += \
src/r_rtd_api.d \
src/r_sensor_common_api.d \
src/rx23ea_local_board_lab.d 

# Each subdirectory must supply rules for building sources it contributes
src/%.obj: ../src/%.c src/Compiler.sub
	@echo 'Scanning and building file: $<'
	@echo src\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\cDepSubCommand.tmp"
	ccrx -subcommand="src\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)" -MT="$(@:%.d=%.obj)" -MT="$(@:%.obj=%.d)" "$<"
	@echo src\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\cSubCommand.tmp"
	ccrx -subcommand="src\cSubCommand.tmp" "$<"


