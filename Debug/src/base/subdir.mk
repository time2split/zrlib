################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/base/ArrayOp.c \
../src/base/Bits.c \
../src/base/MemoryOp.c 

OBJS += \
./src/base/ArrayOp.o \
./src/base/Bits.o \
./src/base/MemoryOp.o 

C_DEPS += \
./src/base/ArrayOp.d \
./src/base/Bits.d \
./src/base/MemoryOp.d 


# Each subdirectory must supply rules for building sources it contributes
src/base/%.o: ../src/base/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


