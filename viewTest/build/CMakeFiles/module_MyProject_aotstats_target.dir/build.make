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
CMAKE_SOURCE_DIR = /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/build

# Utility rule file for module_MyProject_aotstats_target.

# Include any custom commands dependencies for this target.
include CMakeFiles/module_MyProject_aotstats_target.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/module_MyProject_aotstats_target.dir/progress.make

CMakeFiles/module_MyProject_aotstats_target: .rcc/qmlcache/module_MyProject.aotstats

.rcc/qmlcache/module_MyProject.aotstats: .rcc/qmlcache/MyProject_test/Main_qml.cpp.aotstats
.rcc/qmlcache/module_MyProject.aotstats: .rcc/qmlcache/module_MyProject.aotstatslist
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating .rcc/qmlcache/module_MyProject.aotstats"
	/home/errera/Qt/6.9.0/gcc_64/libexec/qmlaotstats aggregate /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/build/.rcc/qmlcache/module_MyProject.aotstatslist /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/build/.rcc/qmlcache/module_MyProject.aotstats

.rcc/qmlcache/MyProject_test/Main_qml.cpp: /home/errera/Qt/6.9.0/gcc_64/libexec/qmlcachegen
.rcc/qmlcache/MyProject_test/Main_qml.cpp: /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/test/Main.qml
.rcc/qmlcache/MyProject_test/Main_qml.cpp: .qt/rcc/qmake_MyApp.qrc
.rcc/qmlcache/MyProject_test/Main_qml.cpp: .qt/rcc/MyProject_raw_qml_0.qrc
.rcc/qmlcache/MyProject_test/Main_qml.cpp: MyApp/MyProject.qmltypes
.rcc/qmlcache/MyProject_test/Main_qml.cpp: MyApp/qmldir
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --blue --bold --progress-dir=/home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating .rcc/qmlcache/MyProject_test/Main_qml.cpp, .rcc/qmlcache/MyProject_test/Main_qml.cpp.aotstats"
	/usr/bin/cmake -E make_directory /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/build/.rcc/qmlcache/MyProject_test
	/home/errera/Qt/6.9.0/gcc_64/libexec/qmlcachegen --bare --resource-path /MyApp/test/Main.qml -I /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/build -I /home/errera/Qt/6.9.0/gcc_64/qml -i /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/build/MyApp/qmldir --resource /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/build/.qt/rcc/qmake_MyApp.qrc --resource /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/build/.qt/rcc/MyProject_raw_qml_0.qrc --dump-aot-stats "--module-id=MyApp(MyProject)" -o /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/build/.rcc/qmlcache/MyProject_test/Main_qml.cpp /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/test/Main.qml

.rcc/qmlcache/MyProject_test/Main_qml.cpp.aotstats: .rcc/qmlcache/MyProject_test/Main_qml.cpp
	@$(CMAKE_COMMAND) -E touch_nocreate .rcc/qmlcache/MyProject_test/Main_qml.cpp.aotstats

module_MyProject_aotstats_target: .rcc/qmlcache/MyProject_test/Main_qml.cpp
module_MyProject_aotstats_target: .rcc/qmlcache/MyProject_test/Main_qml.cpp.aotstats
module_MyProject_aotstats_target: .rcc/qmlcache/module_MyProject.aotstats
module_MyProject_aotstats_target: CMakeFiles/module_MyProject_aotstats_target
module_MyProject_aotstats_target: CMakeFiles/module_MyProject_aotstats_target.dir/build.make
.PHONY : module_MyProject_aotstats_target

# Rule to build all files generated by this target.
CMakeFiles/module_MyProject_aotstats_target.dir/build: module_MyProject_aotstats_target
.PHONY : CMakeFiles/module_MyProject_aotstats_target.dir/build

CMakeFiles/module_MyProject_aotstats_target.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/module_MyProject_aotstats_target.dir/cmake_clean.cmake
.PHONY : CMakeFiles/module_MyProject_aotstats_target.dir/clean

CMakeFiles/module_MyProject_aotstats_target.dir/depend:
	cd /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/build /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/build /home/errera/utfpr/sistemasOperacionais/dashboard-os/viewTest/build/CMakeFiles/module_MyProject_aotstats_target.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : CMakeFiles/module_MyProject_aotstats_target.dir/depend

