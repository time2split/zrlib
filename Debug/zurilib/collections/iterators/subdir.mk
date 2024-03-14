################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../zurilib/collections/iterators/List.c 

OBJS += \
./zurilib/collections/iterators/List.o 

C_DEPS += \
./zurilib/collections/iterators/List.d 


# Each subdirectory must supply rules for building sources it contributes
zurilib/collections/iterators/%.o: ../zurilib/collections/iterators/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


