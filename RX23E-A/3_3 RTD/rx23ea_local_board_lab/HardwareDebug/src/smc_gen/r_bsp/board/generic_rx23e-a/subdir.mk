################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/smc_gen/r_bsp/board/generic_rx23e-a/hwsetup.c 

COMPILER_OBJS += \
src/smc_gen/r_bsp/board/generic_rx23e-a/hwsetup.obj 

C_DEPS += \
src/smc_gen/r_bsp/board/generic_rx23e-a/hwsetup.d 

# Each subdirectory must supply rules for building sources it contributes
src/smc_gen/r_bsp/board/generic_rx23e-a/%.obj: ../src/smc_gen/r_bsp/board/generic_rx23e-a/%.c src/smc_gen/r_bsp/board/generic_rx23e-a/Compiler.sub
	@echo 'Scanning and building file: $<'
	@echo src\smc_gen\r_bsp\board\generic_rx23e-a\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\smc_gen\r_bsp\board\generic_rx23e-a\cDepSubCommand.tmp"
	ccrx -subcommand="src\smc_gen\r_bsp\board\generic_rx23e-a\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)" -MT="$(@:%.d=%.obj)" -MT="$(@:%.obj=%.d)" "$<"
	@echo src\smc_gen\r_bsp\board\generic_rx23e-a\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\smc_gen\r_bsp\board\generic_rx23e-a\cSubCommand.tmp"
	ccrx -subcommand="src\smc_gen\r_bsp\board\generic_rx23e-a\cSubCommand.tmp" "$<"


