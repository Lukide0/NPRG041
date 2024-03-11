include_guard(GLOBAL)

include(FetchContent)
find_package(Git REQUIRED)

FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest.git
    GIT_TAG "v2.4.11"
    GIT_SHALLOW TRUE
    GIT_PROGRESS ON
)
FetchContent_MakeAvailable(doctest)

FetchContent_GetProperties(doctest)
if (NOT doctest_POPULATED)
    FetchContent_Populate(doctest)
endif()

set(DOCTEST_INCLUDE_DIR
    ${doctest_SOURCE_DIR}/doctest
    CACHE INTERNAL "Path to include folder for doctest")
