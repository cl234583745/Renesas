################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/Config_DSAD0/Config_DSAD0.c \
../src/smc_gen/Config_DSAD0/Config_DSAD0_user.c 

COMPILER_OBJS += \
src/smc_gen/Config_DSAD0/Config_DSAD0.obj \
src/smc_gen/Config_DSAD0/Config_DSAD0_user.obj 

C_DEPS += \
src/smc_gen/Config_DSAD0/Config_DSAD0.d \
src/smc_gen/Config_DSAD0/Config_DSAD0_user.d 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/Config_DSAD0/%.obj: ../src/smc_gen/Config_DSAD0/%.c src/smc_gen/Config_DSAD0/Compiler.sub
	@echo 'Scanning and building file: $<'
	@echo src\smc_gen\Config_DSAD0\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\smc_gen\Config_DSAD0\cDepSubCommand.tmp"
	ccrx -subcommand="src\smc_gen\Config_DSAD0\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)" -MT="$(@:%.d=%.obj)" -MT="$(@:%.obj=%.d)" "$<"
	@echo src\smc_gen\Config_DSAD0\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\smc_gen\Config_DSAD0\cSubCommand.tmp"
	ccrx -subcommand="src\smc_gen\Config_DSAD0\cSubCommand.tmp" "$<"


