################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/src/clock.c \
../rt-thread/src/components.c \
../rt-thread/src/device.c \
../rt-thread/src/idle.c \
../rt-thread/src/ipc.c \
../rt-thread/src/irq.c \
../rt-thread/src/kservice.c \
../rt-thread/src/mem.c \
../rt-thread/src/mempool.c \
../rt-thread/src/object.c \
../rt-thread/src/scheduler.c \
../rt-thread/src/thread.c \
../rt-thread/src/timer.c 

OBJS += \
./rt-thread/src/clock.o \
./rt-thread/src/components.o \
./rt-thread/src/device.o \
./rt-thread/src/idle.o \
./rt-thread/src/ipc.o \
./rt-thread/src/irq.o \
./rt-thread/src/kservice.o \
./rt-thread/src/mem.o \
./rt-thread/src/mempool.o \
./rt-thread/src/object.o \
./rt-thread/src/scheduler.o \
./rt-thread/src/thread.o \
./rt-thread/src/timer.o 

C_DEPS += \
./rt-thread/src/clock.d \
./rt-thread/src/components.d \
./rt-thread/src/device.d \
./rt-thread/src/idle.d \
./rt-thread/src/ipc.d \
./rt-thread/src/irq.d \
./rt-thread/src/kservice.d \
./rt-thread/src/mem.d \
./rt-thread/src/mempool.d \
./rt-thread/src/object.d \
./rt-thread/src/scheduler.d \
./rt-thread/src/thread.d \
./rt-thread/src/timer.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/src/%.o: ../rt-thread/src/%.c
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"C:\Users\85953\Desktop\CookGPT\CookGPT" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\applications" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\board\CubeMX_Config\Inc" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\board" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\libraries\Board_Drivers\lcd" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\libraries\Board_Drivers" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\libraries\HAL_Drivers\CMSIS\Include" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\libraries\HAL_Drivers\config" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\libraries\HAL_Drivers\drv_flash" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\libraries\HAL_Drivers" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\libraries\STM32F4xx_HAL\CMSIS\Device\ST\STM32F4xx\Include" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\libraries\STM32F4xx_HAL\STM32F4xx_HAL_Driver\Inc" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\packages\aht10-v2.1.0" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\packages\cJSON-v1.7.17" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\packages\netutils-latest\ntp" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\packages\rw007-v2.0.1\inc" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\packages\rw007-v2.0.1" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\packages\webclient-v2.2.0\inc" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\drivers\include" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\drivers\sensors" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\drivers\spi\sfud\inc" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\drivers\spi" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\drivers\wlan" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\finsh" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\libc\compilers\common\include" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\libc\compilers\newlib" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\libc\posix\io\poll" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\libc\posix\io\stdio" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\libc\posix\ipc" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\net\lwip\lwip-2.0.3\src\include\netif" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\net\lwip\lwip-2.0.3\src\include" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\net\lwip\port" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\net\netdev\include" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\net\sal\impl" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\net\sal\include\socket\sys_socket" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\net\sal\include\socket" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\net\sal\include" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\include" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\libcpu\arm\common" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\libcpu\arm\cortex-m4" -include"C:\Users\85953\Desktop\CookGPT\CookGPT\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

