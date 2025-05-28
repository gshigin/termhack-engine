find_package(GTest CONFIG QUIET)

if (NOT GTest_FOUND)
    message(STATUS "GTest not found: Fetching GTest")

    include(FetchContent)
    FetchContent_Declare(
        GTest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
    )
    FetchContent_MakeAvailable(GTest)
else()
    message(STATUS "GTest found")
endif()

file(GLOB_RECURSE TEST_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests PRIVATE GTest::gtest_main termhack-static)

target_include_directories(tests PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)

include(CTest)
enable_testing()

add_test(NAME RunTests COMMAND tests)