-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
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
all: Kernel

# Tool invocations
Kernel:
	@echo 'Building target: $@'
	@echo 'Invoking: GCC C Linker'
	gcc -L/home/utnso/tp-2019-1c-Los-Dinosaurios-Del-Libro/global/ -o "Kernel" -lcommons -lglobal
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets

.PHONY: all clean dependents

-include ../makefile.targets
export LD_LIBRARY_PATH=/home/utnso/tp-2019-1c-Los-Dinosaurios-Del-Libro/global/Debug:$LD_LIBRARY_PATH
