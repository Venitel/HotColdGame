cmake_minimum_required(VERSION 3.21)
project(HotColdGame VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#Lib
add_library(HCG
    src/ConfigLoader.cpp 
    src/Game.cpp 
    src/Section.cpp
    src/Guess.cpp)

target_include_directories(HCG PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

#Exec
add_executable(HotColdGame main.cpp)
target_link_libraries(HotColdGame PRIVATE HCG)

#Tests
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/release-1.12.1.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

#Text Exec
add_executable(HCGTests
    tests/testGuess.cpp
    tests/testParse.cpp
)

target_link_libraries(HCGTests
    PRIVATE
    HCG
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(HCGTests)

add_custom_target(testAll
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    DEPENDS HotColdGame HCGTests
)

# Copy sections.cfg to the build folder
configure_file(sections.cfg sections.cfg COPYONLY)