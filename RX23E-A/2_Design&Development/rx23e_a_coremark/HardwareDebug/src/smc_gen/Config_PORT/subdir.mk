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

C_DEPS += \
src/smc_gen/Config_PORT/Config_PORT.d \
src/smc_gen/Config_PORT/Config_PORT_user.d 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/Config_PORT/%.obj: ../src/smc_gen/Config_PORT/%.c 
	@echo 'Scanning and building file: $<'
	ccrx -subcommand="src\smc_gen\Config_PORT\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)" -MT="$(@:%.d=%.obj)" -MT="$(@:%.obj=%.d)" "$<"
	ccrx -subcommand="src\smc_gen\Config_PORT\cSubCommand.tmp" "$<"


