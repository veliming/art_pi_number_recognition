################################################################################
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../rt-thread/components/finsh/cmd.c \
../rt-thread/components/finsh/finsh_compiler.c \
../rt-thread/components/finsh/finsh_error.c \
../rt-thread/components/finsh/finsh_heap.c \
../rt-thread/components/finsh/finsh_init.c \
../rt-thread/components/finsh/finsh_node.c \
../rt-thread/components/finsh/finsh_ops.c \
../rt-thread/components/finsh/finsh_parser.c \
../rt-thread/components/finsh/finsh_token.c \
../rt-thread/components/finsh/finsh_var.c \
../rt-thread/components/finsh/finsh_vm.c \
../rt-thread/components/finsh/msh.c \
../rt-thread/components/finsh/msh_file.c \
../rt-thread/components/finsh/shell.c 

OBJS += \
./rt-thread/components/finsh/cmd.o \
./rt-thread/components/finsh/finsh_compiler.o \
./rt-thread/components/finsh/finsh_error.o \
./rt-thread/components/finsh/finsh_heap.o \
./rt-thread/components/finsh/finsh_init.o \
./rt-thread/components/finsh/finsh_node.o \
./rt-thread/components/finsh/finsh_ops.o \
./rt-thread/components/finsh/finsh_parser.o \
./rt-thread/components/finsh/finsh_token.o \
./rt-thread/components/finsh/finsh_var.o \
./rt-thread/components/finsh/finsh_vm.o \
./rt-thread/components/finsh/msh.o \
./rt-thread/components/finsh/msh_file.o \
./rt-thread/components/finsh/shell.o 

C_DEPS += \
./rt-thread/components/finsh/cmd.d \
./rt-thread/components/finsh/finsh_compiler.d \
./rt-thread/components/finsh/finsh_error.d \
./rt-thread/components/finsh/finsh_heap.d \
./rt-thread/components/finsh/finsh_init.d \
./rt-thread/components/finsh/finsh_node.d \
./rt-thread/components/finsh/finsh_ops.d \
./rt-thread/components/finsh/finsh_parser.d \
./rt-thread/components/finsh/finsh_token.d \
./rt-thread/components/finsh/finsh_var.d \
./rt-thread/components/finsh/finsh_vm.d \
./rt-thread/components/finsh/msh.d \
./rt-thread/components/finsh/msh_file.d \
./rt-thread/components/finsh/shell.d 


# Each subdirectory must supply rules for building sources it contributes
rt-thread/components/finsh/%.o: ../rt-thread/components/finsh/%.c
	arm-none-eabi-gcc -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -O3 -ffunction-sections -fdata-sections -Wall  -g -gdwarf-2 -DDEBUG -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\applications" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\board\CubeMX_Config\Core\Inc" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\board\port" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\board" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\STM32H7xx_HAL\CMSIS\Device\ST\STM32H7xx\Include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\STM32H7xx_HAL\CMSIS\Include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\STM32H7xx_HAL\STM32H7xx_HAL_Driver\Inc" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\drivers\include\config" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\drivers\include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\libraries\drivers" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\dfs\filesystems\devfs" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\dfs\include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\drivers\include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\drivers\spi" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\finsh" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\components\libc\compilers\newlib" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\include" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\libcpu\arm\common" -I"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rt-thread\libcpu\arm\cortex-m7" -include"D:\APP\RT-ThreadStudio\workspace\art_pi_number_recognition\rtconfig_preinc.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

