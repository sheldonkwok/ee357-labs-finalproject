################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS_QUOTED += \
"../Sources/MCF52259_sysinit.c" \
"../Sources/exceptions.c" \
"../Sources/main.c" \
"../Sources/uart_support.c" \
"../Sources/usc_support.c" 

C_SRCS += \
../Sources/MCF52259_sysinit.c \
../Sources/exceptions.c \
../Sources/main.c \
../Sources/uart_support.c \
../Sources/usc_support.c 

OBJS += \
./Sources/MCF52259_sysinit.obj \
./Sources/exceptions.obj \
./Sources/main.obj \
./Sources/uart_support.obj \
./Sources/usc_support.obj 

OBJS_QUOTED += \
"./Sources/MCF52259_sysinit.obj" \
"./Sources/exceptions.obj" \
"./Sources/main.obj" \
"./Sources/uart_support.obj" \
"./Sources/usc_support.obj" 

C_DEPS += \
./Sources/MCF52259_sysinit.d \
./Sources/exceptions.d \
./Sources/main.d \
./Sources/uart_support.d \
./Sources/usc_support.d 

OBJS_OS_FORMAT += \
./Sources/MCF52259_sysinit.obj \
./Sources/exceptions.obj \
./Sources/main.obj \
./Sources/uart_support.obj \
./Sources/usc_support.obj 


# Each subdirectory must supply rules for building sources it contributes
Sources/MCF52259_sysinit.obj: ../Sources/MCF52259_sysinit.c
	@echo 'Building file: $<'
	@echo 'Invoking: ColdFire Compiler'
	"C:\Program Files\Freescale\CW MCU v10.0\eclipse\../MCU/ColdFire_Tools/Command_Line_Tools/mwccmcf" @"Sources/MCF52259_sysinit.args" -o "Sources/MCF52259_sysinit.obj" "$<"
	"C:\Program Files\Freescale\CW MCU v10.0\eclipse\../MCU/ColdFire_Tools/Command_Line_Tools/mwccmcf" @"Sources/MCF52259_sysinit.args" -o "Sources/MCF52259_sysinit.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/%.d: ../Sources/%.c
	@echo 'Regenerating dependency file: $@'
	
	@echo ' '

Sources/exceptions.obj: ../Sources/exceptions.c
	@echo 'Building file: $<'
	@echo 'Invoking: ColdFire Compiler'
	"C:\Program Files\Freescale\CW MCU v10.0\eclipse\../MCU/ColdFire_Tools/Command_Line_Tools/mwccmcf" @"Sources/exceptions.args" -o "Sources/exceptions.obj" "$<"
	"C:\Program Files\Freescale\CW MCU v10.0\eclipse\../MCU/ColdFire_Tools/Command_Line_Tools/mwccmcf" @"Sources/exceptions.args" -o "Sources/exceptions.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/main.obj: ../Sources/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: ColdFire Compiler'
	"C:\Program Files\Freescale\CW MCU v10.0\eclipse\../MCU/ColdFire_Tools/Command_Line_Tools/mwccmcf" @"Sources/main.args" -o "Sources/main.obj" "$<"
	"C:\Program Files\Freescale\CW MCU v10.0\eclipse\../MCU/ColdFire_Tools/Command_Line_Tools/mwccmcf" @"Sources/main.args" -o "Sources/main.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/uart_support.obj: ../Sources/uart_support.c
	@echo 'Building file: $<'
	@echo 'Invoking: ColdFire Compiler'
	"C:\Program Files\Freescale\CW MCU v10.0\eclipse\../MCU/ColdFire_Tools/Command_Line_Tools/mwccmcf" @"Sources/uart_support.args" -o "Sources/uart_support.obj" "$<"
	"C:\Program Files\Freescale\CW MCU v10.0\eclipse\../MCU/ColdFire_Tools/Command_Line_Tools/mwccmcf" @"Sources/uart_support.args" -o "Sources/uart_support.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '

Sources/usc_support.obj: ../Sources/usc_support.c
	@echo 'Building file: $<'
	@echo 'Invoking: ColdFire Compiler'
	"C:\Program Files\Freescale\CW MCU v10.0\eclipse\../MCU/ColdFire_Tools/Command_Line_Tools/mwccmcf" @"Sources/usc_support.args" -o "Sources/usc_support.obj" "$<"
	"C:\Program Files\Freescale\CW MCU v10.0\eclipse\../MCU/ColdFire_Tools/Command_Line_Tools/mwccmcf" @"Sources/usc_support.args" -o "Sources/usc_support.obj" "$<" -MD -gccdep
	@echo 'Finished building: $<'
	@echo ' '


