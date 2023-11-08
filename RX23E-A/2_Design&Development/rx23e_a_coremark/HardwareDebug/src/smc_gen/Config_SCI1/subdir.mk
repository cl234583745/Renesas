################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/Config_SCI1/Config_SCI1.c \
../src/smc_gen/Config_SCI1/Config_SCI1_user.c 

COMPILER_OBJS += \
src/smc_gen/Config_SCI1/Config_SCI1.obj \
src/smc_gen/Config_SCI1/Config_SCI1_user.obj 

C_DEPS += \
src/smc_gen/Config_SCI1/Config_SCI1.d \
src/smc_gen/Config_SCI1/Config_SCI1_user.d 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/Config_SCI1/%.obj: ../src/smc_gen/Config_SCI1/%.c 
	@echo 'Scanning and building file: $<'
	ccrx -subcommand="src\smc_gen\Config_SCI1\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)" -MT="$(@:%.d=%.obj)" -MT="$(@:%.obj=%.d)" "$<"
	ccrx -subcommand="src\smc_gen\Config_SCI1\cSubCommand.tmp" "$<"


