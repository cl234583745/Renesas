################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/Config_AFE/Config_AFE.c \
../src/smc_gen/Config_AFE/Config_AFE_user.c 

COMPILER_OBJS += \
src/smc_gen/Config_AFE/Config_AFE.obj \
src/smc_gen/Config_AFE/Config_AFE_user.obj 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/Config_AFE/%.obj: ../src/smc_gen/Config_AFE/%.c 
	@echo 'Building file: $<'
	@echo src\smc_gen\Config_AFE\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\smc_gen\Config_AFE\cSubCommand.tmp"
	ccrx -subcommand="src\smc_gen\Config_AFE\cSubCommand.tmp" "$<"


