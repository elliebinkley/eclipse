################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AutoCompleteVersion.cpp \
../src/Config.cpp \
../src/Corpus.cpp \
../src/CorpusImplMap.cpp \
../src/CorpusImplMapCompress.cpp \
../src/CorpusImplTrie.cpp \
../src/EndOfPhrase.cpp \
../src/EopSuffix.cpp \
../src/FoundItems.cpp \
../src/FoundItemsImplMap.cpp \
../src/FoundItemsImplTrie.cpp \
../src/Letter.cpp \
../src/TopLetter.cpp 

OBJS += \
./src/AutoCompleteVersion.o \
./src/Config.o \
./src/Corpus.o \
./src/CorpusImplMap.o \
./src/CorpusImplMapCompress.o \
./src/CorpusImplTrie.o \
./src/EndOfPhrase.o \
./src/EopSuffix.o \
./src/FoundItems.o \
./src/FoundItemsImplMap.o \
./src/FoundItemsImplTrie.o \
./src/Letter.o \
./src/TopLetter.o 

CPP_DEPS += \
./src/AutoCompleteVersion.d \
./src/Config.d \
./src/Corpus.d \
./src/CorpusImplMap.d \
./src/CorpusImplMapCompress.d \
./src/CorpusImplTrie.d \
./src/EndOfPhrase.d \
./src/EopSuffix.d \
./src/FoundItems.d \
./src/FoundItemsImplMap.d \
./src/FoundItemsImplTrie.d \
./src/Letter.d \
./src/TopLetter.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -std=gnu++17 -DBOOST_LOG_DYN_LINK -I/usr/local/boost/boost_1_71_0/boost -I"/home/laurence/eclipse/eclipse-cpp-neon-workspace" -O0 -g3 -Wall -c -fmessage-length=0 -v -fPIC -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


