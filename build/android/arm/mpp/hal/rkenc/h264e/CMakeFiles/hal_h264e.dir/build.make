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
include mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/depend.make

# Include the progress variables for this target.
include mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/progress.make

# Include the compile flags for this target's objects.
include mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/flags.make

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_api.c.o: mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/flags.make
mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_api.c.o: ../../../mpp/hal/rkenc/h264e/hal_h264e_api.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dzx/mpp_debug/mpp/build/android/arm/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_api.c.o"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/hal/rkenc/h264e && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/hal_h264e.dir/hal_h264e_api.c.o   -c /home/dzx/mpp_debug/mpp/mpp/hal/rkenc/h264e/hal_h264e_api.c

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_api.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hal_h264e.dir/hal_h264e_api.c.i"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/hal/rkenc/h264e && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/dzx/mpp_debug/mpp/mpp/hal/rkenc/h264e/hal_h264e_api.c > CMakeFiles/hal_h264e.dir/hal_h264e_api.c.i

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_api.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hal_h264e.dir/hal_h264e_api.c.s"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/hal/rkenc/h264e && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/dzx/mpp_debug/mpp/mpp/hal/rkenc/h264e/hal_h264e_api.c -o CMakeFiles/hal_h264e.dir/hal_h264e_api.c.s

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_api.c.o.requires:
.PHONY : mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_api.c.o.requires

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_api.c.o.provides: mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_api.c.o.requires
	$(MAKE) -f mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/build.make mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_api.c.o.provides.build
.PHONY : mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_api.c.o.provides

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_api.c.o.provides.build: mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_api.c.o

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.o: mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/flags.make
mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.o: ../../../mpp/hal/rkenc/h264e/hal_h264e_vpu.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dzx/mpp_debug/mpp/build/android/arm/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.o"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/hal/rkenc/h264e && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.o   -c /home/dzx/mpp_debug/mpp/mpp/hal/rkenc/h264e/hal_h264e_vpu.c

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.i"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/hal/rkenc/h264e && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/dzx/mpp_debug/mpp/mpp/hal/rkenc/h264e/hal_h264e_vpu.c > CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.i

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.s"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/hal/rkenc/h264e && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/dzx/mpp_debug/mpp/mpp/hal/rkenc/h264e/hal_h264e_vpu.c -o CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.s

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.o.requires:
.PHONY : mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.o.requires

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.o.provides: mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.o.requires
	$(MAKE) -f mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/build.make mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.o.provides.build
.PHONY : mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.o.provides

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.o.provides.build: mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.o

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.o: mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/flags.make
mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.o: ../../../mpp/hal/rkenc/h264e/hal_h264e_rkv.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dzx/mpp_debug/mpp/build/android/arm/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.o"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/hal/rkenc/h264e && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.o   -c /home/dzx/mpp_debug/mpp/mpp/hal/rkenc/h264e/hal_h264e_rkv.c

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.i"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/hal/rkenc/h264e && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/dzx/mpp_debug/mpp/mpp/hal/rkenc/h264e/hal_h264e_rkv.c > CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.i

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.s"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/hal/rkenc/h264e && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/dzx/mpp_debug/mpp/mpp/hal/rkenc/h264e/hal_h264e_rkv.c -o CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.s

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.o.requires:
.PHONY : mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.o.requires

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.o.provides: mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.o.requires
	$(MAKE) -f mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/build.make mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.o.provides.build
.PHONY : mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.o.provides

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.o.provides.build: mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.o

# Object files for target hal_h264e
hal_h264e_OBJECTS = \
"CMakeFiles/hal_h264e.dir/hal_h264e_api.c.o" \
"CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.o" \
"CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.o"

# External object files for target hal_h264e
hal_h264e_EXTERNAL_OBJECTS =

mpp/hal/rkenc/h264e/libhal_h264e.a: mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_api.c.o
mpp/hal/rkenc/h264e/libhal_h264e.a: mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.o
mpp/hal/rkenc/h264e/libhal_h264e.a: mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.o
mpp/hal/rkenc/h264e/libhal_h264e.a: mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/build.make
mpp/hal/rkenc/h264e/libhal_h264e.a: mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libhal_h264e.a"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/hal/rkenc/h264e && $(CMAKE_COMMAND) -P CMakeFiles/hal_h264e.dir/cmake_clean_target.cmake
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/hal/rkenc/h264e && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hal_h264e.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/build: mpp/hal/rkenc/h264e/libhal_h264e.a
.PHONY : mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/build

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/requires: mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_api.c.o.requires
mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/requires: mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_vpu.c.o.requires
mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/requires: mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/hal_h264e_rkv.c.o.requires
.PHONY : mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/requires

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/clean:
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/hal/rkenc/h264e && $(CMAKE_COMMAND) -P CMakeFiles/hal_h264e.dir/cmake_clean.cmake
.PHONY : mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/clean

mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/depend:
	cd /home/dzx/mpp_debug/mpp/build/android/arm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dzx/mpp_debug/mpp /home/dzx/mpp_debug/mpp/mpp/hal/rkenc/h264e /home/dzx/mpp_debug/mpp/build/android/arm /home/dzx/mpp_debug/mpp/build/android/arm/mpp/hal/rkenc/h264e /home/dzx/mpp_debug/mpp/build/android/arm/mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : mpp/hal/rkenc/h264e/CMakeFiles/hal_h264e.dir/depend

