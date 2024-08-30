# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/Qonnect_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/Qonnect_autogen.dir/ParseCache.txt"
  "Qonnect_autogen"
  )
endif()
