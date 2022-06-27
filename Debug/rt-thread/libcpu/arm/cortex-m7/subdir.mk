################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/libcpu/arm/cortex-m7/cpu_cache.c \
../rt-thread/libcpu/arm/cortex-m7/cpuport.c 

S_UPPER_SRCS += \
../rt-thread/libcpu/arm/cortex-m7/context_gcc.S 

OBJS += \
./rt-thread/libcpu/arm/cortex-m7/context_gcc.o \
./rt-thread/libcpu/arm/cortex-m7/cpu_cache.o \
./rt-thread/libcpu/arm/cortex-m7/cpuport.o 

S_UPPER_DEPS += \
./rt-thread/libcpu/arm/cortex-m7/context_gcc.d 

C_DEPS += \
./rt-thread/libcpu/arm/cortex-m7/cpu_cache.d \
./rt-thread/libcpu/arm/cortex-m7/cpuport.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/libcpu/arm/cortex-m7/%.o: ../rt-thread/libcpu/arm/cortex-m7/%.S
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O3 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -x assembler-with-cpp -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition" -Xassembler -mimplicit-it=thumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
rt-thread/libcpu/arm/cortex-m7/%.o: ../rt-thread/libcpu/arm/cortex-m7/%.c
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O3 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\applications" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\board\CubeMX_Config\Core\Inc" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\board\port" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\board" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\STM32H7xx_HAL\CMSIS\Device\ST\STM32H7xx\Include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\STM32H7xx_HAL\CMSIS\Include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\STM32H7xx_HAL\STM32H7xx_HAL_Driver\Inc" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\drivers\include\config" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\drivers\include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\drivers" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\dfs\filesystems\devfs" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\dfs\include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\drivers\include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\drivers\spi" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\finsh" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\libc\compilers\newlib" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\libcpu\arm\common" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\libcpu\arm\cortex-m7" -include"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

