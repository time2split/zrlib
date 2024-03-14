################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../zurilib/oop/Allocator.c \
../zurilib/oop/Object.c \
../zurilib/oop/memory.c \
../zurilib/oop/objects.c 

OBJS += \
./zurilib/oop/Allocator.o \
./zurilib/oop/Object.o \
./zurilib/oop/memory.o \
./zurilib/oop/objects.o 

C_DEPS += \
./zurilib/oop/Allocator.d \
./zurilib/oop/Object.d \
./zurilib/oop/memory.d \
./zurilib/oop/objects.d 


# Each subdirectory must supply rules for building sources it contributes
zurilib/oop/%.o: ../zurilib/oop/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


