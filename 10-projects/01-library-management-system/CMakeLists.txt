cmake_minimum_required(VERSION 3.16)

# Project configuration
project(LibraryManagementSystem 
    VERSION 1.0.0
    DESCRIPTION "A comprehensive C++ library management system"
    LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-specific options
if(MSVC)
    # MSVC specific options
    add_compile_options(/W4 /WX)  # High warning level, treat warnings as errors
    add_compile_options(/permissive-)  # Disable permissive mode
else()
    # GCC/Clang options
    add_compile_options(-Wall -Wextra -Werror -pedantic)  # Enable all warnings
    add_compile_options(-Wshadow -Wnon-virtual-dtor -Wold-style-cast)
    add_compile_options(-Wcast-align -Wunused -Woverloaded-virtual)
    add_compile_options(-Wpedantic -Wconversion -Wsign-conversion)
    add_compile_options(-Wmisleading-indentation -Wduplicated-cond)
    add_compile_options(-Wduplicated-branches -Wlogical-op)
    add_compile_options(-Wnull-dereference -Wuseless-cast)
    add_compile_options(-Wdouble-promotion -Wformat=2)
endif()

# Debug configuration
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
    if(NOT MSVC)
        add_compile_options(-g -O0)  # Debug symbols, no optimization
        add_compile_options(-fsanitize=address -fsanitize=undefined)
        add_link_options(-fsanitize=address -fsanitize=undefined)
    endif()
endif()

# Release configuration
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(NDEBUG)
    if(NOT MSVC)
        add_compile_options(-O3)  # Maximum optimization
    endif()
endif()

# Include directories
include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/src/core
    ${PROJECT_SOURCE_DIR}/src/utils
    ${PROJECT_SOURCE_DIR}/src/interfaces
)

# Source files
set(CORE_SOURCES
    src/core/book.cpp
    src/core/member.cpp
    src/core/library.cpp
    src/core/transaction.cpp
)

set(UTILS_SOURCES
    src/utils/date_time.cpp
    src/utils/file_manager.cpp
    src/utils/validator.cpp
)

set(INTERFACE_SOURCES
    src/interfaces/searchable.cpp
    src/interfaces/persistable.cpp
)

# Create library for core functionality
add_library(LibraryCore STATIC
    ${CORE_SOURCES}
    ${UTILS_SOURCES}
    ${INTERFACE_SOURCES}
)

# Main executable
add_executable(LibrarySystem
    src/main.cpp
)

# Link libraries
target_link_libraries(LibrarySystem LibraryCore)

# Optional: Find and link threading library
find_package(Threads REQUIRED)
target_link_libraries(LibraryCore Threads::Threads)

# Optional: Enable testing
option(BUILD_TESTS "Build unit tests" ON)

if(BUILD_TESTS)
    enable_testing()
    
    # Find Google Test
    find_package(GTest QUIET)
    
    if(GTest_FOUND)
        message(STATUS "Found Google Test")
        
        # Test sources
        set(TEST_SOURCES
            tests/test_book.cpp
            tests/test_member.cpp
            tests/test_library.cpp
            tests/test_utils.cpp
        )
        
        # Create test executable
        add_executable(LibraryTests
            ${TEST_SOURCES}
        )
        
        # Link test libraries
        target_link_libraries(LibraryTests 
            LibraryCore
            GTest::GTest 
            GTest::Main
        )
        
        # Add tests to CTest
        include(GoogleTest)
        gtest_discover_tests(LibraryTests)
        
    else()
        message(WARNING "Google Test not found. Tests will not be built.")
        message(STATUS "To install Google Test:")
        message(STATUS "  Ubuntu/Debian: sudo apt-get install libgtest-dev")
        message(STATUS "  macOS: brew install googletest")
        message(STATUS "  Or build from source: https://github.com/google/googletest")
    endif()
endif()

# Install configuration
install(TARGETS LibrarySystem
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY src/
    DESTINATION include/library
    FILES_MATCHING PATTERN "*.h"
)

# Package configuration
set(CPACK_PACKAGE_NAME "LibraryManagementSystem")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "A C++ Library Management System")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")

include(CPack)

# Custom targets for development

# Format code using clang-format
find_program(CLANG_FORMAT "clang-format")
if(CLANG_FORMAT)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i -style=file
        ${CORE_SOURCES} ${UTILS_SOURCES} ${INTERFACE_SOURCES}
        src/main.cpp ${TEST_SOURCES}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Formatting code with clang-format"
    )
endif()

# Static analysis using clang-tidy
find_program(CLANG_TIDY "clang-tidy")
if(CLANG_TIDY)
    add_custom_target(tidy
        COMMAND ${CLANG_TIDY} 
        ${CORE_SOURCES} ${UTILS_SOURCES} ${INTERFACE_SOURCES} src/main.cpp
        -- -I${PROJECT_SOURCE_DIR}/src
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running static analysis with clang-tidy"
    )
endif()

# Memory check using valgrind (Linux only)
if(UNIX AND NOT APPLE)
    find_program(VALGRIND "valgrind")
    if(VALGRIND)
        add_custom_target(memcheck
            COMMAND ${VALGRIND} --tool=memcheck --leak-check=full 
            --show-leak-kinds=all --track-origins=yes --verbose
            $<TARGET_FILE:LibrarySystem>
            DEPENDS LibrarySystem
            COMMENT "Running memory check with valgrind"
        )
    endif()
endif()

# Print build information
message(STATUS "")
message(STATUS "Library Management System Configuration:")
message(STATUS "  Version: ${PROJECT_VERSION}")
message(STATUS "  Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "  C++ standard: C++${CMAKE_CXX_STANDARD}")
message(STATUS "  Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "  Build tests: ${BUILD_TESTS}")
message(STATUS "")

# Development instructions
message(STATUS "Build instructions:")
message(STATUS "  mkdir build && cd build")
message(STATUS "  cmake .. -DCMAKE_BUILD_TYPE=Debug")
message(STATUS "  make -j$(nproc)")
message(STATUS "")
message(STATUS "Run instructions:")
message(STATUS "  ./LibrarySystem")
message(STATUS "")
if(BUILD_TESTS AND GTest_FOUND)
    message(STATUS "Test instructions:")
    message(STATUS "  make test")
    message(STATUS "  or: ./LibraryTests")
    message(STATUS "")
endif()
message(STATUS "Development tools:")
if(CLANG_FORMAT)
    message(STATUS "  make format    - Format code")
endif()
if(CLANG_TIDY)
    message(STATUS "  make tidy      - Static analysis")
endif()
if(VALGRIND)
    message(STATUS "  make memcheck  - Memory leak check")
endif()
message(STATUS "")