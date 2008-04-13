################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../engineResults.cpp \
../row.cpp \
../selectFile.cpp \
../tdParam.cpp 

OBJS += \
./engineResults.o \
./row.o \
./selectFile.o \
./tdParam.o 

CPP_DEPS += \
./engineResults.d \
./row.d \
./selectFile.d \
./tdParam.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/libxml2 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


