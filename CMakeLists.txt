# Define the minimum required version of CMake
cmake_minimum_required(VERSION 3.10)

# Set the project name
project(lab2_assignment1)

set(ROOT_DIR ${CMAKE_SOURCE_DIR}/../..)

# Specify the source files
set(SOURCES
    lab2_assignment1.c
    ${ROOT_DIR}/utils/uartstdio.c
    # buttons.c
    # pinout.c
    # ${ROOT_DIR}/driverlib/pin_map.c
    startup_gcc.c
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add the executable target
add_executable(lab2_assignment1 ${SOURCES})

# Include directories
target_include_directories(lab2_assignment1 PRIVATE
    .
    ${CMAKE_SOURCE_DIR}/../../examples/boards/ek-tm4c129exl/
    ${CMAKE_SOURCE_DIR}/../..
    ${CMAKE_SOURCE_DIR}/../../utils
    ${CMAKE_SOURCE_DIR}/../../driverlib
)

# Link the necessary libraries
target_link_libraries(lab2_assignment1 PRIVATE
    ${CMAKE_SOURCE_DIR}/../../driverlib/gcc/libdriver.a
)

# Set compiler flags
target_compile_options(lab2_assignment1 PRIVATE
    -DTARGET_IS_TM4C129_RA1
)

# Specify the linker script and entry point
set_target_properties(lab2_assignment1 PROPERTIES
    LINK_FLAGS "-T ${CMAKE_SOURCE_DIR}/lab2_assignment1.ld -Wl,-Map=${CMAKE_BINARY_DIR}/lab2_assignment1.map,--entry=ResetISR"
)
