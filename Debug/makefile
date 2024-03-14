################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include zurilib/tests/Array/subdir.mk
-include zurilib/src/subdir.mk
-include zurilib/oop/subdir.mk
-include zurilib/collections/virtual/subdir.mk
-include zurilib/collections/iterators/subdir.mk
-include zurilib/collections/components/virtual/subdir.mk
-include zurilib/collections/subdir.mk
-include zurilib/subdir.mk
-include src/base/Vector/subdir.mk
-include src/base/Allocator/subdir.mk
-include src/base/subdir.mk
-include CMakeFiles/3.12.1/CompilerIdC/subdir.mk
-include CMakeFiles/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: libzrlib.so

# Tool invocations
libzrlib.so: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -shared -o "libzrlib.so" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(LIBRARIES)$(OBJS)$(C_DEPS) libzrlib.so
	-@echo ' '

.PHONY: all clean dependents

-include ../makefile.targets
