enable_testing()

include(${PROJECT_SOURCE_DIR}/cmake/create_test.cmake)

create_test(
  "tiles"
  FILES
  koterm/screen/tiles.cpp
  LIBS
  ${LIBRARY_NAME}
  NO_EXCEPTIONS
  WITH_MAIN)
