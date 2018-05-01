SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_VERSION 1)

# specify the cross compiler
SET(CMAKE_C_COMPILER_WORKS 1)
SET(CMAKE_C_COMPILER arm-none-eabi-gcc.exe)
SET(CMAKE_OBJCOPY arm-none-eabi-objcopy.exe)
#SET(CMAKE_CXX_COMPILER arm-none-eabi-g++)

SET(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/arm-gcc-link.ld)
#Uncomment for software floating point
#SET(COMMON_FLAGS "-mcpu=cortex-m4 -mthumb -mthumb-interwork -mfloat-abi=soft -ffunction-sections -fdata-sections -g -fno-common -fmessage-length=0")
SET(COMMON_FLAGS "-mcpu=cortex-m3 -mthumb -Wall -ffunction-sections -g")
SET(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=gnu99")
SET(CMAKE_EXE_LINKER_FLAGS_INIT "-nostartfiles -g -Wl,-Map=binary.map -Wl,--gc-sections -T ${LINKER_SCRIPT}")