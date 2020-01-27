################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Anagram.cpp \
../src/BloomberStringTest.cpp \
../src/DuplicateEntries.cpp \
../src/HomeMadeSort.cpp \
../src/LinkedListInC.cpp \
../src/RansomNote.cpp \
../src/Tries.cpp \
../src/ValidateBinaryTree.cpp \
../src/VariousPuzzles.cpp \
../src/createStairCase.cpp \
../src/inputOutputExample.cpp \
../src/reverseWords.cpp 

OBJS += \
./src/Anagram.o \
./src/BloomberStringTest.o \
./src/DuplicateEntries.o \
./src/HomeMadeSort.o \
./src/LinkedListInC.o \
./src/RansomNote.o \
./src/Tries.o \
./src/ValidateBinaryTree.o \
./src/VariousPuzzles.o \
./src/createStairCase.o \
./src/inputOutputExample.o \
./src/reverseWords.o 

CPP_DEPS += \
./src/Anagram.d \
./src/BloomberStringTest.d \
./src/DuplicateEntries.d \
./src/HomeMadeSort.d \
./src/LinkedListInC.d \
./src/RansomNote.d \
./src/Tries.d \
./src/ValidateBinaryTree.d \
./src/VariousPuzzles.d \
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


