cmake_minimum_required(VERSION 3.28.3)

project(DtekProject)

add_library(SDL3 STATIC IMPORTED)
set_target_properties(SDL3 PROPERTIES
IMPORTED_LOCATION ../Libraries/SDL3-3.2.26/x86_64-w64-mingw32/lib/libSDL3.dll.a
INTERFACE_INCLUDE_DIRECTORIES ../Libraries/SDL3-3.2.26/x86_64-w64-mingw32/include
)

file(GLOB SRC 
"../src/*.c"
"../src/*.h"
)

add_executable(out ${SRC})
target_include_directories(out PRIVATE
    ${CMAKE_SOURCE_DIR}/../src
)

target_compile_definitions(out PRIVATE __WINDOWS__=1)
add_compile_options(out PRIVATE -g)
