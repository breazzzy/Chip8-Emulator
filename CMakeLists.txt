project(Chip8EMU)
cmake_minimum_required(VERSION 3.2)

set(sources
    imgui/imconfig.h
    imgui/imgui.h
    imgui/imgui.cpp
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_internal.h
    imgui/imgui_widgets.cpp
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h
    imgui/imgui_tables.cpp
    imgui/imgui_impl_sdlrenderer.cpp
    imgui/imgui_impl_sdlrenderer.h
    imgui/imgui_impl_sdl2.cpp
    imgui/imgui_impl_sdl2.h
)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})   

add_executable(Chip8EMU src/main.cpp src/chip8.cpp ${sources})
target_link_libraries(Chip8EMU ${SDL2_LIBRARIES})
