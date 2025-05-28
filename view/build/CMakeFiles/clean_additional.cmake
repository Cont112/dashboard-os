# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/dashboard_os_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/dashboard_os_autogen.dir/ParseCache.txt"
  "dashboard_os_autogen"
  )
endif()
