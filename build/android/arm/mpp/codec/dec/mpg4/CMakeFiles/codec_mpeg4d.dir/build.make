# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/dzx/mpp_debug/mpp

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/dzx/mpp_debug/mpp/build/android/arm

# Include any dependencies generated for this target.
include mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/depend.make

# Include the progress variables for this target.
include mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/progress.make

# Include the compile flags for this target's objects.
include mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/flags.make

mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.o: mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/flags.make
mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.o: ../../../mpp/codec/dec/mpg4/mpg4d_api.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dzx/mpp_debug/mpp/build/android/arm/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.o"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/mpg4 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.o   -c /home/dzx/mpp_debug/mpp/mpp/codec/dec/mpg4/mpg4d_api.c

mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.i"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/mpg4 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/dzx/mpp_debug/mpp/mpp/codec/dec/mpg4/mpg4d_api.c > CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.i

mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.s"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/mpg4 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/dzx/mpp_debug/mpp/mpp/codec/dec/mpg4/mpg4d_api.c -o CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.s

mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.o.requires:
.PHONY : mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.o.requires

mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.o.provides: mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.o.requires
	$(MAKE) -f mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/build.make mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.o.provides.build
.PHONY : mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.o.provides

mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.o.provides.build: mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.o

mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.o: mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/flags.make
mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.o: ../../../mpp/codec/dec/mpg4/mpg4d_parser.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dzx/mpp_debug/mpp/build/android/arm/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.o"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/mpg4 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.o   -c /home/dzx/mpp_debug/mpp/mpp/codec/dec/mpg4/mpg4d_parser.c

mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.i"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/mpg4 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/dzx/mpp_debug/mpp/mpp/codec/dec/mpg4/mpg4d_parser.c > CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.i

mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.s"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/mpg4 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/dzx/mpp_debug/mpp/mpp/codec/dec/mpg4/mpg4d_parser.c -o CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.s

mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.o.requires:
.PHONY : mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.o.requires

mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.o.provides: mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.o.requires
	$(MAKE) -f mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/build.make mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.o.provides.build
.PHONY : mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.o.provides

mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.o.provides.build: mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.o

# Object files for target codec_mpeg4d
codec_mpeg4d_OBJECTS = \
"CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.o" \
"CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.o"

# External object files for target codec_mpeg4d
codec_mpeg4d_EXTERNAL_OBJECTS =

mpp/codec/dec/mpg4/libcodec_mpeg4d.a: mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.o
mpp/codec/dec/mpg4/libcodec_mpeg4d.a: mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.o
mpp/codec/dec/mpg4/libcodec_mpeg4d.a: mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/build.make
mpp/codec/dec/mpg4/libcodec_mpeg4d.a: mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libcodec_mpeg4d.a"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/mpg4 && $(CMAKE_COMMAND) -P CMakeFiles/codec_mpeg4d.dir/cmake_clean_target.cmake
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/mpg4 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/codec_mpeg4d.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/build: mpp/codec/dec/mpg4/libcodec_mpeg4d.a
.PHONY : mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/build

mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/requires: mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_api.c.o.requires
mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/requires: mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/mpg4d_parser.c.o.requires
.PHONY : mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/requires

mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/clean:
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/mpg4 && $(CMAKE_COMMAND) -P CMakeFiles/codec_mpeg4d.dir/cmake_clean.cmake
.PHONY : mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/clean

mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/depend:
	cd /home/dzx/mpp_debug/mpp/build/android/arm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dzx/mpp_debug/mpp /home/dzx/mpp_debug/mpp/mpp/codec/dec/mpg4 /home/dzx/mpp_debug/mpp/build/android/arm /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/mpg4 /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : mpp/codec/dec/mpg4/CMakeFiles/codec_mpeg4d.dir/depend

