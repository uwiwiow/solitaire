cmake_minimum_required(VERSION 4.1)
project(solitaire)

set(CMAKE_C_STANDARD 23)

file(GLOB solitaire_SRC CONFIGURE_DEPENDS src/*.h src/*.c)

add_executable(solitaire
	${solitaire_SRC})

find_package(raylib REQUIRED)

target_link_libraries(solitaire raylib m)

file(COPY res DESTINATION .)
