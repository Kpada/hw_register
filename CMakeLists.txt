cmake_minimum_required(VERSION 3.8)

set(PROJ "HWRegisters")
project(${PROJ})


#######################################
# Compiler and Tools
#######################################

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

find_program(ARM_OBJCOPY    arm-none-eabi-objcopy)
find_program(ARM_SIZE       arm-none-eabi-size)
find_program(ARM_DUMP       arm-none-eabi-objdump)
find_program(ARM_CXX        arm-none-eabi-g++)

set(CMAKE_CXX_STANDARD      17)
set(CMAKE_CXX_COMPILER      ${ARM_CXX})

# -rdynamic fix
set(CMAKE_SHARED_LIBRARY_LINK_C_FLAGS   "")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
set(CMAKE_SHARED_LIBRARY_LINK_ASM_FLAGS "")


#######################################
# Sources
#######################################

SET(CPP_SOURCES
    ${CMAKE_SOURCE_DIR}/code/src/startup_stm32f103xb.cpp
    ${CMAKE_SOURCE_DIR}/code/src/led_button.cpp
    ${CMAKE_SOURCE_DIR}/code/src/utils.cpp
)

#######################################
# Includes
#######################################

include_directories(
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/code/inc
)

#######################################
# Flags
#######################################

set(COMMON_FLAGS            " -mcpu=cortex-m3 -mthumb ")
string(APPEND COMMON_FLAGS  " -specs=nano.specs -specs=nosys.specs ")
string(APPEND COMMON_FLAGS  " -Wall  -Og -g ")
string(APPEND COMMON_FLAGS  " -fdata-sections -ffunction-sections -fno-exceptions ")
string(APPEND COMMON_FLAGS  " -fstack-usage -mfloat-abi=soft -MMD -MP ")

set(LINKER_FLAGS            " -T../STM32F103C8Tx_FLASH.ld  ")
string(APPEND LINKER_FLAGS  " -specs=rdimon.specs -Wl,--gc-sections,-Map=${PROJ}.map -lc -lm -lnosys ")

set(CMAKE_ASM_FLAGS         " -x assembler-with-cpp ${COMMON_FLAGS}")
set(CMAKE_CXX_FLAGS         " -std=gnu++17 ${COMMON_FLAGS} -fno-rtti -fno-use-cxa-atexit " )
set(CMAKE_EXE_LINKER_FLAGS  " ${LINKER_FLAGS}" )


#######################################
# Executable
#######################################

add_executable(${PROJ}.elf ${CPP_SOURCES} ${ASM_SOURCES} )


#######################################
# Post
#######################################

add_custom_command(TARGET ${PROJ}.elf POST_BUILD COMMAND ${ARM_DUMP} -h -S ${PROJ}.elf > ${PROJ}.list)
#add_custom_command(TARGET ${PROJ}.elf POST_BUILD COMMAND ${ARM_DUMP} -S -l ${PROJ}.elf > ${PROJ}.S)

add_custom_command(TARGET ${PROJ}.elf POST_BUILD COMMAND ${ARM_SIZE} ${PROJ}.elf)
