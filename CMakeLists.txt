cmake_minimum_required(VERSION 3.23)

project(
  koterm
  VERSION 0.1.0
  LANGUAGES CXX)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/bootstrap.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/create_example.cmake)

# -----------------------------------------------------------------------------
# Set options
option(ENABLE_KOTERM_TESTING "Build and run tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(KOTERM_DEBUG "Enable debug build" ON)
option(KOTERM_DEBUG_DOM "Show elements bounding boxes" OFF)
option(DISABLE_EXCEPTIONS "Disable c++ exceptions" OFF)

set(LIBRARY_NAME "${PROJECT_NAME}")
set(GEN_FILES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/gen")
set(TEMPLATE_FILES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates")

# -----------------------------------------------------------------------------
# Target

add_library(${LIBRARY_NAME} STATIC)

target_include_directories(
  ${LIBRARY_NAME} PUBLIC $<INSTALL_INTERFACE:include>
                         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>)

target_compile_features(${LIBRARY_NAME} PRIVATE cxx_std_20)
target_compile_definitions(
  ${LIBRARY_NAME}
  PRIVATE $<$<BOOL:${KOTERM_DEBUG}>:KOTERM_DEBUG>
          $<$<BOOL:${KOTERM_DEBUG_DOM}>:KOTERM_DEBUG_DOM>
          $<$<BOOL:${DISABLE_EXCEPTIONS}>:KOTERM_NO_EXCEPTIONS>)

target_compile_options(${LIBRARY_NAME}
                       PRIVATE $<$<BOOL:${DISABLE_EXCEPTIONS}>:-fexceptions>)

set_target_properties(${LIBRARY_NAME} PROPERTIES CXX_EXTENSIONS OFF)

# -----------------------------------------------------------------------------
# Config
set(CONFIG_VERSION "${PROJECT_VERSION}")

configure_file("${TEMPLATE_FILES_DIR}/config.h" "${GEN_FILES_DIR}/config.h"
               @ONLY)

target_include_directories(${LIBRARY_NAME} PUBLIC ${GEN_FILES_DIR})

add_subdirectory("${PROJECT_SOURCE_DIR}/src")

if(ENABLE_KOTERM_TESTING MATCHES ON)
    add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES MATCHES ON)
    add_subdirectory(examples)
endif()
