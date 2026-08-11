cmake_minimum_required(VERSION 3.16)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/toolchain/build)

if(CMAKE_HOST_APPLE)
    set(ARM_TOOLCHAIN_ROOT, "/Applications/ArmGNUToolchain/15.2.rel1/arm-none-eabi")
elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(ARM_TOOLCHAIN_ROOT, "$ENV{USERPROFILE}/.local/toolchain/arm-gnu-toolchain-15.3.rel1")
else()
    set(ARM_TOOLCHAIN_ROOT, "$ENV{HOME}/.local/toolchain/arm-gnu-toolchain-15.3.rel1")
endif()

project(openretro-portable-fw-console VERSION 0.0.0 LANGUAGES C)