################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables
C_SRCS += \
../src/r_dsp/r_dsp_iirbiquad.c 

COMPILER_OBJS += \
src/r_dsp/r_dsp_iirbiquad.obj 

C_DEPS += \
src/r_dsp/r_dsp_iirbiquad.d 

# Each subdirectory must supply rules for building sources it contributes
src/r_dsp/%.obj: ../src/r_dsp/%.c src/r_dsp/Compiler.sub
	@echo 'Scanning and building file: $<'
	@echo src\r_dsp\cDepSubCommand.tmp=
	@sed -e "s/^/ /" "src\r_dsp\cDepSubCommand.tmp"
	ccrx -subcommand="src\r_dsp\cDepSubCommand.tmp" -output=dep="$(@:%.obj=%.d)" -MT="$(@:%.d=%.obj)" -MT="$(@:%.obj=%.d)" "$<"
	@echo src\r_dsp\cSubCommand.tmp=
	@sed -e "s/^/ /" "src\r_dsp\cSubCommand.tmp"
	ccrx -subcommand="src\r_dsp\cSubCommand.tmp" "$<"


