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
include mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/depend.make

# Include the progress variables for this target.
include mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/progress.make

# Include the compile flags for this target's objects.
include mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/flags.make

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_api.c.o: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/flags.make
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_api.c.o: ../../../mpp/codec/dec/h264/h264d_api.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dzx/mpp_debug/mpp/build/android/arm/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_api.c.o"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/codec_h264d.dir/h264d_api.c.o   -c /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_api.c

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_api.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/codec_h264d.dir/h264d_api.c.i"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_api.c > CMakeFiles/codec_h264d.dir/h264d_api.c.i

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_api.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/codec_h264d.dir/h264d_api.c.s"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_api.c -o CMakeFiles/codec_h264d.dir/h264d_api.c.s

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_api.c.o.requires:
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_api.c.o.requires

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_api.c.o.provides: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_api.c.o.requires
	$(MAKE) -f mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/build.make mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_api.c.o.provides.build
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_api.c.o.provides

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_api.c.o.provides.build: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_api.c.o

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_log.c.o: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/flags.make
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_log.c.o: ../../../mpp/codec/dec/h264/h264d_log.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dzx/mpp_debug/mpp/build/android/arm/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_log.c.o"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/codec_h264d.dir/h264d_log.c.o   -c /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_log.c

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_log.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/codec_h264d.dir/h264d_log.c.i"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_log.c > CMakeFiles/codec_h264d.dir/h264d_log.c.i

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_log.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/codec_h264d.dir/h264d_log.c.s"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_log.c -o CMakeFiles/codec_h264d.dir/h264d_log.c.s

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_log.c.o.requires:
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_log.c.o.requires

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_log.c.o.provides: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_log.c.o.requires
	$(MAKE) -f mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/build.make mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_log.c.o.provides.build
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_log.c.o.provides

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_log.c.o.provides.build: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_log.c.o

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_parse.c.o: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/flags.make
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_parse.c.o: ../../../mpp/codec/dec/h264/h264d_parse.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dzx/mpp_debug/mpp/build/android/arm/CMakeFiles $(CMAKE_PROGRESS_3)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_parse.c.o"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/codec_h264d.dir/h264d_parse.c.o   -c /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_parse.c

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_parse.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/codec_h264d.dir/h264d_parse.c.i"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_parse.c > CMakeFiles/codec_h264d.dir/h264d_parse.c.i

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_parse.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/codec_h264d.dir/h264d_parse.c.s"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_parse.c -o CMakeFiles/codec_h264d.dir/h264d_parse.c.s

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_parse.c.o.requires:
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_parse.c.o.requires

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_parse.c.o.provides: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_parse.c.o.requires
	$(MAKE) -f mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/build.make mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_parse.c.o.provides.build
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_parse.c.o.provides

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_parse.c.o.provides.build: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_parse.c.o

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_slice.c.o: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/flags.make
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_slice.c.o: ../../../mpp/codec/dec/h264/h264d_slice.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dzx/mpp_debug/mpp/build/android/arm/CMakeFiles $(CMAKE_PROGRESS_4)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_slice.c.o"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/codec_h264d.dir/h264d_slice.c.o   -c /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_slice.c

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_slice.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/codec_h264d.dir/h264d_slice.c.i"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_slice.c > CMakeFiles/codec_h264d.dir/h264d_slice.c.i

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_slice.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/codec_h264d.dir/h264d_slice.c.s"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_slice.c -o CMakeFiles/codec_h264d.dir/h264d_slice.c.s

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_slice.c.o.requires:
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_slice.c.o.requires

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_slice.c.o.provides: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_slice.c.o.requires
	$(MAKE) -f mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/build.make mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_slice.c.o.provides.build
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_slice.c.o.provides

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_slice.c.o.provides.build: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_slice.c.o

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sps.c.o: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/flags.make
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sps.c.o: ../../../mpp/codec/dec/h264/h264d_sps.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dzx/mpp_debug/mpp/build/android/arm/CMakeFiles $(CMAKE_PROGRESS_5)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sps.c.o"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/codec_h264d.dir/h264d_sps.c.o   -c /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_sps.c

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sps.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/codec_h264d.dir/h264d_sps.c.i"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_sps.c > CMakeFiles/codec_h264d.dir/h264d_sps.c.i

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sps.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/codec_h264d.dir/h264d_sps.c.s"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_sps.c -o CMakeFiles/codec_h264d.dir/h264d_sps.c.s

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sps.c.o.requires:
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sps.c.o.requires

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sps.c.o.provides: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sps.c.o.requires
	$(MAKE) -f mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/build.make mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sps.c.o.provides.build
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sps.c.o.provides

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sps.c.o.provides.build: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sps.c.o

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_pps.c.o: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/flags.make
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_pps.c.o: ../../../mpp/codec/dec/h264/h264d_pps.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dzx/mpp_debug/mpp/build/android/arm/CMakeFiles $(CMAKE_PROGRESS_6)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_pps.c.o"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/codec_h264d.dir/h264d_pps.c.o   -c /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_pps.c

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_pps.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/codec_h264d.dir/h264d_pps.c.i"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_pps.c > CMakeFiles/codec_h264d.dir/h264d_pps.c.i

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_pps.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/codec_h264d.dir/h264d_pps.c.s"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_pps.c -o CMakeFiles/codec_h264d.dir/h264d_pps.c.s

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_pps.c.o.requires:
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_pps.c.o.requires

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_pps.c.o.provides: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_pps.c.o.requires
	$(MAKE) -f mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/build.make mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_pps.c.o.provides.build
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_pps.c.o.provides

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_pps.c.o.provides.build: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_pps.c.o

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_scalist.c.o: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/flags.make
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_scalist.c.o: ../../../mpp/codec/dec/h264/h264d_scalist.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dzx/mpp_debug/mpp/build/android/arm/CMakeFiles $(CMAKE_PROGRESS_7)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_scalist.c.o"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/codec_h264d.dir/h264d_scalist.c.o   -c /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_scalist.c

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_scalist.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/codec_h264d.dir/h264d_scalist.c.i"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_scalist.c > CMakeFiles/codec_h264d.dir/h264d_scalist.c.i

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_scalist.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/codec_h264d.dir/h264d_scalist.c.s"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_scalist.c -o CMakeFiles/codec_h264d.dir/h264d_scalist.c.s

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_scalist.c.o.requires:
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_scalist.c.o.requires

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_scalist.c.o.provides: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_scalist.c.o.requires
	$(MAKE) -f mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/build.make mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_scalist.c.o.provides.build
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_scalist.c.o.provides

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_scalist.c.o.provides.build: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_scalist.c.o

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sei.c.o: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/flags.make
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sei.c.o: ../../../mpp/codec/dec/h264/h264d_sei.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dzx/mpp_debug/mpp/build/android/arm/CMakeFiles $(CMAKE_PROGRESS_8)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sei.c.o"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/codec_h264d.dir/h264d_sei.c.o   -c /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_sei.c

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sei.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/codec_h264d.dir/h264d_sei.c.i"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_sei.c > CMakeFiles/codec_h264d.dir/h264d_sei.c.i

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sei.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/codec_h264d.dir/h264d_sei.c.s"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_sei.c -o CMakeFiles/codec_h264d.dir/h264d_sei.c.s

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sei.c.o.requires:
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sei.c.o.requires

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sei.c.o.provides: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sei.c.o.requires
	$(MAKE) -f mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/build.make mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sei.c.o.provides.build
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sei.c.o.provides

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sei.c.o.provides.build: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sei.c.o

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_dpb.c.o: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/flags.make
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_dpb.c.o: ../../../mpp/codec/dec/h264/h264d_dpb.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dzx/mpp_debug/mpp/build/android/arm/CMakeFiles $(CMAKE_PROGRESS_9)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_dpb.c.o"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/codec_h264d.dir/h264d_dpb.c.o   -c /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_dpb.c

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_dpb.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/codec_h264d.dir/h264d_dpb.c.i"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_dpb.c > CMakeFiles/codec_h264d.dir/h264d_dpb.c.i

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_dpb.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/codec_h264d.dir/h264d_dpb.c.s"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_dpb.c -o CMakeFiles/codec_h264d.dir/h264d_dpb.c.s

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_dpb.c.o.requires:
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_dpb.c.o.requires

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_dpb.c.o.provides: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_dpb.c.o.requires
	$(MAKE) -f mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/build.make mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_dpb.c.o.provides.build
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_dpb.c.o.provides

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_dpb.c.o.provides.build: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_dpb.c.o

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_init.c.o: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/flags.make
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_init.c.o: ../../../mpp/codec/dec/h264/h264d_init.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dzx/mpp_debug/mpp/build/android/arm/CMakeFiles $(CMAKE_PROGRESS_10)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_init.c.o"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/codec_h264d.dir/h264d_init.c.o   -c /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_init.c

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_init.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/codec_h264d.dir/h264d_init.c.i"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_init.c > CMakeFiles/codec_h264d.dir/h264d_init.c.i

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_init.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/codec_h264d.dir/h264d_init.c.s"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_init.c -o CMakeFiles/codec_h264d.dir/h264d_init.c.s

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_init.c.o.requires:
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_init.c.o.requires

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_init.c.o.provides: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_init.c.o.requires
	$(MAKE) -f mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/build.make mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_init.c.o.provides.build
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_init.c.o.provides

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_init.c.o.provides.build: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_init.c.o

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_fill.c.o: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/flags.make
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_fill.c.o: ../../../mpp/codec/dec/h264/h264d_fill.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dzx/mpp_debug/mpp/build/android/arm/CMakeFiles $(CMAKE_PROGRESS_11)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_fill.c.o"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/codec_h264d.dir/h264d_fill.c.o   -c /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_fill.c

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_fill.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/codec_h264d.dir/h264d_fill.c.i"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_fill.c > CMakeFiles/codec_h264d.dir/h264d_fill.c.i

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_fill.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/codec_h264d.dir/h264d_fill.c.s"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_fill.c -o CMakeFiles/codec_h264d.dir/h264d_fill.c.s

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_fill.c.o.requires:
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_fill.c.o.requires

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_fill.c.o.provides: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_fill.c.o.requires
	$(MAKE) -f mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/build.make mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_fill.c.o.provides.build
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_fill.c.o.provides

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_fill.c.o.provides.build: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_fill.c.o

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_rwfile.c.o: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/flags.make
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_rwfile.c.o: ../../../mpp/codec/dec/h264/h264d_rwfile.c
	$(CMAKE_COMMAND) -E cmake_progress_report /home/dzx/mpp_debug/mpp/build/android/arm/CMakeFiles $(CMAKE_PROGRESS_12)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building C object mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_rwfile.c.o"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -o CMakeFiles/codec_h264d.dir/h264d_rwfile.c.o   -c /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_rwfile.c

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_rwfile.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/codec_h264d.dir/h264d_rwfile.c.i"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -E /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_rwfile.c > CMakeFiles/codec_h264d.dir/h264d_rwfile.c.i

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_rwfile.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/codec_h264d.dir/h264d_rwfile.c.s"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && /home/pub/ndk/android-ndk-r10d/toolchains/arm-linux-androideabi-4.8/prebuilt/linux-x86_64/bin/arm-linux-androideabi-gcc  $(C_DEFINES) $(C_FLAGS) -S /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264/h264d_rwfile.c -o CMakeFiles/codec_h264d.dir/h264d_rwfile.c.s

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_rwfile.c.o.requires:
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_rwfile.c.o.requires

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_rwfile.c.o.provides: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_rwfile.c.o.requires
	$(MAKE) -f mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/build.make mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_rwfile.c.o.provides.build
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_rwfile.c.o.provides

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_rwfile.c.o.provides.build: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_rwfile.c.o

# Object files for target codec_h264d
codec_h264d_OBJECTS = \
"CMakeFiles/codec_h264d.dir/h264d_api.c.o" \
"CMakeFiles/codec_h264d.dir/h264d_log.c.o" \
"CMakeFiles/codec_h264d.dir/h264d_parse.c.o" \
"CMakeFiles/codec_h264d.dir/h264d_slice.c.o" \
"CMakeFiles/codec_h264d.dir/h264d_sps.c.o" \
"CMakeFiles/codec_h264d.dir/h264d_pps.c.o" \
"CMakeFiles/codec_h264d.dir/h264d_scalist.c.o" \
"CMakeFiles/codec_h264d.dir/h264d_sei.c.o" \
"CMakeFiles/codec_h264d.dir/h264d_dpb.c.o" \
"CMakeFiles/codec_h264d.dir/h264d_init.c.o" \
"CMakeFiles/codec_h264d.dir/h264d_fill.c.o" \
"CMakeFiles/codec_h264d.dir/h264d_rwfile.c.o"

# External object files for target codec_h264d
codec_h264d_EXTERNAL_OBJECTS =

mpp/codec/dec/h264/libcodec_h264d.a: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_api.c.o
mpp/codec/dec/h264/libcodec_h264d.a: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_log.c.o
mpp/codec/dec/h264/libcodec_h264d.a: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_parse.c.o
mpp/codec/dec/h264/libcodec_h264d.a: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_slice.c.o
mpp/codec/dec/h264/libcodec_h264d.a: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sps.c.o
mpp/codec/dec/h264/libcodec_h264d.a: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_pps.c.o
mpp/codec/dec/h264/libcodec_h264d.a: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_scalist.c.o
mpp/codec/dec/h264/libcodec_h264d.a: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sei.c.o
mpp/codec/dec/h264/libcodec_h264d.a: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_dpb.c.o
mpp/codec/dec/h264/libcodec_h264d.a: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_init.c.o
mpp/codec/dec/h264/libcodec_h264d.a: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_fill.c.o
mpp/codec/dec/h264/libcodec_h264d.a: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_rwfile.c.o
mpp/codec/dec/h264/libcodec_h264d.a: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/build.make
mpp/codec/dec/h264/libcodec_h264d.a: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library libcodec_h264d.a"
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && $(CMAKE_COMMAND) -P CMakeFiles/codec_h264d.dir/cmake_clean_target.cmake
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/codec_h264d.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/build: mpp/codec/dec/h264/libcodec_h264d.a
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/build

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/requires: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_api.c.o.requires
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/requires: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_log.c.o.requires
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/requires: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_parse.c.o.requires
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/requires: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_slice.c.o.requires
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/requires: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sps.c.o.requires
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/requires: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_pps.c.o.requires
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/requires: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_scalist.c.o.requires
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/requires: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_sei.c.o.requires
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/requires: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_dpb.c.o.requires
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/requires: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_init.c.o.requires
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/requires: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_fill.c.o.requires
mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/requires: mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/h264d_rwfile.c.o.requires
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/requires

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/clean:
	cd /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 && $(CMAKE_COMMAND) -P CMakeFiles/codec_h264d.dir/cmake_clean.cmake
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/clean

mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/depend:
	cd /home/dzx/mpp_debug/mpp/build/android/arm && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/dzx/mpp_debug/mpp /home/dzx/mpp_debug/mpp/mpp/codec/dec/h264 /home/dzx/mpp_debug/mpp/build/android/arm /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264 /home/dzx/mpp_debug/mpp/build/android/arm/mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : mpp/codec/dec/h264/CMakeFiles/codec_h264d.dir/depend

