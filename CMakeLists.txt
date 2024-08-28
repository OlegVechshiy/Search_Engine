cmake_minimum_required(VERSION 3.5)

project(Search_Engine LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(JSON_BuildTests OFF CACHE INTERNAL "")

set(VERSION_APP ${search_engine_VERSION_MAJOR}.${search_engine_VERSION_MINOR})

set(MY_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(nlohmann_json)
add_subdirectory(src)
add_subdirectory(tests)
