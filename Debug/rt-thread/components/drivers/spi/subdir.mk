################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/components/drivers/spi/spi_core.c \
../rt-thread/components/drivers/spi/spi_dev.c 

OBJS += \
./rt-thread/components/drivers/spi/spi_core.o \
./rt-thread/components/drivers/spi/spi_dev.o 

C_DEPS += \
./rt-thread/components/drivers/spi/spi_core.d \
./rt-thread/components/drivers/spi/spi_dev.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/components/drivers/spi/%.o: ../rt-thread/components/drivers/spi/%.c
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O3 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\applications" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\board\CubeMX_Config\Core\Inc" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\board\port" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\board" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\STM32H7xx_HAL\CMSIS\Device\ST\STM32H7xx\Include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\STM32H7xx_HAL\CMSIS\Include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\STM32H7xx_HAL\STM32H7xx_HAL_Driver\Inc" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\drivers\include\config" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\drivers\include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\drivers" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\dfs\filesystems\devfs" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\dfs\include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\drivers\include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\drivers\spi" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\finsh" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\libc\compilers\newlib" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\libcpu\arm\common" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\libcpu\arm\cortex-m7" -include"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

