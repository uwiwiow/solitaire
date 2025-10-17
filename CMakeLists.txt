cmake_minimum_required(VERSION 4.0.2)
project(solitaire VERSION 1.0)

set(CMAKE_C_STANDARD 23)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_C_FLAGS_DEBUG "-g -fsanitize=address")
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
		add_compile_options(-fsanitize=address)
		add_link_options(-fsanitize=address)
	endif()
endif()

file(GLOB solitaire_SRC CONFIGURE_DEPENDS src/*.h src/*.c)
add_executable(solitaire ${solitaire_SRC})

set(BUILD_SHARED_LIBS OFF)

find_library(RAYLIB_STATIC NAMES raylib PATHS /usr/local/lib NO_DEFAULT_PATH)
if(RAYLIB_STATIC)
	message(STATUS "Using static Raylib: ${RAYLIB_STATIC}")
	set(RAYLIB ${RAYLIB_STATIC})
else()
	message(WARNING "Static libraylib.a not found, falling back to shared version")
	find_package(raylib REQUIRED)
	set(RAYLIB raylib)
endif()

target_link_libraries(solitaire ${RAYLIB} m)

file(COPY res DESTINATION .)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
	set(OUTPUT_DIR ${CMAKE_BINARY_DIR}/Solitaire)
	file(MAKE_DIRECTORY ${OUTPUT_DIR})

	add_custom_command(TARGET solitaire POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:solitaire> ${OUTPUT_DIR}/
	)
	add_custom_command(TARGET solitaire POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy_directory
			${CMAKE_SOURCE_DIR}/res ${OUTPUT_DIR}/res
	)

	string(TOLOWER ${CMAKE_SYSTEM_PROCESSOR} ARCH)

	add_custom_target(package_release ALL
			COMMAND ${CMAKE_COMMAND} -E tar "cfv"
			${CMAKE_BINARY_DIR}/solitaire-${PROJECT_VERSION}-${ARCH}.zip
			--format=zip Solitaire
			WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
			DEPENDS solitaire
	)
endif()
