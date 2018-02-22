################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Graph.cpp \
../src/GraphDeclarations.cpp \
../src/main_program.cpp 

OBJS += \
./src/Graph.o \
./src/GraphDeclarations.o \
./src/main_program.o 

CPP_DEPS += \
./src/Graph.d \
./src/GraphDeclarations.d \
./src/main_program.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


