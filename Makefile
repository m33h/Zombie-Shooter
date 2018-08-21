# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.11

# Default target executed when no arguments are given to make.
default_target: all

.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:


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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/x1/Dokumenty/C++/zombie-shooter

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/x1/Dokumenty/C++/zombie-shooter

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target install/strip
install/strip: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/local/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip

# Special rule for the target install/strip
install/strip/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing the project stripped..."
	/usr/local/bin/cmake -DCMAKE_INSTALL_DO_STRIP=1 -P cmake_install.cmake
.PHONY : install/strip/fast

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/local/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache

.PHONY : edit_cache/fast

# Special rule for the target install
install: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/local/bin/cmake -P cmake_install.cmake
.PHONY : install

# Special rule for the target install
install/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Install the project..."
	/usr/local/bin/cmake -P cmake_install.cmake
.PHONY : install/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/local/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache

.PHONY : rebuild_cache/fast

# Special rule for the target list_install_components
list_install_components:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Available install components are: \"Unspecified\""
.PHONY : list_install_components

# Special rule for the target list_install_components
list_install_components/fast: list_install_components

.PHONY : list_install_components/fast

# Special rule for the target install/local
install/local: preinstall
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/local/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local

# Special rule for the target install/local
install/local/fast: preinstall/fast
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Installing only the local directory..."
	/usr/local/bin/cmake -DCMAKE_INSTALL_LOCAL_ONLY=1 -P cmake_install.cmake
.PHONY : install/local/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/x1/Dokumenty/C++/zombie-shooter/CMakeFiles /home/x1/Dokumenty/C++/zombie-shooter/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/x1/Dokumenty/C++/zombie-shooter/CMakeFiles 0
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
# Target rules for targets named zombie-shooter

# Build rule for target.
zombie-shooter: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 zombie-shooter
.PHONY : zombie-shooter

# fast build rule for target.
zombie-shooter/fast:
	$(MAKE) -f CMakeFiles/zombie-shooter.dir/build.make CMakeFiles/zombie-shooter.dir/build
.PHONY : zombie-shooter/fast

GameObject.o: GameObject.cpp.o

.PHONY : GameObject.o

# target to build an object file
GameObject.cpp.o:
	$(MAKE) -f CMakeFiles/zombie-shooter.dir/build.make CMakeFiles/zombie-shooter.dir/GameObject.cpp.o
.PHONY : GameObject.cpp.o

GameObject.i: GameObject.cpp.i

.PHONY : GameObject.i

# target to preprocess a source file
GameObject.cpp.i:
	$(MAKE) -f CMakeFiles/zombie-shooter.dir/build.make CMakeFiles/zombie-shooter.dir/GameObject.cpp.i
.PHONY : GameObject.cpp.i

GameObject.s: GameObject.cpp.s

.PHONY : GameObject.s

# target to generate assembly for a file
GameObject.cpp.s:
	$(MAKE) -f CMakeFiles/zombie-shooter.dir/build.make CMakeFiles/zombie-shooter.dir/GameObject.cpp.s
.PHONY : GameObject.cpp.s

Player.o: Player.cpp.o

.PHONY : Player.o

# target to build an object file
Player.cpp.o:
	$(MAKE) -f CMakeFiles/zombie-shooter.dir/build.make CMakeFiles/zombie-shooter.dir/Player.cpp.o
.PHONY : Player.cpp.o

Player.i: Player.cpp.i

.PHONY : Player.i

# target to preprocess a source file
Player.cpp.i:
	$(MAKE) -f CMakeFiles/zombie-shooter.dir/build.make CMakeFiles/zombie-shooter.dir/Player.cpp.i
.PHONY : Player.cpp.i

Player.s: Player.cpp.s

.PHONY : Player.s

# target to generate assembly for a file
Player.cpp.s:
	$(MAKE) -f CMakeFiles/zombie-shooter.dir/build.make CMakeFiles/zombie-shooter.dir/Player.cpp.s
.PHONY : Player.cpp.s

Zombie.o: Zombie.cpp.o

.PHONY : Zombie.o

# target to build an object file
Zombie.cpp.o:
	$(MAKE) -f CMakeFiles/zombie-shooter.dir/build.make CMakeFiles/zombie-shooter.dir/Zombie.cpp.o
.PHONY : Zombie.cpp.o

Zombie.i: Zombie.cpp.i

.PHONY : Zombie.i

# target to preprocess a source file
Zombie.cpp.i:
	$(MAKE) -f CMakeFiles/zombie-shooter.dir/build.make CMakeFiles/zombie-shooter.dir/Zombie.cpp.i
.PHONY : Zombie.cpp.i

Zombie.s: Zombie.cpp.s

.PHONY : Zombie.s

# target to generate assembly for a file
Zombie.cpp.s:
	$(MAKE) -f CMakeFiles/zombie-shooter.dir/build.make CMakeFiles/zombie-shooter.dir/Zombie.cpp.s
.PHONY : Zombie.cpp.s

gs_main_menu.o: gs_main_menu.cpp.o

.PHONY : gs_main_menu.o

# target to build an object file
gs_main_menu.cpp.o:
	$(MAKE) -f CMakeFiles/zombie-shooter.dir/build.make CMakeFiles/zombie-shooter.dir/gs_main_menu.cpp.o
.PHONY : gs_main_menu.cpp.o

gs_main_menu.i: gs_main_menu.cpp.i

.PHONY : gs_main_menu.i

# target to preprocess a source file
gs_main_menu.cpp.i:
	$(MAKE) -f CMakeFiles/zombie-shooter.dir/build.make CMakeFiles/zombie-shooter.dir/gs_main_menu.cpp.i
.PHONY : gs_main_menu.cpp.i

gs_main_menu.s: gs_main_menu.cpp.s

.PHONY : gs_main_menu.s

# target to generate assembly for a file
gs_main_menu.cpp.s:
	$(MAKE) -f CMakeFiles/zombie-shooter.dir/build.make CMakeFiles/zombie-shooter.dir/gs_main_menu.cpp.s
.PHONY : gs_main_menu.cpp.s

gs_playing.o: gs_playing.cpp.o

.PHONY : gs_playing.o

# target to build an object file
gs_playing.cpp.o:
	$(MAKE) -f CMakeFiles/zombie-shooter.dir/build.make CMakeFiles/zombie-shooter.dir/gs_playing.cpp.o
.PHONY : gs_playing.cpp.o

gs_playing.i: gs_playing.cpp.i

.PHONY : gs_playing.i

# target to preprocess a source file
gs_playing.cpp.i:
	$(MAKE) -f CMakeFiles/zombie-shooter.dir/build.make CMakeFiles/zombie-shooter.dir/gs_playing.cpp.i
.PHONY : gs_playing.cpp.i

gs_playing.s: gs_playing.cpp.s

.PHONY : gs_playing.s

# target to generate assembly for a file
gs_playing.cpp.s:
	$(MAKE) -f CMakeFiles/zombie-shooter.dir/build.make CMakeFiles/zombie-shooter.dir/gs_playing.cpp.s
.PHONY : gs_playing.cpp.s

main.o: main.cpp.o

.PHONY : main.o

# target to build an object file
main.cpp.o:
	$(MAKE) -f CMakeFiles/zombie-shooter.dir/build.make CMakeFiles/zombie-shooter.dir/main.cpp.o
.PHONY : main.cpp.o

main.i: main.cpp.i

.PHONY : main.i

# target to preprocess a source file
main.cpp.i:
	$(MAKE) -f CMakeFiles/zombie-shooter.dir/build.make CMakeFiles/zombie-shooter.dir/main.cpp.i
.PHONY : main.cpp.i

main.s: main.cpp.s

.PHONY : main.s

# target to generate assembly for a file
main.cpp.s:
	$(MAKE) -f CMakeFiles/zombie-shooter.dir/build.make CMakeFiles/zombie-shooter.dir/main.cpp.s
.PHONY : main.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... install/strip"
	@echo "... edit_cache"
	@echo "... zombie-shooter"
	@echo "... install"
	@echo "... rebuild_cache"
	@echo "... list_install_components"
	@echo "... install/local"
	@echo "... GameObject.o"
	@echo "... GameObject.i"
	@echo "... GameObject.s"
	@echo "... Player.o"
	@echo "... Player.i"
	@echo "... Player.s"
	@echo "... Zombie.o"
	@echo "... Zombie.i"
	@echo "... Zombie.s"
	@echo "... gs_main_menu.o"
	@echo "... gs_main_menu.i"
	@echo "... gs_main_menu.s"
	@echo "... gs_playing.o"
	@echo "... gs_playing.i"
	@echo "... gs_playing.s"
	@echo "... main.o"
	@echo "... main.i"
	@echo "... main.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

