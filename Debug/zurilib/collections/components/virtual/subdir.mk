################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../zurilib/collections/components/virtual/GrowManager.c 

OBJS += \
./zurilib/collections/components/virtual/GrowManager.o 

C_DEPS += \
./zurilib/collections/components/virtual/GrowManager.d 


# Each subdirectory must supply rules for building sources it contributes
zurilib/collections/components/virtual/%.o: ../zurilib/collections/components/virtual/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


