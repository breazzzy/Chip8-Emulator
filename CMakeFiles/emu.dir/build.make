# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/billy/Documents/proj/chip8

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/billy/Documents/proj/chip8

# Include any dependencies generated for this target.
include CMakeFiles/emu.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/emu.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/emu.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/emu.dir/flags.make

CMakeFiles/emu.dir/src/main.o: CMakeFiles/emu.dir/flags.make
CMakeFiles/emu.dir/src/main.o: src/main.cpp
CMakeFiles/emu.dir/src/main.o: CMakeFiles/emu.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/billy/Documents/proj/chip8/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/emu.dir/src/main.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/emu.dir/src/main.o -MF CMakeFiles/emu.dir/src/main.o.d -o CMakeFiles/emu.dir/src/main.o -c /home/billy/Documents/proj/chip8/src/main.cpp

CMakeFiles/emu.dir/src/main.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/emu.dir/src/main.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/billy/Documents/proj/chip8/src/main.cpp > CMakeFiles/emu.dir/src/main.i

CMakeFiles/emu.dir/src/main.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/emu.dir/src/main.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/billy/Documents/proj/chip8/src/main.cpp -o CMakeFiles/emu.dir/src/main.s

CMakeFiles/emu.dir/src/chip8.o: CMakeFiles/emu.dir/flags.make
CMakeFiles/emu.dir/src/chip8.o: src/chip8.cpp
CMakeFiles/emu.dir/src/chip8.o: CMakeFiles/emu.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/billy/Documents/proj/chip8/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/emu.dir/src/chip8.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/emu.dir/src/chip8.o -MF CMakeFiles/emu.dir/src/chip8.o.d -o CMakeFiles/emu.dir/src/chip8.o -c /home/billy/Documents/proj/chip8/src/chip8.cpp

CMakeFiles/emu.dir/src/chip8.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/emu.dir/src/chip8.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/billy/Documents/proj/chip8/src/chip8.cpp > CMakeFiles/emu.dir/src/chip8.i

CMakeFiles/emu.dir/src/chip8.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/emu.dir/src/chip8.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/billy/Documents/proj/chip8/src/chip8.cpp -o CMakeFiles/emu.dir/src/chip8.s

# Object files for target emu
emu_OBJECTS = \
"CMakeFiles/emu.dir/src/main.o" \
"CMakeFiles/emu.dir/src/chip8.o"

# External object files for target emu
emu_EXTERNAL_OBJECTS =

emu: CMakeFiles/emu.dir/src/main.o
emu: CMakeFiles/emu.dir/src/chip8.o
emu: CMakeFiles/emu.dir/build.make
emu: CMakeFiles/emu.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/billy/Documents/proj/chip8/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable emu"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/emu.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/emu.dir/build: emu
.PHONY : CMakeFiles/emu.dir/build

CMakeFiles/emu.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/emu.dir/cmake_clean.cmake
.PHONY : CMakeFiles/emu.dir/clean

CMakeFiles/emu.dir/depend:
	cd /home/billy/Documents/proj/chip8 && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/billy/Documents/proj/chip8 /home/billy/Documents/proj/chip8 /home/billy/Documents/proj/chip8 /home/billy/Documents/proj/chip8 /home/billy/Documents/proj/chip8/CMakeFiles/emu.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/emu.dir/depend

