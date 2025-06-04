################################################################################
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include rt-thread/src/subdir.mk
-include rt-thread/libcpu/arm/cortex-m4/subdir.mk
-include rt-thread/libcpu/arm/common/subdir.mk
-include rt-thread/components/net/sal/src/subdir.mk
-include rt-thread/components/net/sal/socket/subdir.mk
-include rt-thread/components/net/sal/impl/subdir.mk
-include rt-thread/components/net/netdev/src/subdir.mk
-include rt-thread/components/net/lwip/port/subdir.mk
-include rt-thread/components/net/lwip/lwip-2.0.3/src/netif/subdir.mk
-include rt-thread/components/net/lwip/lwip-2.0.3/src/core/ipv4/subdir.mk
-include rt-thread/components/net/lwip/lwip-2.0.3/src/core/subdir.mk
-include rt-thread/components/net/lwip/lwip-2.0.3/src/apps/ping/subdir.mk
-include rt-thread/components/net/lwip/lwip-2.0.3/src/api/subdir.mk
-include rt-thread/components/libc/compilers/newlib/subdir.mk
-include rt-thread/components/libc/compilers/common/subdir.mk
-include rt-thread/components/finsh/subdir.mk
-include rt-thread/components/drivers/wlan/subdir.mk
-include rt-thread/components/drivers/spi/sfud/src/subdir.mk
-include rt-thread/components/drivers/spi/subdir.mk
-include rt-thread/components/drivers/serial/subdir.mk
-include rt-thread/components/drivers/sensors/subdir.mk
-include rt-thread/components/drivers/rtc/subdir.mk
-include rt-thread/components/drivers/misc/subdir.mk
-include rt-thread/components/drivers/ipc/subdir.mk
-include rt-thread/components/drivers/i2c/subdir.mk
-include packages/webclient-v2.2.0/src/subdir.mk
-include packages/rw007-v2.0.1/src/subdir.mk
-include packages/rw007-v2.0.1/example/subdir.mk
-include packages/netutils-latest/ntp/subdir.mk
-include packages/cJSON-v1.7.17/subdir.mk
-include packages/aht10-v2.1.0/subdir.mk
-include libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/subdir.mk
-include libraries/STM32F4xx_HAL/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/subdir.mk
-include libraries/STM32F4xx_HAL/CMSIS/Device/ST/STM32F4xx/Source/Templates/subdir.mk
-include libraries/HAL_Drivers/drv_flash/subdir.mk
-include libraries/HAL_Drivers/subdir.mk
-include libraries/Board_Drivers/lcd/subdir.mk
-include build/libraries/STM32F4xx_HAL/STM32F4xx_HAL_Driver/Src/subdir.mk
-include build/libraries/STM32F4xx_HAL/CMSIS/Device/ST/STM32F4xx/Source/Templates/gcc/subdir.mk
-include build/libraries/STM32F4xx_HAL/CMSIS/Device/ST/STM32F4xx/Source/Templates/subdir.mk
-include build/libraries/HAL_Drivers/subdir.mk
-include build/libraries/Board_Drivers/lcd/subdir.mk
-include build/kernel/src/subdir.mk
-include build/kernel/libcpu/arm/cortex-m4/subdir.mk
-include build/kernel/libcpu/arm/common/subdir.mk
-include build/kernel/components/libc/compilers/newlib/subdir.mk
-include build/kernel/components/libc/compilers/common/subdir.mk
-include build/kernel/components/finsh/subdir.mk
-include build/kernel/components/drivers/serial/subdir.mk
-include build/kernel/components/drivers/rtc/subdir.mk
-include build/kernel/components/drivers/misc/subdir.mk
-include build/kernel/components/drivers/ipc/subdir.mk
-include build/kernel/components/drivers/i2c/subdir.mk
-include build/keil/Obj/subdir.mk
-include build/board/CubeMX_Config/Src/subdir.mk
-include build/board/subdir.mk
-include build/applications/subdir.mk
-include board/CubeMX_Config/Src/subdir.mk
-include board/subdir.mk
-include applications/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
rtthread.bin \

SECONDARY_SIZE += \
rtthread.siz \


# 所有目标
all: rtthread.elf secondary-outputs

# 工具调用
rtthread.elf: $(OBJS) $(USER_OBJS)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -T "C:\Users\85953\Desktop\CookGPT\CookGPT\board\linker_scripts\link.lds" -Xlinker --gc-sections -Wl,-Map,"rtthread.map" -Xlinker --cref -o "rtthread.elf" $(OBJS) $(USER_OBJS) $(LIBS)

rtthread.bin: rtthread.elf
	arm-none-eabi-objcopy -O binary "rtthread.elf"  "rtthread.bin"

rtthread.siz: rtthread.elf
	arm-none-eabi-size --format=berkeley "rtthread.elf"

# 其他目标
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) rtthread.elf
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
