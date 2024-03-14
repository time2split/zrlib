################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/base/Vector/Vector.c \
../src/base/Vector/Vector2SideStrategy.c 

OBJS += \
./src/base/Vector/Vector.o \
./src/base/Vector/Vector2SideStrategy.o 

C_DEPS += \
./src/base/Vector/Vector.d \
./src/base/Vector/Vector2SideStrategy.d 


# Each subdirectory must supply rules for building sources it contributes
src/base/Vector/%.o: ../src/base/Vector/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


