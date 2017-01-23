################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/ListUser.cpp \
../src/Person\ .cpp \
../src/appUser.cpp 

OBJS += \
./src/ListUser.o \
./src/Person\ .o \
./src/appUser.o 

CPP_DEPS += \
./src/ListUser.d \
./src/Person\ .d \
./src/appUser.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++1y -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/Person\ .o: ../src/Person\ .cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -std=c++1y -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src/Person .d" -MT"src/Person\ .d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


