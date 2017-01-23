################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/BloomberStringTest.cpp \
../src/HomeMadeSort.cpp \
../src/createStairCase.cpp \
../src/inputOutputExample.cpp \
../src/reverseWords.cpp 

OBJS += \
./src/BloomberStringTest.o \
./src/HomeMadeSort.o \
./src/createStairCase.o \
./src/inputOutputExample.o \
./src/reverseWords.o 

CPP_DEPS += \
./src/BloomberStringTest.d \
./src/HomeMadeSort.d \
./src/createStairCase.d \
./src/inputOutputExample.d \
./src/reverseWords.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++1y -O0 -g3 -Wall -c -fmessage-length=0 -std=gnu++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


