# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/host/bin/cmake

# The command to remove a file.
RM = /opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/host/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-lzo-2.10

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-lzo-2.10/buildroot-build

# Include any dependencies generated for this target.
include CMakeFiles/overlap.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/overlap.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/overlap.dir/flags.make

CMakeFiles/overlap.dir/examples/overlap.c.o: CMakeFiles/overlap.dir/flags.make
CMakeFiles/overlap.dir/examples/overlap.c.o: ../examples/overlap.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --progress-dir=/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-lzo-2.10/buildroot-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/overlap.dir/examples/overlap.c.o"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/overlap.dir/examples/overlap.c.o   -c /opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-lzo-2.10/examples/overlap.c

CMakeFiles/overlap.dir/examples/overlap.c.i: cmake_force
	@echo "Preprocessing C source to CMakeFiles/overlap.dir/examples/overlap.c.i"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-lzo-2.10/examples/overlap.c > CMakeFiles/overlap.dir/examples/overlap.c.i

CMakeFiles/overlap.dir/examples/overlap.c.s: cmake_force
	@echo "Compiling C source to assembly CMakeFiles/overlap.dir/examples/overlap.c.s"
	/usr/bin/gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-lzo-2.10/examples/overlap.c -o CMakeFiles/overlap.dir/examples/overlap.c.s

CMakeFiles/overlap.dir/examples/overlap.c.o.requires:

.PHONY : CMakeFiles/overlap.dir/examples/overlap.c.o.requires

CMakeFiles/overlap.dir/examples/overlap.c.o.provides: CMakeFiles/overlap.dir/examples/overlap.c.o.requires
	$(MAKE) -f CMakeFiles/overlap.dir/build.make CMakeFiles/overlap.dir/examples/overlap.c.o.provides.build
.PHONY : CMakeFiles/overlap.dir/examples/overlap.c.o.provides

CMakeFiles/overlap.dir/examples/overlap.c.o.provides.build: CMakeFiles/overlap.dir/examples/overlap.c.o


# Object files for target overlap
overlap_OBJECTS = \
"CMakeFiles/overlap.dir/examples/overlap.c.o"

# External object files for target overlap
overlap_EXTERNAL_OBJECTS =

overlap: CMakeFiles/overlap.dir/examples/overlap.c.o
overlap: CMakeFiles/overlap.dir/build.make
overlap: liblzo2.so.2.0.0
overlap: CMakeFiles/overlap.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --progress-dir=/opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-lzo-2.10/buildroot-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable overlap"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/overlap.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/overlap.dir/build: overlap

.PHONY : CMakeFiles/overlap.dir/build

CMakeFiles/overlap.dir/requires: CMakeFiles/overlap.dir/examples/overlap.c.o.requires

.PHONY : CMakeFiles/overlap.dir/requires

CMakeFiles/overlap.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/overlap.dir/cmake_clean.cmake
.PHONY : CMakeFiles/overlap.dir/clean

CMakeFiles/overlap.dir/depend:
	cd /opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-lzo-2.10/buildroot-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-lzo-2.10 /opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-lzo-2.10 /opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-lzo-2.10/buildroot-build /opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-lzo-2.10/buildroot-build /opt/mscc/mscc-brsdk-source-2019.02.7-232/mscc_stage1_servalt_defconfig/build/host-lzo-2.10/buildroot-build/CMakeFiles/overlap.dir/DependInfo.cmake
.PHONY : CMakeFiles/overlap.dir/depend

