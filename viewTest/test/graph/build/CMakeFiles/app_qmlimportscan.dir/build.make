# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.28

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
CMAKE_SOURCE_DIR = /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/test/graph

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/test/graph/build

# Utility rule file for app_qmlimportscan.

# Include any custom commands dependencies for this target.
include CMakeFiles/app_qmlimportscan.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/app_qmlimportscan.dir/progress.make

CMakeFiles/app_qmlimportscan: .qt/qml_imports/app_build.cmake

.qt/qml_imports/app_build.cmake: /home/errera/Qt/6.9.0/gcc_64/libexec/qmlimportscanner
.qt/qml_imports/app_build.cmake: .qt/rcc/qmake_MyApp.qrc
.qt/qml_imports/app_build.cmake: .qt/rcc/app_raw_qml_0.qrc
.qt/qml_imports/app_build.cmake: /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/test/graph/main.qml
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/test/graph/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Running qmlimportscanner for app"
	cd /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/test/graph && /home/errera/Qt/6.9.0/gcc_64/libexec/qmlimportscanner @/home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/test/graph/build/.qt/qml_imports/app_build.rsp

app_qmlimportscan: .qt/qml_imports/app_build.cmake
app_qmlimportscan: CMakeFiles/app_qmlimportscan
app_qmlimportscan: CMakeFiles/app_qmlimportscan.dir/build.make
.PHONY : app_qmlimportscan

# Rule to build all files generated by this target.
CMakeFiles/app_qmlimportscan.dir/build: app_qmlimportscan
.PHONY : CMakeFiles/app_qmlimportscan.dir/build

CMakeFiles/app_qmlimportscan.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/app_qmlimportscan.dir/cmake_clean.cmake
.PHONY : CMakeFiles/app_qmlimportscan.dir/clean

CMakeFiles/app_qmlimportscan.dir/depend:
	cd /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/test/graph/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/test/graph /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/test/graph /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/test/graph/build /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/test/graph/build /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/test/graph/build/CMakeFiles/app_qmlimportscan.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/app_qmlimportscan.dir/depend

