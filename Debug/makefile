################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccs1110/ccs/tools/compiler/c5500_4.4.1

GEN_OPTS__FLAG := 
GEN_CMDS__FLAG := 

ORDERED_OBJS += \
"./main.obj" \
"./BOOST5545ULP/button.obj" \
"./BOOST5545ULP/codec_3206.obj" \
"./BOOST5545ULP/led.obj" \
"./BOOST5545ULP/oled.obj" \
"./src/csl_audioClass.obj" \
"./src/csl_cdc.obj" \
"./src/csl_dat.obj" \
"./src/csl_dma.obj" \
"./src/csl_emif.obj" \
"./src/csl_gpio.obj" \
"./src/csl_gpt.obj" \
"./src/csl_i2c.obj" \
"./src/csl_i2s.obj" \
"./src/csl_intc.obj" \
"./src/csl_irqplug.obj" \
"./src/csl_lcdc.obj" \
"./src/csl_mcbsp.obj" \
"./src/csl_mcspi.obj" \
"./src/csl_mem.obj" \
"./src/csl_mmcsd.obj" \
"./src/csl_mmcsd_ataIf.obj" \
"./src/csl_msc.obj" \
"./src/csl_pll.obj" \
"./src/csl_rtc.obj" \
"./src/csl_sar.obj" \
"./src/csl_sdio.obj" \
"./src/csl_spi.obj" \
"./src/csl_sysctrl.obj" \
"./src/csl_uart.obj" \
"./src/csl_uhpi.obj" \
"./src/csl_usb.obj" \
"./src/csl_wdt.obj" \
"./src/evm5515.obj" \
"./src/evm5515_gpio.obj" \
"./src/evm5515_i2c.obj" \
"./src/i2c_probe.obj" \
"./src/platform.obj" \
"./src/system_init.obj" \
"./src/vectors.obj" \
"../C5545BP.cmd" \
$(GEN_CMDS__FLAG) \
-llibc.a \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include BOOST5545ULP/subdir_vars.mk
-include src/subdir_vars.mk
-include subdir_rules.mk
-include BOOST5545ULP/subdir_rules.mk
-include src/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(INO_DEPS)),)
-include $(INO_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(SV7A_DEPS)),)
-include $(SV7A_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(PDE_DEPS)),)
-include $(PDE_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
EXE_OUTPUTS += \
Przetwarzanie_sygnalow.out \

EXE_OUTPUTS__QUOTED += \
"Przetwarzanie_sygnalow.out" \

BIN_OUTPUTS += \
Przetwarzanie_sygnalow.hex \

BIN_OUTPUTS__QUOTED += \
"Przetwarzanie_sygnalow.hex" \


# All Target
all: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@$(MAKE) --no-print-directory -Onone "Przetwarzanie_sygnalow.out"

# Tool invocations
Przetwarzanie_sygnalow.out: $(OBJS) $(CMD_SRCS) $(GEN_CMDS)
	@echo 'Building target: "$@"'
	@echo 'Invoking: C5500 Linker'
	"C:/ti/ccs1110/ccs/tools/compiler/c5500_4.4.1/bin/cl55" -v5515 --memory_model=large -g --define=c5545 --display_error_number --diag_warning=225 --ptrdiff_size=16 --algebraic --asm_source=algebraic -z -m"Przetwarzanie_sygnalow.map" --stack_size=0x200 --heap_size=0x400 -i"C:/ti/ccs1110/ccs/tools/compiler/c5500_4.4.1/lib" -i"C:/ti/ccs1110/ccs/tools/compiler/c5500_4.4.1/include" --reread_libs --display_error_number --warn_sections --xml_link_info="Przetwarzanie_sygnalow_linkInfo.xml" --rom_model --sys_stacksize=0x200 -o "Przetwarzanie_sygnalow.out" $(ORDERED_OBJS)
	@echo 'Finished building target: "$@"'
	@echo ' '

Przetwarzanie_sygnalow.hex: $(EXE_OUTPUTS)
	@echo 'Building secondary target: "$@"'
	@echo 'Invoking: C5500 Hex Utility'
	"C:/ti/ccs1110/ccs/tools/compiler/c5500_4.4.1/bin/hex55"  -o "Przetwarzanie_sygnalow.hex" $(EXE_OUTPUTS__QUOTED)
	@echo 'Finished building secondary target: "$@"'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(BIN_OUTPUTS__QUOTED)$(EXE_OUTPUTS__QUOTED)
	-$(RM) "main.obj" "BOOST5545ULP\button.obj" "BOOST5545ULP\codec_3206.obj" "BOOST5545ULP\led.obj" "BOOST5545ULP\oled.obj" "src\csl_audioClass.obj" "src\csl_cdc.obj" "src\csl_dat.obj" "src\csl_dma.obj" "src\csl_emif.obj" "src\csl_gpio.obj" "src\csl_gpt.obj" "src\csl_i2c.obj" "src\csl_i2s.obj" "src\csl_intc.obj" "src\csl_irqplug.obj" "src\csl_lcdc.obj" "src\csl_mcbsp.obj" "src\csl_mcspi.obj" "src\csl_mem.obj" "src\csl_mmcsd.obj" "src\csl_mmcsd_ataIf.obj" "src\csl_msc.obj" "src\csl_pll.obj" "src\csl_rtc.obj" "src\csl_sar.obj" "src\csl_sdio.obj" "src\csl_spi.obj" "src\csl_sysctrl.obj" "src\csl_uart.obj" "src\csl_uhpi.obj" "src\csl_usb.obj" "src\csl_wdt.obj" "src\evm5515.obj" "src\evm5515_gpio.obj" "src\evm5515_i2c.obj" "src\i2c_probe.obj" "src\platform.obj" "src\system_init.obj" "src\vectors.obj" 
	-$(RM) "main.d" "BOOST5545ULP\button.d" "BOOST5545ULP\codec_3206.d" "BOOST5545ULP\led.d" "BOOST5545ULP\oled.d" "src\csl_audioClass.d" "src\csl_cdc.d" "src\csl_dat.d" "src\csl_dma.d" "src\csl_emif.d" "src\csl_gpio.d" "src\csl_gpt.d" "src\csl_i2c.d" "src\csl_i2s.d" "src\csl_intc.d" "src\csl_lcdc.d" "src\csl_mcbsp.d" "src\csl_mcspi.d" "src\csl_mem.d" "src\csl_mmcsd.d" "src\csl_mmcsd_ataIf.d" "src\csl_msc.d" "src\csl_pll.d" "src\csl_rtc.d" "src\csl_sar.d" "src\csl_sdio.d" "src\csl_spi.d" "src\csl_sysctrl.d" "src\csl_uart.d" "src\csl_uhpi.d" "src\csl_usb.d" "src\csl_wdt.d" "src\evm5515.d" "src\evm5515_gpio.d" "src\evm5515_i2c.d" "src\i2c_probe.d" "src\platform.d" "src\system_init.d" 
	-$(RM) "src\csl_irqplug.d" "src\vectors.d" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets

