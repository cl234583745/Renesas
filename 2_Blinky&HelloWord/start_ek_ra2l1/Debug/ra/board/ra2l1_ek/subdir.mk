################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ra/board/ra2l1_ek/board_init.c \
../ra/board/ra2l1_ek/board_leds.c 

C_DEPS += \
./ra/board/ra2l1_ek/board_init.d \
./ra/board/ra2l1_ek/board_leds.d 

OBJS += \
./ra/board/ra2l1_ek/board_init.o \
./ra/board/ra2l1_ek/board_leds.o 

SREC += \
start.srec 

MAP += \
start.map 


# Each subdirectory must supply rules for building sources it contributes
ra/board/ra2l1_ek/%.o: ../ra/board/ra2l1_ek/%.c
	$(file > $@.in,-mcpu=cortex-m23 -mthumb -O2 -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wunused -Wuninitialized -Wall -Wextra -Wmissing-declarations -Wconversion -Wpointer-arith -Wshadow -Wlogical-op -Waggregate-return -Wfloat-equal  -g -gdwarf-4 -D_RA_CORE=CM23 -D_RENESAS_RA_ -I"E:/e2s_workspace/start/src" -I"E:/e2s_workspace/start/ra/fsp/inc" -I"E:/e2s_workspace/start/ra/fsp/inc/api" -I"E:/e2s_workspace/start/ra/fsp/inc/instances" -I"E:/e2s_workspace/start/ra/arm/CMSIS_5/CMSIS/Core/Include" -I"E:/e2s_workspace/start/ra_gen" -I"E:/e2s_workspace/start/ra_cfg/fsp_cfg/bsp" -I"E:/e2s_workspace/start/ra_cfg/fsp_cfg" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" -x c "$<")
	@echo Building file: $< && arm-none-eabi-gcc @"$@.in"

