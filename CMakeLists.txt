cmake_minimum_required(VERSION 3.16)

project(SandboxEngine VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/Source")
set(VENDOR_DIR "${SOURCE_DIR}/Vendor")
set(ENGINE_DIR "${SOURCE_DIR}/SandboxEngine")
set(PLATFORM_DIR "${SOURCE_DIR}/Platform")

# Specify project source files
file(GLOB_RECURSE PROJECT_PRECOMPILED_HEADERS CONFIGURE_DEPENDS 
    "${SOURCE_DIR}/*pch.h"
    "${SOURCE_DIR}/*pch.cpp"
)

file(GLOB_RECURSE PROJECT_SOURCE CONFIGURE_DEPENDS 
    "${ENGINE_DIR}/*.h"
    "${ENGINE_DIR}/*.cpp"
    "${PLATFORM_DIR}/${CMAKE_SYSTEM_NAME}/*.h"
    "${PLATFORM_DIR}/${CMAKE_SYSTEM_NAME}/*.cpp"
)

add_executable(${PROJECT_NAME} "${SOURCE_DIR}/Main.cpp" ${PROJECT_SOURCE} ${PROJECT_PRECOMPILED_HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR})
target_precompile_headers(${PROJECT_NAME} PUBLIC "${PROJECT_PRECOMPILED_HEADERS}")

if (MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/Zc:__cplusplus")
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
    # Build engine as export dll
    SND_BUILD_DLL    
    # Setup platform
    $<$<PLATFORM_ID:Windows>:SND_PLATFORM_WINDOWS>
    # Setup configuration
    $<$<CONFIG:Debug>:SND_DEBUG>
)

# Set custom binary output directory for different project configurations
foreach(CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${CONFIG_TYPE} CONFIG_TYPE_UPPER)
    set_target_properties(${PROJECT_NAME} PROPERTIES 
        ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} "${CMAKE_BINARY_DIR}/Binaries/${CMAKE_SYSTEM_NAME}/${CONFIG_TYPE}"
        LIBRARY_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} "${CMAKE_BINARY_DIR}/Binaries/${CMAKE_SYSTEM_NAME}/${CONFIG_TYPE}"
        RUNTIME_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} "${CMAKE_BINARY_DIR}/Binaries/${CMAKE_SYSTEM_NAME}/${CONFIG_TYPE}"
    )
endforeach()

set_target_properties(${PROJECT_NAME} PROPERTIES OBJECT_OUTPUT_DIR "${CMAKE_BINARY_DIR}/Intermediates/${PROJECT_NAME}")

# Add GLFW
add_subdirectory("${VENDOR_DIR}/glfw")
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

if (WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_EXPOSE_NATIVE_WIN32) # add native window acccess
endif()

# Add GLM
add_subdirectory("${VENDOR_DIR}/glm")
target_link_libraries(${PROJECT_NAME} PRIVATE glm::glm)

# Add BGFX
add_subdirectory("${VENDOR_DIR}/bgfx")
target_link_libraries(${PROJECT_NAME} PRIVATE bgfx)

# Add spdlog
add_subdirectory("${VENDOR_DIR}/spdlog")
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE) # set all log levels as active
