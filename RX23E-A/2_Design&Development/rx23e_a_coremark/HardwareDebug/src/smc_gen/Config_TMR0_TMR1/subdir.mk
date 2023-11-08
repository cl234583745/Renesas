################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/Config_TMR0_TMR1/Config_TMR0_TMR1.c \
../src/smc_gen/Config_TMR0_TMR1/Config_TMR0_TMR1_user.c 

COMPILER_OBJS += \
src/smc_gen/Config_TMR0_TMR1/Config_TMR0_TMR1.obj \
src/smc_gen/Config_TMR0_TMR1/Config_TMR0_TMR1_user.obj 

C_DEPS += \
src/smc_gen/Config_TMR0_TMR1/Config_TMR0_TMR1.d \
src/smc_gen/Config_TMR0_TMR1/Config_TMR0_TMR1_user.d 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/Config_TMR0_TMR1/%.obj: ../src/smc_gen/Config_TMR0_TMR1/%.c 
	@echo 'Scanning and building file: $<'
	ccrx -subcommand="src\smc_gen\Config_TMR0_TMR1\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)" -MT="$(@:%.d=%.obj)" -MT="$(@:%.obj=%.d)" "$<"
	ccrx -subcommand="src\smc_gen\Config_TMR0_TMR1\cSubCommand.tmp" "$<"


