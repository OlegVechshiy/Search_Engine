cmake_minimum_required(VERSION 3.5)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set(gtest_disable_pthreads on)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(SearchEngTests SearchEngTests.cpp ${MY_INCLUDE_DIR}/InvertedIndex.h ../src/InvertedIndex.cpp
    ${MY_INCLUDE_DIR}/SearchServer.h ../src/SearchServer.cpp)

target_link_libraries(SearchEngTests GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(SearchEngTests)
