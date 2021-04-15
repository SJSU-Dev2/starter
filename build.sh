#!/bin/bash

TOOLCHAIN="./tools/gcc-arm-none-eabi-nano-exceptions/gcc-arm-none-eabi-10-2021-q2-preview"

${TOOLCHAIN}/bin/arm-none-eabi-g++ \
  -T library/libstm32f1xx/libstm32f1xx/platform/default.ld \
  -D PLATFORM="stm32f10x" \
  -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -mtpcs-frame -mtpcs-leaf-frame \
  -fexceptions -fno-rtti -ffunction-sections -fdata-sections \
  -fno-threadsafe-statics -fno-omit-frame-pointer -fdiagnostics-color \
  -Wno-main -Wall -Wextra -Wformat=2 -Wno-uninitialized -Wnull-dereference \
  -Wold-style-cast -Woverloaded-virtual -Wsuggest-override \
  -Wl,--gc-sections --specs=rdimon.specs --specs=nano.specs \
  -std=c++2a -g -Og -I library/link -o main.elf \
  main.cpp library/libstm32f1xx/libstm32f1xx/platform/startup.cpp
