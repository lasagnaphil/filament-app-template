cmake_minimum_required(VERSION 3.10)
project(filament_app_template)
enable_language(CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (UNIX AND NOT APPLE AND NOT ANDROID AND NOT WEBGL)
    set(LINUX TRUE)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -fno-builtin")

find_package(Threads REQUIRED)

add_subdirectory(deps)

set(ROOT_DIR ${CMAKE_SOURCE_DIR})

set(MATC_BIN ${ROOT_DIR}/deps/filament/bin/matc)
set(RESGEN_BIN ${ROOT_DIR}/deps/filament/bin/resgen)
set(CMGEN_BIN ${ROOT_DIR}/deps/filament/bin/cmgen)
set(MIPGEN_BIN ${ROOT_DIR}/deps/filament/bin/mipgen)
set(FILAMESH_BIN ${ROOT_DIR}/deps/filament/bin/filamesh)

set(MATERIAL_DIR ${ROOT_DIR}/generated/materials)
set(RESOURCE_DIR ${ROOT_DIR}/generated/resources)
set(TEXTURE_DIR ${ROOT_DIR}/generated/textures)
set(MESH_DIR ${ROOT_DIR}/generated/meshes)
set(ENVMAP_DIR ${ROOT_DIR}/generated/envmap)

file(MAKE_DIRECTORY ${ROOT_DIR}/generated)
file(MAKE_DIRECTORY ${MATERIAL_DIR})
file(MAKE_DIRECTORY ${RESOURCE_DIR})
file(MAKE_DIRECTORY ${TEXTURE_DIR})
file(MAKE_DIRECTORY ${MESH_DIR})
file(MAKE_DIRECTORY ${ENVMAP_DIR})

set(RESOURCE_BINS)

function(add_material MAT_SRC)
    get_filename_component(localname "${MAT_SRC}" NAME_WE)
    get_filename_component(fullname "${MAT_SRC}" ABSOLUTE)
    set(output_path "${MATERIAL_DIR}/${localname}.filamat")
    add_custom_command(
            OUTPUT ${output_path}
            COMMAND ${MATC_BIN} ${MATC_BASE_FLAGS} -o ${output_path} ${fullname}
            MAIN_DEPENDENCY ${MAT_SRC}
            COMMENT "Compiling material ${MAT_SRC} to ${output_path}"
    )
    set(RESOURCE_BINS ${RESOURCE_BINS} ${output_path} PARENT_SCOPE)
endfunction()

function(get_resgen_vars ARCHIVE_DIR ARCHIVE_NAME)
    set(OUTPUTS
            ${ARCHIVE_DIR}/${ARCHIVE_NAME}.bin
            ${ARCHIVE_DIR}/${ARCHIVE_NAME}.S
            ${ARCHIVE_DIR}/${ARCHIVE_NAME}.apple.S
            ${ARCHIVE_DIR}/${ARCHIVE_NAME}.h
            )
    if (IOS)
        set(ASM_ARCH_FLAG "-arch ${DIST_ARCH}")
    endif()
    if (APPLE)
        set(ASM_SUFFIX ".apple")
    endif()
    set(RESGEN_HEADER "${ARCHIVE_DIR}/${ARCHIVE_NAME}.h" PARENT_SCOPE)
    # Visual Studio makes it difficult to use assembly without using MASM. MASM doesn't support
    # the equivalent of .incbin, so on Windows we'll just tell resgen to output a C file.
    if (WEBGL OR WIN32 OR ANDROID_ON_WINDOWS)
        set(RESGEN_OUTPUTS "${OUTPUTS};${ARCHIVE_DIR}/${ARCHIVE_NAME}.c" PARENT_SCOPE)
        set(RESGEN_FLAGS -qcx ${ARCHIVE_DIR} -p ${ARCHIVE_NAME} PARENT_SCOPE)
        set(RESGEN_SOURCE "${ARCHIVE_DIR}/${ARCHIVE_NAME}.c" PARENT_SCOPE)
    else()
        set(RESGEN_OUTPUTS "${OUTPUTS}" PARENT_SCOPE)
        set(RESGEN_FLAGS -qx ${ARCHIVE_DIR} -p ${ARCHIVE_NAME} PARENT_SCOPE)
        set(RESGEN_SOURCE "${ARCHIVE_DIR}/${ARCHIVE_NAME}${ASM_SUFFIX}.S" PARENT_SCOPE)
        set(RESGEN_SOURCE_FLAGS "-I${ARCHIVE_DIR} ${ASM_ARCH_FLAG}" PARENT_SCOPE)
    endif()
endfunction()

set(CMGEN_ARGS --quiet --format=ktx --size=256 --extract-blur=0.1)

function(add_envmap SOURCE TARGET)
    set(source_envmap "${ROOT_DIR}/${SOURCE}")

    file(MAKE_DIRECTORY "${ENVMAP_DIR}/${TARGET}")
    set(target_skybox   "${ENVMAP_DIR}/${TARGET}/${TARGET}_skybox.ktx")
    set(target_envmap   "${ENVMAP_DIR}/${TARGET}/${TARGET}_ibl.ktx")

    add_custom_command(OUTPUT ${target_skybox} ${target_envmap}
            COMMAND ${CMGEN_BIN} -x "${ENVMAP_DIR}/${TARGET}" ${CMGEN_ARGS} ${source_envmap}
            MAIN_DEPENDENCY ${source_envmap}
            COMMENT "Generating environment map ${target_envmap}")
    set(RESOURCE_BINS ${RESOURCE_BINS} ${target_skybox} ${target_envmap} PARENT_SCOPE)
endfunction()

function(add_ktxfiles SOURCE TARGET EXTRA_ARGS)
    set(source_path "${ROOT_DIR}/${SOURCE}")
    set(target_path "${TEXTURE_DIR}/${TARGET}")
    set(MONKEY_FILES ${MONKEY_FILES} ${target_path} PARENT_SCOPE)
    add_custom_command(
            OUTPUT ${target_path}
            COMMAND ${MIPGEN_BIN} --quiet --strip-alpha ${EXTRA_ARGS} ${source_path} ${target_path}
            MAIN_DEPENDENCY ${source_path}
            COMMENT "Generating mip levels for ${source_path}")
    set(RESOURCE_BINS ${RESOURCE_BINS} ${target_path} PARENT_SCOPE)
endfunction()

function(add_pngfile SOURCE TARGET)
    set(source_path "${ROOT_DIR}/${SOURCE}")
    set(target_path "${TEXTURE_DIR}/${TARGET}")
    add_custom_command(
            OUTPUT ${target_path}
            COMMAND ${CMAKE_COMMAND} -E copy ${source_path} ${target_path}
            MAIN_DEPENDENCY ${source_path})
    set(RESOURCE_BINS ${RESOURCE_BINS} ${target_path} PARENT_SCOPE)
endfunction()

function(add_mesh SOURCE TARGET)
    set(source_mesh "${ROOT_DIR}/${SOURCE}")
    set(target_mesh "${MESH_DIR}/${TARGET}")
    add_custom_command(
            OUTPUT ${target_mesh}
            COMMAND ${FILAMESH_BIN} --compress ${source_mesh} ${target_mesh}
            MAIN_DEPENDENCY ${source_mesh})
    set(RESOURCE_BINS ${RESOURCE_BINS} ${target_mesh} PARENT_SCOPE)
endfunction()

##
## Compile assets
##

add_material("assets/materials/aiDefaultMat.mat")
add_material("assets/materials/aiDefaultTrans.mat")
add_material("assets/materials/bakedColor.mat")
add_material("assets/materials/depthVisualizer.mat")
add_material("assets/materials/transparentColor.mat")
add_material("assets/materials/uiBlit.mat")

add_envmap("assets/environments/lightroom_14b.hdr" "default_env")

add_mesh("assets/models/monkey/monkey.obj" "suzanne.filamesh")

add_ktxfiles("assets/models/monkey/albedo.png" "albedo_s3tc.ktx" "--compression=s3tc_rgb_dxt1")
add_ktxfiles("assets/models/monkey/roughness.png" "roughness.ktx" "--grayscale")
add_ktxfiles("assets/models/monkey/metallic.png" "metallic.ktx" "--grayscale")
add_ktxfiles("assets/models/monkey/ao.png" "ao.ktx" "--grayscale")

add_pngfile("assets/models/monkey/normal.png" "normal.png")

##
## Generate resources
##

get_resgen_vars(${RESOURCE_DIR} resources)
add_custom_command(
        OUTPUT ${RESGEN_OUTPUTS}
        COMMAND ${RESGEN_BIN} ${RESGEN_FLAGS} ${RESOURCE_BINS}
        DEPENDS ${RESOURCE_BINS}
        COMMENT "Aggregating resources"
)
if (DEFINED RESGEN_SOURCE_FLAGS)
    set_source_files_properties(${RESGEN_SOURCE} PROPERTIES COMPILE_FLAGS ${RESGEN_SOURCE_FLAGS})
endif()

##
## Compile executable
##

set(FILAMENTAPP_DIR deps/filamentapp)
set(FILAMENTAPP_INCLUDE_DIR deps/filamentapp/include)
set(FILAMENTAPP_SRCS
        ${FILAMENTAPP_DIR}/src/Cube.cpp
        ${FILAMENTAPP_DIR}/src/FilamentApp.cpp
        ${FILAMENTAPP_DIR}/src/IBL.cpp
        ${FILAMENTAPP_DIR}/src/IcoSphere.cpp
        ${FILAMENTAPP_DIR}/src/Image.cpp
        ${FILAMENTAPP_DIR}/src/MeshAssimp.cpp
        ${FILAMENTAPP_DIR}/src/Sphere.cpp
        ${FILAMENTAPP_DIR}/src/EasyApp.cpp)

if (WIN32)
    set(FILAMENTAPP_SRCS ${FILAMENTAPP_SRCS} ${FILAMENTAPP_DIR}/src/NativeWindowHelperWindows.cpp)
endif()
if (LINUX)
    set(FILAMENTAPP_SRCS ${FILAMENTAPP_SRCS} ${FILAMENTAPP_DIR}/src/NativeWindowHelperLinux.cpp)
endif()
if (APPLE)
    set(FILAMENTAPP_SRCS ${FILAMENTAPP_SRCS} ${FILAMENTAPP_DIR}/src/NativeWindowHelperCocoa.mm)
endif()

set(FILAGUI_DIR deps/filagui)
set(FILAGUI_INCLUDE_DIR deps/filagui/include)
set(FILAGUI_SRCS
        ${FILAGUI_DIR}/src/ImGuiExtensions.cpp
        ${FILAGUI_DIR}/src/ImGuiHelper.cpp)

set(APP_SRCS src/main.cpp)

add_executable(filament_app_template ${APP_SRCS} ${FILAMENTAPP_SRCS} ${FILAGUI_SRCS} ${RESGEN_SOURCE})
target_link_libraries(filament_app_template PRIVATE
        filament
        pthread c++ c++abi dl
        imgui implot stb assimp sdl2)
target_include_directories(filament_app_template PRIVATE
        include ${RESOURCE_DIR} ${FILAMENTAPP_INCLUDE_DIR} ${FILAGUI_INCLUDE_DIR})
target_compile_definitions(filament_app_template PRIVATE -DRELATIVE_ASSET_PATH="..")