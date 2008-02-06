################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../xml/nodeDoc.cpp \
../xml/xmlEngine.cpp \
../xml/xmlFile.cpp 

OBJS += \
./xml/nodeDoc.o \
./xml/xmlEngine.o \
./xml/xmlFile.o 

CPP_DEPS += \
./xml/nodeDoc.d \
./xml/xmlEngine.d \
./xml/xmlFile.d 


# Each subdirectory must supply rules for building sources it contributes
xml/%.o: ../xml/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/include/libxml2 -O3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


