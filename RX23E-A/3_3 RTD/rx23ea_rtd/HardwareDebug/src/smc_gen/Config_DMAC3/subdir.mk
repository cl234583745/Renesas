################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/Config_DMAC3/Config_DMAC3.c \
../src/smc_gen/Config_DMAC3/Config_DMAC3_user.c 

COMPILER_OBJS += \
src/smc_gen/Config_DMAC3/Config_DMAC3.obj \
src/smc_gen/Config_DMAC3/Config_DMAC3_user.obj 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/Config_DMAC3/%.obj: ../src/smc_gen/Config_DMAC3/%.c 
	@echo 'Building file: $<'
	@echo src\smc_gen\Config_DMAC3\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\smc_gen\Config_DMAC3\cSubCommand.tmp"
	ccrx -subcommand="src\smc_gen\Config_DMAC3\cSubCommand.tmp" "$<"


