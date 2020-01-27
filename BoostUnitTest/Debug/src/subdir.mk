################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ConstructionCPlusPlusTest.cpp \
../src/InheritanceCPlusPlus.cpp \
../src/InheritanceCPlusPlusTest.cpp \
../src/SmartPointerTest.cpp \
../src/UniformInitializationSyntaxTest.cpp \
../src/VirtualFunctionsTest.cpp 

OBJS += \
./src/ConstructionCPlusPlusTest.o \
./src/InheritanceCPlusPlus.o \
./src/InheritanceCPlusPlusTest.o \
./src/SmartPointerTest.o \
./src/UniformInitializationSyntaxTest.o \
./src/VirtualFunctionsTest.o 

CPP_DEPS += \
./src/ConstructionCPlusPlusTest.d \
./src/InheritanceCPlusPlus.d \
./src/InheritanceCPlusPlusTest.d \
./src/SmartPointerTest.d \
./src/UniformInitializationSyntaxTest.d \
./src/VirtualFunctionsTest.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -Dboost_system -I"/home/laurence/eclipse/eclipse-cpp-neon-workspace" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


