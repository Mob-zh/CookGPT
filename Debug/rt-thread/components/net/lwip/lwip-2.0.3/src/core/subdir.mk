################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/components/net/lwip/lwip-2.0.3/src/core/def.c \
../rt-thread/components/net/lwip/lwip-2.0.3/src/core/dns.c \
../rt-thread/components/net/lwip/lwip-2.0.3/src/core/inet_chksum.c \
../rt-thread/components/net/lwip/lwip-2.0.3/src/core/init.c \
../rt-thread/components/net/lwip/lwip-2.0.3/src/core/ip.c \
../rt-thread/components/net/lwip/lwip-2.0.3/src/core/memp.c \
../rt-thread/components/net/lwip/lwip-2.0.3/src/core/netif.c \
../rt-thread/components/net/lwip/lwip-2.0.3/src/core/pbuf.c \
../rt-thread/components/net/lwip/lwip-2.0.3/src/core/raw.c \
../rt-thread/components/net/lwip/lwip-2.0.3/src/core/stats.c \
../rt-thread/components/net/lwip/lwip-2.0.3/src/core/sys.c \
../rt-thread/components/net/lwip/lwip-2.0.3/src/core/tcp.c \
../rt-thread/components/net/lwip/lwip-2.0.3/src/core/tcp_in.c \
../rt-thread/components/net/lwip/lwip-2.0.3/src/core/tcp_out.c \
../rt-thread/components/net/lwip/lwip-2.0.3/src/core/timeouts.c \
../rt-thread/components/net/lwip/lwip-2.0.3/src/core/udp.c 

OBJS += \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/def.o \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/dns.o \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/inet_chksum.o \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/init.o \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/ip.o \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/memp.o \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/netif.o \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/pbuf.o \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/raw.o \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/stats.o \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/sys.o \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/tcp.o \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/tcp_in.o \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/tcp_out.o \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/timeouts.o \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/udp.o 

C_DEPS += \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/def.d \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/dns.d \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/inet_chksum.d \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/init.d \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/ip.d \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/memp.d \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/netif.d \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/pbuf.d \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/raw.d \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/stats.d \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/sys.d \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/tcp.d \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/tcp_in.d \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/tcp_out.d \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/timeouts.d \
./rt-thread/components/net/lwip/lwip-2.0.3/src/core/udp.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/components/net/lwip/lwip-2.0.3/src/core/%.o: ../rt-thread/components/net/lwip/lwip-2.0.3/src/core/%.c
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"C:\Users\85953\Desktop\CookGPT\CookGPT" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\applications" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\board\CubeMX_Config\Inc" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\board" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\libraries\Board_Drivers\lcd" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\libraries\Board_Drivers" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\libraries\HAL_Drivers\CMSIS\Include" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\libraries\HAL_Drivers\config" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\libraries\HAL_Drivers\drv_flash" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\libraries\HAL_Drivers" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\libraries\STM32F4xx_HAL\CMSIS\Device\ST\STM32F4xx\Include" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\libraries\STM32F4xx_HAL\STM32F4xx_HAL_Driver\Inc" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\packages\aht10-v2.1.0" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\packages\cJSON-v1.7.17" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\packages\netutils-latest\ntp" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\packages\rw007-v2.0.1\inc" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\packages\rw007-v2.0.1" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\packages\webclient-v2.2.0\inc" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\drivers\include" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\drivers\sensors" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\drivers\spi\sfud\inc" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\drivers\spi" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\drivers\wlan" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\finsh" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\libc\compilers\common\include" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\libc\compilers\newlib" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\libc\posix\io\poll" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\libc\posix\io\stdio" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\libc\posix\ipc" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\net\lwip\lwip-2.0.3\src\include\netif" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\net\lwip\lwip-2.0.3\src\include" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\net\lwip\port" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\net\netdev\include" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\net\sal\impl" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\net\sal\include\socket\sys_socket" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\net\sal\include\socket" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\components\net\sal\include" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\include" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\libcpu\arm\common" -I"C:\Users\85953\Desktop\CookGPT\CookGPT\rt-thread\libcpu\arm\cortex-m4" -include"C:\Users\85953\Desktop\CookGPT\CookGPT\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

