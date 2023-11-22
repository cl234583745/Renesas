################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/Config_CMT0/Config_CMT0.c \
../src/smc_gen/Config_CMT0/Config_CMT0_user.c 

COMPILER_OBJS += \
src/smc_gen/Config_CMT0/Config_CMT0.obj \
src/smc_gen/Config_CMT0/Config_CMT0_user.obj 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/Config_CMT0/%.obj: ../src/smc_gen/Config_CMT0/%.c 
	@echo 'Building file: $<'
	@echo src\smc_gen\Config_CMT0\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\smc_gen\Config_CMT0\cSubCommand.tmp"
	ccrx -subcommand="src\smc_gen\Config_CMT0\cSubCommand.tmp" "$<"


