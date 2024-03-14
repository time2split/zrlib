################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../zurilib/collections/Array.c \
../zurilib/collections/Table.c 

OBJS += \
./zurilib/collections/Array.o \
./zurilib/collections/Table.o 

C_DEPS += \
./zurilib/collections/Array.d \
./zurilib/collections/Table.d 


# Each subdirectory must supply rules for building sources it contributes
zurilib/collections/%.o: ../zurilib/collections/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


