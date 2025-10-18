# Memory Card Flip Game - CMake Configuration
# MSTC DA-IICT Hacktoberfest 2025 Project

cmake_minimum_required(VERSION 3.16)

# Project Information
project(memory_card_game
    VERSION 1.0.0
    DESCRIPTION "A classic memory card flip game built with C++ and raylib"
    LANGUAGES CXX
)

# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Compiler-specific options
if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /DDEBUG")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -DDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()

# Build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Option to enable/disable tests (default OFF if tests directory is absent)
option(BUILD_TESTS "Build unit tests" OFF)

# Dependencies
include(FetchContent)

# Fetch raylib
message(STATUS "Fetching raylib...")
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.0.tar.gz
    URL_HASH SHA256=98f049b9ea2a9c40a14e4e543eeea1a7ec3090ebdcd329c4ca2cf98bc9793482
)

# Configure raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_MakeAvailable(raylib)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Source files
set(SOURCES
    src/main.cpp
    src/Game.cpp
    src/Card.cpp
    src/GameBoard.cpp
    src/Utils.cpp
    src/AudioManager.cpp
    src/ScoreManager.cpp
)

# Header files
set(HEADERS
    include/Game.h
    include/Card.h
    include/GameBoard.h
    include/Utils.h
    include/AudioManager.h
    include/ScoreManager.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Platform-specific settings
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE winmm)
elseif(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework CoreVideo" "-framework IOKit" "-framework Cocoa" "-framework GLUT" "-framework OpenGL")
elseif(UNIX)
    target_link_libraries(${PROJECT_NAME} PRIVATE GL m pthread dl rt X11)
endif()

# Set properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    OUTPUT_NAME "memory_game"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Copy assets to build directory (only if assets exist)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/assets)
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/assets DESTINATION ${CMAKE_BINARY_DIR})
else()
    message(STATUS "No assets directory found. Skipping asset copy.")
endif()

# Install target
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(DIRECTORY assets DESTINATION .)

# Testing
if(BUILD_TESTS AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    enable_testing()

    # Collect test files if they exist
    file(GLOB TEST_SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/tests/*.cpp"
    )

    if(TEST_SOURCES)
        # Create a source list without main.cpp for tests
        set(SOURCES_NO_MAIN ${SOURCES})
        list(REMOVE_ITEM SOURCES_NO_MAIN src/main.cpp)

        add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES} ${SOURCES_NO_MAIN})
        target_link_libraries(${PROJECT_NAME}_tests PRIVATE raylib)

        # Platform-specific test linking
        if(WIN32)
            target_link_libraries(${PROJECT_NAME}_tests PRIVATE winmm)
        elseif(APPLE)
            target_link_libraries(${PROJECT_NAME}_tests PRIVATE "-framework CoreVideo" "-framework IOKit" "-framework Cocoa" "-framework GLUT" "-framework OpenGL")
        elseif(UNIX)
            target_link_libraries(${PROJECT_NAME}_tests PRIVATE GL m pthread dl rt X11)
        endif()

        # Add test
        add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)

        set_target_properties(${PROJECT_NAME}_tests PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/tests
        )
    else()
        message(WARNING "BUILD_TESTS is ON but no test sources were found in the tests directory.")
    endif()
elseif(BUILD_TESTS)
    message(WARNING "BUILD_TESTS is ON but tests directory not found. Skipping test target generation.")
endif()

# Package configuration
set(CPACK_PACKAGE_NAME "MemoryCardGame")
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY ${PROJECT_DESCRIPTION})
set(CPACK_PACKAGE_VENDOR "MSTC DA-IICT")
set(CPACK_PACKAGE_CONTACT "microsoftclub@daiict.ac.in")

if(WIN32)
    set(CPACK_GENERATOR "ZIP;NSIS")
elseif(APPLE)
    set(CPACK_GENERATOR "ZIP;DragNDrop")
else()
    set(CPACK_GENERATOR "ZIP;TGZ;DEB")
endif()

include(CPack)

# Print configuration
message(STATUS "")
message(STATUS "========== Build Configuration ==========")
message(STATUS "Project: ${PROJECT_NAME} v${PROJECT_VERSION}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Compiler: ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build tests: ${BUILD_TESTS}")
message(STATUS "Install prefix: ${CMAKE_INSTALL_PREFIX}")
message(STATUS "==========================================")
message(STATUS "")