cmake_minimum_required(VERSION 3.16)

project(SandboxEngine VERSION 0.1.2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(COMPILE_SHADERS "Compile shaders during build" ON)

set(ASSET_DIR "${PROJECT_SOURCE_DIR}/Assets")
set(SHADER_DIR "${ASSET_DIR}/Shaders")
set(TEXTURE_DIR "${ASSET_DIR}/Textures")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/Source")
set(VENDOR_DIR "${SOURCE_DIR}/Vendor")
set(ENGINE_DIR "${SOURCE_DIR}/Engine")

add_subdirectory("${ENGINE_DIR}")
add_subdirectory("${VENDOR_DIR}")
