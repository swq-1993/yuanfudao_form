# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/sunwanqi/Documents/yuanfudao_form

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/sunwanqi/Documents/yuanfudao_form

# Include any dependencies generated for this target.
include CMakeFiles/form.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/form.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/form.dir/flags.make

CMakeFiles/form.dir/main.cpp.o: CMakeFiles/form.dir/flags.make
CMakeFiles/form.dir/main.cpp.o: main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sunwanqi/Documents/yuanfudao_form/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/form.dir/main.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/form.dir/main.cpp.o -c /Users/sunwanqi/Documents/yuanfudao_form/main.cpp

CMakeFiles/form.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/form.dir/main.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sunwanqi/Documents/yuanfudao_form/main.cpp > CMakeFiles/form.dir/main.cpp.i

CMakeFiles/form.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/form.dir/main.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sunwanqi/Documents/yuanfudao_form/main.cpp -o CMakeFiles/form.dir/main.cpp.s

CMakeFiles/form.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/form.dir/main.cpp.o.requires

CMakeFiles/form.dir/main.cpp.o.provides: CMakeFiles/form.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/form.dir/build.make CMakeFiles/form.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/form.dir/main.cpp.o.provides

CMakeFiles/form.dir/main.cpp.o.provides.build: CMakeFiles/form.dir/main.cpp.o


CMakeFiles/form.dir/formOperator.cpp.o: CMakeFiles/form.dir/flags.make
CMakeFiles/form.dir/formOperator.cpp.o: formOperator.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/sunwanqi/Documents/yuanfudao_form/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/form.dir/formOperator.cpp.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/form.dir/formOperator.cpp.o -c /Users/sunwanqi/Documents/yuanfudao_form/formOperator.cpp

CMakeFiles/form.dir/formOperator.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/form.dir/formOperator.cpp.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/sunwanqi/Documents/yuanfudao_form/formOperator.cpp > CMakeFiles/form.dir/formOperator.cpp.i

CMakeFiles/form.dir/formOperator.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/form.dir/formOperator.cpp.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/sunwanqi/Documents/yuanfudao_form/formOperator.cpp -o CMakeFiles/form.dir/formOperator.cpp.s

CMakeFiles/form.dir/formOperator.cpp.o.requires:

.PHONY : CMakeFiles/form.dir/formOperator.cpp.o.requires

CMakeFiles/form.dir/formOperator.cpp.o.provides: CMakeFiles/form.dir/formOperator.cpp.o.requires
	$(MAKE) -f CMakeFiles/form.dir/build.make CMakeFiles/form.dir/formOperator.cpp.o.provides.build
.PHONY : CMakeFiles/form.dir/formOperator.cpp.o.provides

CMakeFiles/form.dir/formOperator.cpp.o.provides.build: CMakeFiles/form.dir/formOperator.cpp.o


# Object files for target form
form_OBJECTS = \
"CMakeFiles/form.dir/main.cpp.o" \
"CMakeFiles/form.dir/formOperator.cpp.o"

# External object files for target form
form_EXTERNAL_OBJECTS =

form: CMakeFiles/form.dir/main.cpp.o
form: CMakeFiles/form.dir/formOperator.cpp.o
form: CMakeFiles/form.dir/build.make
form: CMakeFiles/form.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/sunwanqi/Documents/yuanfudao_form/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable form"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/form.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/form.dir/build: form

.PHONY : CMakeFiles/form.dir/build

CMakeFiles/form.dir/requires: CMakeFiles/form.dir/main.cpp.o.requires
CMakeFiles/form.dir/requires: CMakeFiles/form.dir/formOperator.cpp.o.requires

.PHONY : CMakeFiles/form.dir/requires

CMakeFiles/form.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/form.dir/cmake_clean.cmake
.PHONY : CMakeFiles/form.dir/clean

CMakeFiles/form.dir/depend:
	cd /Users/sunwanqi/Documents/yuanfudao_form && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/sunwanqi/Documents/yuanfudao_form /Users/sunwanqi/Documents/yuanfudao_form /Users/sunwanqi/Documents/yuanfudao_form /Users/sunwanqi/Documents/yuanfudao_form /Users/sunwanqi/Documents/yuanfudao_form/CMakeFiles/form.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/form.dir/depend

