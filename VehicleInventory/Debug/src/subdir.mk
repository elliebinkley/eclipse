################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Main.cpp \
../src/Vehicle.cpp \
../src/VehicleFile.cpp \
../src/VehicleRepo.cpp \
../src/VehicleRepoIntrusive.cpp \
../src/VehicleRepoShrPtr.cpp \
../src/VehicleRepoShrPtrUnordered.cpp \
../src/VehicleRepoTest.cpp \
../src/VehicleRepoVector.cpp 

OBJS += \
./src/Main.o \
./src/Vehicle.o \
./src/VehicleFile.o \
./src/VehicleRepo.o \
./src/VehicleRepoIntrusive.o \
./src/VehicleRepoShrPtr.o \
./src/VehicleRepoShrPtrUnordered.o \
./src/VehicleRepoTest.o \
./src/VehicleRepoVector.o 

CPP_DEPS += \
./src/Main.d \
./src/Vehicle.d \
./src/VehicleFile.d \
./src/VehicleRepo.d \
./src/VehicleRepoIntrusive.d \
./src/VehicleRepoShrPtr.d \
./src/VehicleRepoShrPtrUnordered.d \
./src/VehicleRepoTest.d \
./src/VehicleRepoVector.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=gnu++17 -I/usr/local/boost/boost_1_71_0 -O0 -g3 -Wall -c -fmessage-length=0 -v -fPIC -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


