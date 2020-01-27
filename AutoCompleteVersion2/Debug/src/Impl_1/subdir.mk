################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Impl_1/EndOfPhrase1.cpp \
../src/Impl_1/EopSuffix1.cpp \
../src/Impl_1/Letter1.cpp \
../src/Impl_1/TopLetter1.cpp 

OBJS += \
./src/Impl_1/EndOfPhrase1.o \
./src/Impl_1/EopSuffix1.o \
./src/Impl_1/Letter1.o \
./src/Impl_1/TopLetter1.o 

CPP_DEPS += \
./src/Impl_1/EndOfPhrase1.d \
./src/Impl_1/EopSuffix1.d \
./src/Impl_1/Letter1.d \
./src/Impl_1/TopLetter1.d 


# Each subdirectory must supply rules for building sources it contributes
src/Impl_1/%.o: ../src/Impl_1/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=gnu++17 -DBOOST_LOG_DYN_LINK -O0 -g3 -Wall -c -fmessage-length=0 -v -fPIC -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


