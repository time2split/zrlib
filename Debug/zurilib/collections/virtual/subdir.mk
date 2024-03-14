################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../zurilib/collections/virtual/Collection.c \
../zurilib/collections/virtual/Iterator.c \
../zurilib/collections/virtual/List.c 

OBJS += \
./zurilib/collections/virtual/Collection.o \
./zurilib/collections/virtual/Iterator.o \
./zurilib/collections/virtual/List.o 

C_DEPS += \
./zurilib/collections/virtual/Collection.d \
./zurilib/collections/virtual/Iterator.d \
./zurilib/collections/virtual/List.d 


# Each subdirectory must supply rules for building sources it contributes
zurilib/collections/virtual/%.o: ../zurilib/collections/virtual/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


