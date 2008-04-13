################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../regexp/regExp.cpp 

OBJS += \
./regexp/regExp.o 

CPP_DEPS += \
./regexp/regExp.d 


# Each subdirectory must supply rules for building sources it contributes
regexp/%.o: ../regexp/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/libxml2 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


