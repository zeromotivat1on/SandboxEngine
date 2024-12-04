# Add GDL
add_subdirectory("${VENDOR_DIR}/gdl")
target_link_libraries(${PROJECT_NAME} PRIVATE gdl)
target_include_directories(${PROJECT_NAME} PRIVATE "${VENDOR_DIR}/gdl" "${VENDOR_DIR}/gdl/include")
 
# Add BGFX
add_subdirectory("${VENDOR_DIR}/bgfx")
target_link_libraries(${PROJECT_NAME} PRIVATE bgfx bx bimg)
set(BGFX_BUILD_EXAMPLES OFF CACHE BOOL "Disable building bgfx examples")

# Add imgui
file(GLOB IMGUI_CORE_SOURCE CONFIGURE_DEPENDS
    "${VENDOR_DIR}/imgui/*.h"
    "${VENDOR_DIR}/imgui/*.cpp")

add_library(imgui STATIC ${IMGUI_CORE_SOURCE} ${IMGUI_BACKEND_SOURCE})
target_include_directories(imgui PRIVATE "${VENDOR_DIR}/imgui")
target_include_directories(imgui PRIVATE "${VENDOR_DIR}/glfw/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${VENDOR_DIR}/imgui")

target_link_libraries(${PROJECT_NAME} PRIVATE imgui)

# Compile shaders
if(COMPILE_SHADERS)
    # temp shader compiler path
    set(SHADER_COMPILER "${PROJECT_SOURCE_DIR}/build/src/vendor/bgfx/cmake/bgfx/debug/shaderc.exe")

    function(compile_shader shader_name shader_type)
        set(input_file "${SHADER_DIR}/${shader_name}.sc")
        set(output_file "${SHADER_DIR}/Bin/${shader_name}.bin")

        add_custom_command(
            OUTPUT ${output_file}
            COMMAND ${SHADER_COMPILER} -f ${input_file} -o ${output_file} --type ${shader_type} --platform ${CMAKE_SYSTEM_NAME} --varying ${SHADER_DIR}/varying.def.sc --profile 150
            DEPENDS ${input_file}
            COMMENT "Compiling shader ${input_file} to ${output_file}")

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
        DEPENDS ${SHADER_BINARIES})

    add_dependencies(bgfx_shader_compilation bgfx shaderc)
endif()
