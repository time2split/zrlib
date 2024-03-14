################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../zurilib/tests/Array/0add.c \
../zurilib/tests/Array/1iterator.c \
../zurilib/tests/Array/2removecarray.c \
../zurilib/tests/Array/3containscarray.c \
../zurilib/tests/Array/4allindexof.c \
../zurilib/tests/Array/5walkmapreduce.c 

OBJS += \
./zurilib/tests/Array/0add.o \
./zurilib/tests/Array/1iterator.o \
./zurilib/tests/Array/2removecarray.o \
./zurilib/tests/Array/3containscarray.o \
./zurilib/tests/Array/4allindexof.o \
./zurilib/tests/Array/5walkmapreduce.o 

C_DEPS += \
./zurilib/tests/Array/0add.d \
./zurilib/tests/Array/1iterator.d \
./zurilib/tests/Array/2removecarray.d \
./zurilib/tests/Array/3containscarray.d \
./zurilib/tests/Array/4allindexof.d \
./zurilib/tests/Array/5walkmapreduce.d 


# Each subdirectory must supply rules for building sources it contributes
zurilib/tests/Array/%.o: ../zurilib/tests/Array/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


