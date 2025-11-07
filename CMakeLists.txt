cmake_minimum_required(VERSION 3.10)
project(m3u8 C CXX)

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

option(COMPILE_TESTS "Compile test executables" OFF)
option(M3U8_ENABLE_PREEMPTIVE_LOGGER "Enable preemptive logger" OFF)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(PROJECT_VERSION "1.0.0")

include(GNUInstallDirs)

file(GLOB SOURCE "${CMAKE_SOURCE_DIR}/src/*.c")
file(GLOB HEADER "${CMAKE_SOURCE_DIR}/src/*.h")

add_library(m3u8 SHARED ${SOURCE})

# compile definitions
if (M3U8_ENABLE_PREEMPTIVE_LOGGER)
  target_compile_definitions(m3u8 PRIVATE M3U8_PREEMPTIVE_LOGGER)
endif()

target_compile_options(m3u8 PRIVATE -Wall -g -pthread)
target_link_libraries(m3u8 PRIVATE pthread)

configure_file(${CMAKE_SOURCE_DIR}/m3u8.pc.in ${CMAKE_BINARY_DIR}/m3u8.pc @ONLY)

message(${CMAKE_INSTALL_LIBDIR}/pkgconfig)

install(FILES ${CMAKE_BINARY_DIR}/m3u8.pc DESTINATION ${CMAKE_INSTALL_LIBDIR}/pkgconfig)
install(TARGETS m3u8 LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR} ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})
install(FILES ${HEADER} DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/m3u8)

if(COMPILE_TESTS)
    enable_testing()
    find_package(GTest REQUIRED)

    file(GLOB TEST_SOURCES "${CMAKE_SOURCE_DIR}/tests/*.cc")

    foreach(test_file ${TEST_SOURCES})
        get_filename_component(test_name ${test_file} NAME_WE)
        add_executable(${test_name} ${test_file} ${MOCK_SOURCES})
        target_compile_options(${test_name} PRIVATE -fsanitize=address -g)
        target_link_libraries(${test_name} PRIVATE -fsanitize=address)
        target_include_directories(${test_name} PRIVATE ${CMAKE_SOURCE_DIR}/m3u8 ${CMAKE_SOURCE_DIR}/tests/mocks)
        target_link_libraries(${test_name} PRIVATE m3u8 GTest::gmock GTest::gtest GTest::gtest_main pthread)

        include(GoogleTest)
        gtest_discover_tests(${test_name})
    endforeach()
endif()

