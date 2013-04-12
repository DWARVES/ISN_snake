
# On s'assure d'avoir la bonne version
cmake_minimum_required(VERSION 2.6)

# Déclaration du projet
project(snakekans)
set(EXECUTABLE_OUTPUT_PATH bin/${CMAKE_BUILD_TYPE})
set(executable snakekans)

# On trouve les fichiers sources
file(
	GLOB_RECURSE
	source_files
	src/*.cpp
	src/*.hpp
	)

# Inclusion de la SDL
include(FindSDL)
find_package(SDL REQUIRED)
link_directories(${SDL_LIBRARY_DIR})
include_directories(${SDL_INCLUDE_DIR})

include(FindSDL_image)
find_package(SDL_image REQUIRED)
link_directories(${SDL_IMAGE_LIBRARY_DIR})
include_directories(${SDL_IMAGE_INCLUDE_DIR})

# L'exécutable et les sources
add_executable(
	${executable}
	${source_files}
	)

# Le linkage
target_link_libraries(
	${executable}
	${SDL_LIBRARY}
	${SDL_IMAGE_LIBRARY}
	)

