if(NOT TARGET mio_tflite)
  return()
endif(NOT TARGET mio_tflite)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(tfl-verify ${SOURCES})
target_include_directories(tfl-verify PRIVATE src)
target_link_libraries(tfl-verify mio_tflite)
target_link_libraries(tfl-verify safemain)
target_link_libraries(tfl-verify cwrap)
target_link_libraries(tfl-verify stdex)
