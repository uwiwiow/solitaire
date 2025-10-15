cmake_minimum_required(VERSION 3.30.5)
project(solitaire)

set(CMAKE_C_STANDARD 23)

set(CMAKE_BUILD_TYPE Debug)

if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
	add_compile_options(-fsanitize=address -g)
	add_link_options(-fsanitize=address)
endif()

file(GLOB solitaire_SRC CONFIGURE_DEPENDS src/*.h src/*.c)

add_executable(solitaire
	${solitaire_SRC})

find_package(raylib REQUIRED)

target_link_libraries(solitaire raylib m)

file(COPY res DESTINATION .)
