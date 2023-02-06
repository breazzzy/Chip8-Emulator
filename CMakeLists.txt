# find_package(glfw3 3.3 REQUIRED)
# find_package(OpenGL REQUIRED)
# include_directories( ${OPENGL_INCLUDE_DIRS} )


# add_executable(emu src/main.cpp src/chip8.cpp)

# target_link_libraries(emu GL)
# target_link_libraries(emu glfw3)
# target_link_libraries(emu X11)
# target_link_libraries(emu Xrandr)
# target_link_libraries(emu pthread)
# target_link_libraries(emu Xi)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(emu src/main.cpp src/chip8.cpp)
target_link_libraries(emu ${SDL2_LIBRARIES})