#!/bin/bash

function usage() {
  echo "Usage: $0 [-p <stm32f10x|lpc40xx>] [-f <file.cpp>]" 1>&2; exit 1;
}

# Default behavior
PLATFORM="stm32f10x"
SOURCE_FILE="main.cpp"

while getopts ":p:f:" o; do
    case "${o}" in
        p)
            PLATFORM=${OPTARG}
            ;;
        f)
            SOURCE_FILE=${OPTARG}
            ;;
        *)
            usage
            ;;
    esac
done
shift $((OPTIND-1))

TOOLCHAIN="./packages/gcc-arm-none-eabi-nano-exceptions/"
BUILD_DIRECTORY="build"
PLATFORM_GCC_ARGS=$(cat library/lib${PLATFORM}/platform/gcc.txt)
ARTIFACTS_DIRECTORY="${BUILD_DIRECTORY}/${PLATFORM}"
ARTIFACTS_COMMON_NAME="${ARTIFACTS_DIRECTORY}/${PLATFORM}.${SOURCE_FILE}"

echo "Using ELF -> ${ARTIFACTS_COMMON_NAME}.elf"

${TOOLCHAIN}/bin/arm-none-eabi-gdb-py \
  -ex "source ./tools/gdb-dashboard/.gdbinit" \
  -ex "target remote :3333" \
  -ex "monitor arm semihosting enable" \
  ${ARTIFACTS_COMMON_NAME}.elf
