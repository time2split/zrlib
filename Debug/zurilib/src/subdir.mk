################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../zurilib/src/zurilibc.c 

OBJS += \
./zurilib/src/zurilibc.o 

C_DEPS += \
./zurilib/src/zurilibc.d 


# Each subdirectory must supply rules for building sources it contributes
zurilib/src/%.o: ../zurilib/src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


