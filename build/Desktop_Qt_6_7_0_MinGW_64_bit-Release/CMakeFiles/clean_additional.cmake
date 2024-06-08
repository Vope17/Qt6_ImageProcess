# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\new_imgproc_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\new_imgproc_autogen.dir\\ParseCache.txt"
  "new_imgproc_autogen"
  )
endif()
