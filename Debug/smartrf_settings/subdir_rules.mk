################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
smartrf_settings/smartrf_settings.obj: ../smartrf_settings/smartrf_settings.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.12.0.STS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="/Users/rustemavhadiev/Documents/sensorCC1310" --include_path="/Users/rustemavhadiev/ti/simplelink_cc13x0_sdk_1_30_00_06/kernel/tirtos/packages/ti/sysbios/posix" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.12.0.STS/include" --define=DEVICE_FAMILY=cc13x0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="smartrf_settings/smartrf_settings.d" --obj_directory="smartrf_settings" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

smartrf_settings/smartrf_settings_predefined.obj: ../smartrf_settings/smartrf_settings_predefined.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.12.0.STS/bin/armcl" -mv7M3 --code_state=16 --float_support=vfplib -me --include_path="/Users/rustemavhadiev/Documents/sensorCC1310" --include_path="/Users/rustemavhadiev/ti/simplelink_cc13x0_sdk_1_30_00_06/kernel/tirtos/packages/ti/sysbios/posix" --include_path="/Applications/ti/ccsv7/tools/compiler/ti-cgt-arm_16.12.0.STS/include" --define=DEVICE_FAMILY=cc13x0 -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="smartrf_settings/smartrf_settings_predefined.d" --obj_directory="smartrf_settings" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


