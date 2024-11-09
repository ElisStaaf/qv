cmake_minimum_required(VERSION 3.5)

set(PROJECT_NAME "vine")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "/usr/bin")
set(SOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/vine.c")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic -std=c99")

project(
   ${CMAKE_CURRENT_SOURCE_DIR} LANGUAGES "C"
   DESCRIPTION "Minimal CMake build for the \"Vine\" text editor."
)

if(MSVC)
   add_compile_options(/W3 /experimental:preprocessor)
else()
   add_compile_options(-Wall -Wextra -Wpedantic)
endif()

project(${PROJECT_NAME})

add_executable(vine ${SOURCE_FILES})
install(
    TARGETS "vine"
    DESTINATION "/usr/bin"
)
