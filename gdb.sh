#!/bin/bash

# If the first argument is empty default to stm32f10x
if [[ -z "$1" ]]; then
  PLATFORM="stm32f10x"
else
  PLATFORM="$1"
fi

TOOLCHAIN="./packages/gcc-arm-none-eabi-nano-exceptions/"
BUILD_DIRECTORY="build"
PLATFORM_GCC_ARGS=$(cat library/lib${PLATFORM}/platform/gcc.txt)
ARTIFACTS_DIRECTORY="${BUILD_DIRECTORY}/${PLATFORM}"
ARTIFACTS_COMMON_NAME="${ARTIFACTS_DIRECTORY}/main-${PLATFORM}"

echo "Using ELF -> ${ARTIFACTS_COMMON_NAME}.elf"

${TOOLCHAIN}/bin/arm-none-eabi-gdb-py \
  -ex "source ./tools/gdb-dashboard/.gdbinit" \
  -ex "target remote :3333" \
  -ex "monitor arm semihosting enable" \
  ${ARTIFACTS_COMMON_NAME}.elf
