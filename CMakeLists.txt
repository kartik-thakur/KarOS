cmake_minimum_required(VERSION 3.13)

# Pull in SDK (must be before project)
include(pico_sdk_import.cmake)

project(KarOS C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

set(PICO_EXAMPLES_PATH ${PROJECT_SOURCE_DIR})

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

add_executable(KarOS)

target_link_libraries(KarOS
	pico_stdlib
	hardware_i2c
)

target_include_directories(KarOS PUBLIC ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(apps)
add_subdirectory(drivers)
add_subdirectory(kernel)
add_subdirectory(lib)

pico_add_extra_outputs(KarOS)
pico_enable_stdio_usb(KarOS 1)
