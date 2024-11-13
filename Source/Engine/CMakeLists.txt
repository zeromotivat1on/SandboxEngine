# Specify project source files
# TODO: specify source manually?
file(GLOB_RECURSE ENGINE_SOURCE CONFIGURE_DEPENDS "*.h" "*.cpp")

add_executable(${PROJECT_NAME} ${ENGINE_SOURCE})
target_precompile_headers(${PROJECT_NAME} PUBLIC pch.h)
target_include_directories(${PROJECT_NAME} PRIVATE ${SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${VENDOR_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${ENGINE_DIR})

if(MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC "/Zc:__cplusplus")
    target_compile_options(${PROJECT_NAME} PUBLIC "/W3")
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
    # Platform
    $<$<PLATFORM_ID:Windows>:WIN32>
    # Configuration
    $<$<CONFIG:Debug>:BUILD_DEBUG>
    $<$<CONFIG:Release>:BUILD_RELEASE>
    # Paths
    ROOT_FOLDER_PATH="${PROJECT_SOURCE_DIR}"
    ASSET_FOLDER_PATH="${ASSET_DIR}"
    SHADER_FOLDER_PATH="${SHADER_DIR}"
    SHADER_BINARY_FOLDER_PATH="${SHADER_BINARY_DIR}")

# Set custom binary output directory for different project configurations
foreach(CONFIG_TYPE ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${CONFIG_TYPE} CONFIG_TYPE_UPPER)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} "${CMAKE_BINARY_DIR}/Binaries/${CMAKE_SYSTEM_NAME}/${CONFIG_TYPE}"
        LIBRARY_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} "${CMAKE_BINARY_DIR}/Binaries/${CMAKE_SYSTEM_NAME}/${CONFIG_TYPE}"
        RUNTIME_OUTPUT_DIRECTORY_${CONFIG_TYPE_UPPER} "${CMAKE_BINARY_DIR}/Binaries/${CMAKE_SYSTEM_NAME}/${CONFIG_TYPE}")
endforeach()
