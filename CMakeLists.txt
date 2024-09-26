cmake_minimum_required(VERSION 3.25)
project(OpenGL)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

cmake_policy(SET CMP0079 NEW)
cmake_policy(SET CMP0054 NEW)

if(${CMAKE_VERSION} VERSION_GREATER_EQUAL "3.24.0")
    cmake_policy(SET CMP0135 NEW)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -Wall -Wextra -Wno-sign-compare")

# Custom target that always executes.
# Creates a symlink to assets inside the build directory.
add_custom_target(create_asset_link ALL COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets)

add_compile_definitions("DEBUG=$<CONFIG:Debug>")

function(add_example example_name USES)
    if(${USES} STREQUAL "USES")
        add_executable(${example_name} ${example_name}.cpp)
        target_link_libraries(${example_name} ${ARGN})
    else()
        message(FATAL_ERROR "add_example lacks uses clause")
    endif()
endfunction(add_example)

macro(add_example_assets example_name)
    target_compile_definitions(${example_name} PUBLIC "CUSTOM_ASSETS_PATH=\"${CMAKE_CURRENT_SOURCE_DIR}/assets\"")
endmacro(add_example_assets)


# Dependencies
add_subdirectory(dependencies)
find_package(glfw3 3.3 REQUIRED)
add_subdirectory(libs)

# Source files
add_subdirectory(MKE)

# Examples
option(BUILD_EXAMPLES "Build examples" ON)
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(BUILD_EXAMPLES)
