#!/bin/bash

# If the first argument is empty default to stm32f10x
if [[ -z "$1" ]]; then
  PLATFORM="stm32f10x"
else
  PLATFORM="$1"
fi

TOOLCHAIN="./tools/gcc-arm-none-eabi-nano-exceptions/gcc-arm-none-eabi-10-2021-q2-preview"

${TOOLCHAIN}/bin/arm-none-eabi-g++ \
  -T library/lib${PLATFORM}/lib${PLATFORM}/platform/default.ld \
  -D PLATFORM=${PLATFORM} \
  -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -mtpcs-frame -mtpcs-leaf-frame \
  -fexceptions -fno-rtti -ffunction-sections -fdata-sections \
  -fno-threadsafe-statics -fno-omit-frame-pointer -fdiagnostics-color \
  -Wno-main -Wall -Wextra -Wformat=2 -Wno-uninitialized -Wnull-dereference \
  -Wold-style-cast -Woverloaded-virtual -Wsuggest-override \
  -Wl,--gc-sections --specs=rdimon.specs --specs=nano.specs \
  -std=c++20 -g -Og -I library/lib \
  main.cpp library/lib${PLATFORM}/lib${PLATFORM}/platform/startup.cpp \
  -o main.elf
