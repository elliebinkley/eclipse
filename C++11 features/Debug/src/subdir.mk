################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/C++11\ features.cpp \
../src/Construction.cpp \
../src/SmartPointer.cpp \
../src/UniformInitializationSyntax.cpp 

OBJS += \
./src/C++11\ features.o \
./src/Construction.o \
./src/SmartPointer.o \
./src/UniformInitializationSyntax.o 

CPP_DEPS += \
./src/C++11\ features.d \
./src/Construction.d \
./src/SmartPointer.d \
./src/UniformInitializationSyntax.d 


# Each subdirectory must supply rules for building sources it contributes
src/C++11\ features.o: ../src/C++11\ features.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -v -MMD -MP -MF"src/C++11 features.d" -MT"src/C++11\ features.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++0x -O0 -g3 -Wall -c -fmessage-length=0 -v -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


