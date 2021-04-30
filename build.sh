#!/bin/bash


function check_mark() {
  if [[ $? -eq "0" ]] ; then
    printf "${CHECK_MARK}\n" # check mark
  else
    printf "${BALLOT_X}\n\n" # ballot cross
    cat ${ARTIFACTS_COMMON_NAME}.log
    echo ""
    exit 1
  fi
}

# If the first argument is empty default to stm32f10x
if [[ -z "$1" ]]; then
  PLATFORM="stm32f10x"
else
  PLATFORM="$1"
fi

CHECK_MARK="\e[32m\xE2\x9C\x94\e[0m"
BALLOT_X="\e[31m\xE2\x9C\x97\e[0m"

TOOLCHAIN="./packages/gcc-arm-none-eabi-nano-exceptions/"
BUILD_DIRECTORY="build"
PLATFORM_GCC_ARGS=$(cat library/lib${PLATFORM}/platform/gcc.txt)
ARTIFACTS_DIRECTORY="${BUILD_DIRECTORY}/${PLATFORM}"
ARTIFACTS_COMMON_NAME="${ARTIFACTS_DIRECTORY}/main-${PLATFORM}"

mkdir -p ${ARTIFACTS_DIRECTORY}/

printf "Building '${ARTIFACTS_COMMON_NAME}.elf' ... "
${TOOLCHAIN}/bin/arm-none-eabi-g++ \
  -fexceptions -fno-rtti -ffunction-sections -fdata-sections \
  -fno-threadsafe-statics -fno-omit-frame-pointer -fdiagnostics-color \
  -Wno-main -Wall -Wextra -Wformat=2 -Wno-uninitialized -Wnull-dereference \
  -Wold-style-cast -Woverloaded-virtual -Wsuggest-override \
  -Wl,--gc-sections --specs=rdimon.specs --specs=nano.specs \
  -fcompare-debug-second \
  -std=c++20 -g -Og -I library/ \
  ${PLATFORM_GCC_ARGS} \
  -D PLATFORM=${PLATFORM} \
  -T library/lib${PLATFORM}/platform/default.ld \
  main.cpp library/lib${PLATFORM}/platform/startup.cpp \
  -o ${ARTIFACTS_COMMON_NAME}.elf &> ${ARTIFACTS_COMMON_NAME}.log


check_mark $?

printf "Generating: .bin  (binary) "
${TOOLCHAIN}/bin/arm-none-eabi-objcopy -O binary \
  ${ARTIFACTS_COMMON_NAME}.elf \
  ${ARTIFACTS_COMMON_NAME}.bin
check_mark $?

printf "            .hex  (intel HEX file) "
${TOOLCHAIN}/bin/arm-none-eabi-objcopy -O ihex \
  ${ARTIFACTS_COMMON_NAME}.elf \
  ${ARTIFACTS_COMMON_NAME}.hex
check_mark $?

printf "            .S    (disassembly) "
${TOOLCHAIN}/bin/arm-none-eabi-objdump --disassemble --demangle \
  ${ARTIFACTS_COMMON_NAME}.elf > ${ARTIFACTS_COMMON_NAME}.S
check_mark $?

printf "            .lst  (disassembly with source code) "
${TOOLCHAIN}/bin/arm-none-eabi-objdump \
  --all-headers --source --disassemble --demangle \
  ${ARTIFACTS_COMMON_NAME}.elf > ${ARTIFACTS_COMMON_NAME}.lst
check_mark $?

printf "            .size (size information) "
${TOOLCHAIN}/bin/arm-none-eabi-size \
  ${ARTIFACTS_COMMON_NAME}.elf > ${ARTIFACTS_COMMON_NAME}.siz
check_mark $?
