cmake_minimum_required(VERSION 3.5)

configure_file(${MY_INCLUDE_DIR}/config.h in ${MY_INCLUDE_DIR}/config.h @ONLY)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(Search_Engine main.cpp ConverterJs.cpp ${MY_INCLUDE_DIR}/ConverterJs.h
    InvertedIndex.cpp ${MY_INCLUDE_DIR}/InvertedIndex.h SearchServer.cpp ${MY_INCLUDE_DIR}/SearchServer.h)

target_link_libraries(Search_Engine PRIVATE nlohmann_json::nlohmann_json)

target_include_directories(Search_Engine PUBLIC ${MY_INCLUDE_DIR})
