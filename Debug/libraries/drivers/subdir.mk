################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../libraries/drivers/drv_common.c \
../libraries/drivers/drv_dcmi.c \
../libraries/drivers/drv_gpio.c \
../libraries/drivers/drv_lcd.c \
../libraries/drivers/drv_ov7670.c \
../libraries/drivers/drv_sdio.c \
../libraries/drivers/drv_sdram.c \
../libraries/drivers/drv_soft_i2c.c \
../libraries/drivers/drv_spi.c \
../libraries/drivers/drv_usart.c \
../libraries/drivers/drv_wlan.c 

OBJS += \
./libraries/drivers/drv_common.o \
./libraries/drivers/drv_dcmi.o \
./libraries/drivers/drv_gpio.o \
./libraries/drivers/drv_lcd.o \
./libraries/drivers/drv_ov7670.o \
./libraries/drivers/drv_sdio.o \
./libraries/drivers/drv_sdram.o \
./libraries/drivers/drv_soft_i2c.o \
./libraries/drivers/drv_spi.o \
./libraries/drivers/drv_usart.o \
./libraries/drivers/drv_wlan.o 

C_DEPS += \
./libraries/drivers/drv_common.d \
./libraries/drivers/drv_dcmi.d \
./libraries/drivers/drv_gpio.d \
./libraries/drivers/drv_lcd.d \
./libraries/drivers/drv_ov7670.d \
./libraries/drivers/drv_sdio.d \
./libraries/drivers/drv_sdram.d \
./libraries/drivers/drv_soft_i2c.d \
./libraries/drivers/drv_spi.d \
./libraries/drivers/drv_usart.d \
./libraries/drivers/drv_wlan.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/drivers/%.o: ../libraries/drivers/%.c
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O0 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\applications" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\board\CubeMX_Config\Core\Inc" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\board\port" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\board" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\STM32H7xx_HAL\CMSIS\Device\ST\STM32H7xx\Include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\STM32H7xx_HAL\CMSIS\Include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\STM32H7xx_HAL\STM32H7xx_HAL_Driver\Inc" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\drivers\include\config" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\drivers\include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\drivers" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\dfs\filesystems\devfs" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\dfs\include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\drivers\include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\drivers\spi" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\finsh" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\libc\compilers\newlib" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\libcpu\arm\common" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\libcpu\arm\cortex-m7" -include"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

