#!/bin/bash

git clone -b main \
  https://github.com/SJSU-Dev2/libcore.git packages/

git clone -b main \
  https://github.com/SJSU-Dev2/libarmcortex.git packages/

git clone -b main \
  https://github.com/SJSU-Dev2/libstm32f10x.git packages/

git clone -b main \
  https://github.com/SJSU-Dev2/liblpc40xx.git packages/

git clone -b main \
  https://github.com/SJSU-Dev2/gcc-arm-none-eabi-nano-exceptions.git packages/

git clone -b "gcc10-2021-q2-preview" \
  https://github.com/SJSU-Dev2/gcc-arm-none-eabi-nano-exceptions.git packages/

./link_libraries.sh
