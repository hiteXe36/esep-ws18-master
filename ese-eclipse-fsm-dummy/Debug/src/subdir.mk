################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/controller/Dispatcher.cpp \
../src/controller/DummyGenerator.cpp \
../src/controller/GateControl.cpp \
../src/controller/MainControl.cpp \
../src/controller/MotorControl.cpp \
../src/controller/PUKTypeControl.cpp \
../src/controller/SerialControl.cpp \
../src/controller/TimerControl.cpp 

OBJS += \
./src/controller/Dispatcher.o \
./src/controller/DummyGenerator.o \
./src/controller/GateControl.o \
./src/controller/MainControl.o \
./src/controller/MotorControl.o \
./src/controller/PUKTypeControl.o \
./src/controller/SerialControl.o \
./src/controller/TimerControl.o 

CPP_DEPS += \
./src/controller/Dispatcher.d \
./src/controller/DummyGenerator.d \
./src/controller/GateControl.d \
./src/controller/MainControl.d \
./src/controller/MotorControl.d \
./src/controller/PUKTypeControl.d \
./src/controller/SerialControl.d \
./src/controller/TimerControl.d 


# Each subdirectory must supply rules for building sources it contributes
src/controller/%.o: ../src/controller/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cygwin C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


