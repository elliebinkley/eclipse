################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/MainBoostThreads.cpp \
../src/MyLogger.cpp \
../src/MyThreadBoost.cpp 

OBJS += \
./src/MainBoostThreads.o \
./src/MyLogger.o \
./src/MyThreadBoost.o 

CPP_DEPS += \
./src/MainBoostThreads.d \
./src/MyLogger.d \
./src/MyThreadBoost.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=c++1y -Dboost_system -I"/home/laurence/eclipse/eclipse-cpp-neon-workspace" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


