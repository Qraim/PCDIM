# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/PCDIM_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/PCDIM_autogen.dir/ParseCache.txt"
  "PCDIM_autogen"
  )
endif()
