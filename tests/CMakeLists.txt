enable_testing()

include(${PROJECT_SOURCE_DIR}/cmake/create_test.cmake)

create_test(
  "tiles"
  FILES
  koterm/terminal/tiles.cpp
  LIBS
  ${LIBRARY_NAME}
  NO_EXCEPTIONS
  WITH_MAIN)

create_test(
  "parser"
  FILES
  koterm/terminal/parser.cpp
  LIBS
  ${LIBRARY_NAME}
  NO_EXCEPTIONS
  WITH_MAIN)

create_test(
  "events"
  FILES
  koterm/event/events.cpp
  LIBS
  ${LIBRARY_NAME}
  NO_EXCEPTIONS
  WITH_MAIN)
