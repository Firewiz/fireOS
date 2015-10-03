# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.0

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:
.PHONY : .NOTPARALLEL

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

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/beebop/src/fireos

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/beebop/src/fireos

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/beebop/src/fireos/CMakeFiles /home/beebop/src/fireos/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/beebop/src/fireos/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named c

# Build rule for target.
c : cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 c
.PHONY : c

# fast build rule for target.
c/fast:
	$(MAKE) -f src/CMakeFiles/c.dir/build.make src/CMakeFiles/c.dir/build
.PHONY : c/fast

#=============================================================================
# Target rules for targets named fireos.bin

# Build rule for target.
fireos.bin: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 fireos.bin
.PHONY : fireos.bin

# fast build rule for target.
fireos.bin/fast:
	$(MAKE) -f src/CMakeFiles/fireos.bin.dir/build.make src/CMakeFiles/fireos.bin.dir/build
.PHONY : fireos.bin/fast

#=============================================================================
# Target rules for targets named init.exe

# Build rule for target.
init.exe: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 init.exe
.PHONY : init.exe

# fast build rule for target.
init.exe/fast:
	$(MAKE) -f src/CMakeFiles/init.exe.dir/build.make src/CMakeFiles/init.exe.dir/build
.PHONY : init.exe/fast

#=============================================================================
# Target rules for targets named version

# Build rule for target.
version: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 version
.PHONY : version

# fast build rule for target.
version/fast:
	$(MAKE) -f src/CMakeFiles/version.dir/build.make src/CMakeFiles/version.dir/build
.PHONY : version/fast

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... c"
	@echo "... fireos.bin"
	@echo "... init.exe"
	@echo "... version"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

