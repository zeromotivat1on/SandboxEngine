cmake_minimum_required(VERSION 3.16)
find_package(Git REQUIRED)

option(GIT_SUBMODULE "Check submodules during build" ON)
option(COMPILE_SHADERS "Compile shaders during build" ON)

if(GIT_SUBMODULE)
    message(STATUS "Submodule update")
    execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                    RESULT_VARIABLE GIT_SUBMOD_RESULT)
    if(NOT GIT_SUBMOD_RESULT EQUAL "0")
        message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
    endif()
endif()

project(SandboxEngine VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(SHADER_DIR "${PROJECT_SOURCE_DIR}/Shaders")
set(SHADER_BINARY_DIR "${SHADER_DIR}/Bin")
set(SOURCE_DIR "${PROJECT_SOURCE_DIR}/Source")
set(VENDOR_DIR "${SOURCE_DIR}/Vendor")
set(PLATFORM_DIR "${SOURCE_DIR}/Platform")
set(PROJECT_ENGINE_DIR "${SOURCE_DIR}/SandboxEngine")

# Specify project source files
file(GLOB_RECURSE PROJECT_PRECOMPILED_HEADERS CONFIGURE_DEPENDS
    "${PROJECT_ENGINE_DIR}/*pch.h"
    "${PROJECT_ENGINE_DIR}/*pch.cpp"
)

file(GLOB_RECURSE PROJECT_SOURCE CONFIGURE_DEPENDS
    "${PROJECT_ENGINE_DIR}/*.h"
    "${PROJECT_ENGINE_DIR}/*.cpp"
    "${PLATFORM_DIR}/${CMAKE_SYSTEM_NAME}/*.h"
    "${PLATFORM_DIR}/${CMAKE_SYSTEM_NAME}/*.cpp"
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCE} ${PROJECT_PRECOMPILED_HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${VENDOR_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_ENGINE_DIR})
target_precompile_headers(${PROJECT_NAME} PUBLIC "${PROJECT_PRECOMPILED_HEADERS}")

if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/Zc:__cplusplus")
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
    # Setup platform
    $<$<PLATFORM_ID:Windows>:SND_PLATFORM_WINDOWS>
    # Setup configuration
    $<$<CONFIG:Debug>:SND_BUILD_DEBUG>
    $<$<CONFIG:Release>:SND_BUILD_RELEASE>
    # Paths
    PROJECT_SOURCE_DIR="${PROJECT_SOURCE_DIR}"
    SHADER_DIR="${SHADER_DIR}"
    SHADER_BINARY_DIR="${SHADER_BINARY_DIR}"
)

# Set custom binary output directory for different project configurations
function(set_def_output_dirs target_name)
    foreach(CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${CONFIG_TYPE} CONFIG_TYPE_UPPER)
        set_target_properties(${target_name} PROPERTIES
            ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} "${CMAKE_BINARY_DIR}/Binaries/${CMAKE_SYSTEM_NAME}/${CONFIG_TYPE}"
            LIBRARY_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} "${CMAKE_BINARY_DIR}/Binaries/${CMAKE_SYSTEM_NAME}/${CONFIG_TYPE}"
            RUNTIME_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} "${CMAKE_BINARY_DIR}/Binaries/${CMAKE_SYSTEM_NAME}/${CONFIG_TYPE}"
        )
    endforeach()
endfunction()

set_def_output_dirs(${PROJECT_NAME})

# Add googletest
add_subdirectory("${VENDOR_DIR}/googletest")
target_link_libraries(${PROJECT_NAME} PRIVATE gtest)

# Add fastdelegate
target_include_directories(${PROJECT_NAME} PRIVATE "${VENDOR_DIR}/fastdelegate/FastDelegate.h")

# Add GLFW
add_subdirectory("${VENDOR_DIR}/glfw")
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

if(WIN32)
    target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_EXPOSE_NATIVE_WIN32) # add native window acccess
endif()

# Add spdlog
add_subdirectory("${VENDOR_DIR}/spdlog")
target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog $<$<BOOL:${MINGW}>:ws2_32>)
target_compile_definitions(${PROJECT_NAME} PRIVATE SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE) # set all log levels as active

# Add BGFX
set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "Disable building bgfx examples")
add_subdirectory("${VENDOR_DIR}/bgfx")
target_link_libraries(${PROJECT_NAME} PRIVATE bgfx bx bimg)

# Add imgui
file(GLOB IMGUI_CORE_SOURCE CONFIGURE_DEPENDS
    "${VENDOR_DIR}/imgui/*.h"
    "${VENDOR_DIR}/imgui/*.cpp"
)

set(IMGUI_BACKEND_SOURCE
    "${VENDOR_DIR}/imgui/backends/imgui_impl_glfw.h"
    "${VENDOR_DIR}/imgui/backends/imgui_impl_glfw.cpp"
)

add_library(imgui STATIC ${IMGUI_CORE_SOURCE} ${IMGUI_BACKEND_SOURCE})
target_include_directories(imgui PRIVATE "${VENDOR_DIR}/imgui")
target_include_directories(imgui PRIVATE "${VENDOR_DIR}/glfw/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${VENDOR_DIR}/imgui")
set_def_output_dirs(imgui)

target_link_libraries(${PROJECT_NAME} PRIVATE imgui)

# Compile shaders
if(COMPILE_SHADERS)
    # temp shader compiler path
    set(SHADER_COMPILER "${PROJECT_SOURCE_DIR}/Build/Source/Vendor/bgfx/cmake/bgfx/Debug/shaderc.exe")

    function(compile_shader shader_name shader_type)
        set(input_file "${SHADER_DIR}/${shader_name}.sc")
        set(output_file "${SHADER_DIR}/Bin/${shader_name}.bin")

        add_custom_command(
            OUTPUT ${output_file}
            COMMAND ${SHADER_COMPILER} -f ${input_file} -o ${output_file} --type ${shader_type} --platform ${CMAKE_SYSTEM_NAME} --varying ${SHADER_DIR}/varying.def.sc --profile 150
            DEPENDS ${input_file}
            COMMENT "Compiling shader ${input_file} to ${output_file}"
        )

        list(APPEND SHADER_BINARIES ${output_file})
        set(SHADER_BINARIES ${SHADER_BINARIES} PARENT_SCOPE)
    endfunction()

    # List and compile all shaders
    file(GLOB SHADERS "${SHADER_DIR}/*.sc")
    message(STATUS "Compiling shaders from ${SHADER_DIR} using ${SHADER_COMPILER}")

    foreach(SHADER ${SHADERS})
        get_filename_component(shader_name ${SHADER} NAME_WE)
        string(REGEX MATCH "(\\.fs|\\.vs|\\.cs|\\.def)\\.sc$" shader_extension ${SHADER})

        if(shader_extension STREQUAL ".vs.sc")
            compile_shader("${shader_name}.vs" vertex)
        elseif(shader_extension STREQUAL ".fs.sc")
            compile_shader("${shader_name}.fs" fragment)
        elseif(shader_extension STREQUAL ".cs.sc")
            compile_shader($"${shader_name}.cs" compute)
        elseif(shader_extension STREQUAL ".def.sc")
            continue()
        else()
            message(FATAL_ERROR "Unsupported shader naming convention for ${SHADER}")
        endif()
    endforeach()

    add_custom_target(
        bgfx_shader_compilation ALL
        DEPENDS ${SHADER_BINARIES}
    )

    add_dependencies(bgfx_shader_compilation bgfx shaderc)
endif()

# Build tests
enable_testing()

file(GLOB_RECURSE TESTS_SOURCE CONFIGURE_DEPENDS
    "${SOURCE_DIR}/Tests/*.h"
    "${SOURCE_DIR}/Tests/*.cpp"
)

add_executable(sndtests "${TESTS_SOURCE}" "${PROJECT_ENGINE_DIR}/Core/Log.cpp")
target_include_directories(sndtests PRIVATE ${SOURCE_DIR})
target_include_directories(sndtests PRIVATE ${VENDOR_DIR})
target_include_directories(sndtests PRIVATE ${PROJECT_ENGINE_DIR})
target_include_directories(sndtests PRIVATE "${VENDOR_DIR}/spdlog/include")
target_link_libraries(sndtests GTest::gtest_main)
set_def_output_dirs(sndtests)

include(GoogleTest)
gtest_discover_tests(sndtests)
