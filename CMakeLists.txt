cmake_minimum_required(VERSION 3.20)

project(BUnknown)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
    set(PLATFORM Web CACHE STRING "raylib target platform" FORCE)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
endif()

set(BUILD_EXAMPLES OFF CACHE BOOL "Build raylib examples" FORCE)
add_subdirectory(vendor/raylib)

set(IMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/imgui")
add_library(imgui STATIC
    "${IMGUI_DIR}/imgui.cpp"
    "${IMGUI_DIR}/imgui_draw.cpp"
    "${IMGUI_DIR}/imgui_tables.cpp"
    "${IMGUI_DIR}/imgui_widgets.cpp"
)
target_include_directories(imgui PUBLIC "${IMGUI_DIR}")

set(RLIMGUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor/rlImGui")
add_library(rlImGui STATIC
    "${RLIMGUI_DIR}/rlImGui.cpp"
)
target_include_directories(rlImGui PUBLIC
    "${RLIMGUI_DIR}"
    "${IMGUI_DIR}"
)
target_link_libraries(rlImGui PUBLIC raylib imgui)

if(NOT EXISTS "${RLIMGUI_DIR}/extras/FA6FreeSolidFontData.h")
    target_compile_definitions(rlImGui PRIVATE NO_FONT_AWESOME)
endif()

add_executable(BUnknown src/main.cc)
target_link_libraries(BUnknown PRIVATE raylib imgui rlImGui)

add_custom_command(TARGET BUnknown POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/data"
        "$<TARGET_FILE_DIR:BUnknown>/data"
)
