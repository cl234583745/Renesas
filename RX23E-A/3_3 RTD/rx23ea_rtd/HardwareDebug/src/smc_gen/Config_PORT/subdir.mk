################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/Config_PORT/Config_PORT.c \
../src/smc_gen/Config_PORT/Config_PORT_user.c 

COMPILER_OBJS += \
src/smc_gen/Config_PORT/Config_PORT.obj \
src/smc_gen/Config_PORT/Config_PORT_user.obj 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/Config_PORT/%.obj: ../src/smc_gen/Config_PORT/%.c 
	@echo 'Building file: $<'
	@echo src\smc_gen\Config_PORT\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\smc_gen\Config_PORT\cSubCommand.tmp"
	ccrx -subcommand="src\smc_gen\Config_PORT\cSubCommand.tmp" "$<"


