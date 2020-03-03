################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include Src/subdir.mk
-include Drivers/STM32L0xx_HAL_Driver/Src/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXECUTABLES += \
ESP_smart_home.elf \

SIZE_OUTPUT += \
default.size.stdout \

OBJDUMP_LIST += \
ESP_smart_home.list \

OBJCOPY_HEX += \
ESP_smart_home.hex \


# All Target
all: ESP_smart_home.elf secondary-outputs

# Tool invocations
ESP_smart_home.elf: $(OBJS) $(USER_OBJS) ../STM32L073RZTx_FLASH.ld
	arm-none-eabi-gcc -o "ESP_smart_home.elf" @"objects.list" $(USER_OBJS) $(LIBS) -mcpu=cortex-m0plus -T"../STM32L073RZTx_FLASH.ld" --specs=nosys.specs -Wl,-Map="ESP_smart_home.map" -Wl,--gc-sections -static --specs=nano.specs -mfloat-abi=soft -mthumb -Wl,--start-group -lc -lm -Wl,--end-group
	@echo 'Finished building target: $@'
	@echo ' '

default.size.stdout: $(EXECUTABLES)
	arm-none-eabi-size  $(EXECUTABLES)
	@echo 'Finished building: $@'
	@echo ' '

ESP_smart_home.list: $(EXECUTABLES)
	arm-none-eabi-objdump -h -S $(EXECUTABLES) > "ESP_smart_home.list"
	@echo 'Finished building: $@'
	@echo ' '

ESP_smart_home.hex: $(EXECUTABLES)
	arm-none-eabi-objcopy  -O ihex $(EXECUTABLES) "ESP_smart_home.hex"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) *
	-@echo ' '

secondary-outputs: $(SIZE_OUTPUT) $(OBJDUMP_LIST) $(OBJCOPY_HEX)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
