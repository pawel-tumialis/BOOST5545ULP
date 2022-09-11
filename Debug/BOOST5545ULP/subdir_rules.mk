################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
BOOST5545ULP/%.obj: ../BOOST5545ULP/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C5500 Compiler'
	"C:/ti/ccs1110/ccs/tools/compiler/c5500_4.4.1/bin/cl55" -v5515 --memory_model=large -g --include_path="C:/ti/ccs1110/ccs/tools/compiler/c5500_4.4.1/include" --include_path="C:/INZYNIERKA/Przetwarzanie_sygnalow/BOOST5545ULP" --include_path="C:/INZYNIERKA/Przetwarzanie_sygnalow" --include_path="C:/INZYNIERKA/Przetwarzanie_sygnalow/inc" --include_path="C:/ti/ccs1110/ccs/tools/compiler/c5500_4.4.1/include" --define=c5545 --display_error_number --diag_warning=225 --ptrdiff_size=16 --algebraic --asm_source=algebraic --preproc_with_compile --preproc_dependency="BOOST5545ULP/$(basename $(<F)).d_raw" --obj_directory="BOOST5545ULP" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


