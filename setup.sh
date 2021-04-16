#!/bin/bash

function link_library() {
  ln -s $(pwd)/$1/$1 lib/$1
}

cd ./library/

git clone https://github.com/SJSU-Dev2/libcore.git
git clone https://github.com/SJSU-Dev2/libarmcortex.git
git clone https://github.com/SJSU-Dev2/libstm32f10x.git
git clone https://github.com/SJSU-Dev2/liblpc40xx.git

rm -rf lib
mkdir lib

link_library libcore
link_library libarmcortex
link_library libstm32f10x
link_library liblpc40xx

cd -

cd tools/

git clone https://github.com/SJSU-Dev2/gcc-arm-none-eabi-nano-exceptions.git
cd gcc-arm-none-eabi-nano-exceptions/
git checkout gcc10-2021-q2-preview
