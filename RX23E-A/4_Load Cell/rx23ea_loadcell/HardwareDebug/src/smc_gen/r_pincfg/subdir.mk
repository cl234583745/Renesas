################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/r_pincfg/Pin.c 

COMPILER_OBJS += \
src/smc_gen/r_pincfg/Pin.obj 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/r_pincfg/%.obj: ../src/smc_gen/r_pincfg/%.c 
	@echo 'Building file: $<'
	@echo src\smc_gen\r_pincfg\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\smc_gen\r_pincfg\cSubCommand.tmp"
	ccrx -subcommand="src\smc_gen\r_pincfg\cSubCommand.tmp" "$<"


