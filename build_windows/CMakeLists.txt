cmake_minimum_required(VERSION 3.28.3)

project(DtekProject)

add_library(SDL3 STATIC IMPORTED)
set_target_properties(SDL3 PROPERTIES
IMPORTED_LOCATION ${CMAKE_SOURCE_DIR}/../Libraries/SDL3-3.2.26/x86_64-w64-mingw32/lib/libSDL3.dll.a
INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_SOURCE_DIR}/../Libraries/SDL3-3.2.26/x86_64-w64-mingw32/include
)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS 
"${CMAKE_SOURCE_DIR}/../src/*.c"
"${CMAKE_SOURCE_DIR}/../src/*.h"
"${CMAKE_SOURCE_DIR}/../src/**/*.c"
"${CMAKE_SOURCE_DIR}/../src/**/*.h"
)

add_executable(out ${SRC})
target_include_directories(out PRIVATE
    ${CMAKE_SOURCE_DIR}/../src
    ${CMAKE_SOURCE_DIR}/../Libraries/SDL3-3.2.26/x86_64-w64-mingw32/include
)

target_compile_definitions(out PRIVATE __WINDOWS__=1)
add_compile_options(out PRIVATE -g)

target_link_libraries(out PRIVATE SDL3)

# Run after 'out' is built
add_custom_target(copy_SDL ALL
    COMMAND cp ${CMAKE_SOURCE_DIR}/../Libraries/SDL3-3.2.26/x86_64-w64-mingw32/bin/SDL3.dll SDL3.dll
    COMMAND echo "copied ${CMAKE_SOURCE_DIR}/../Libraries/SDL3-3.2.26/x86_64-w64-mingw32/bin/SDL3.dll"
    DEPENDS out
)
