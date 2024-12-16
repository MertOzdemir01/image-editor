# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/ImagEditor_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/ImagEditor_autogen.dir/ParseCache.txt"
  "ImagEditor_autogen"
  )
endif()
