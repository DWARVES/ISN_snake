
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
	src/gui/*.cpp
	src/gui/*.hpp
	src/gameplay/*.cpp
	src/gameplay/*.hpp
	src/ctrls/*.cpp
	src/ctrls/*.hpp
	src/misc/*.cpp
	src/misc/*.hpp
	)

# On indique les dossiers d'inclusion
include_directories(src/)
include_directories(src/gui/)
include_directories(src/gameplay/)
include_directories(src/ctrls/)
include_directories(src/misc/)

# Inclusion de la SDL
include(FindSDL)
find_package(SDL REQUIRED)
link_directories(${SDL_LIBRARY_DIR})
include_directories(${SDL_INCLUDE_DIR})

# Inclusion de SDL_image
include(FindSDL_image)
find_package(SDL_image REQUIRED)
link_directories(${SDL_IMAGE_LIBRARY_DIR})
include_directories(${SDL_IMAGE_INCLUDE_DIR})

# Inclusion de SDL_ttf
include(FindSDL_ttf)
find_package(SDL_ttf REQUIRED)
link_directories(${SDL_TTF_LIBRARY_DIR})
include_directories(${SDL_TTF_INCLUDE_DIR})

# Inclusion de SDL_mixer
include(FindSDL_mixer)
find_package(SDL_mixer REQUIRED)
link_directories(${SDL_MIXER_LIBRARY_DIR})
include_directories(${SDL_MIXER_INCLUDE_DIR})

# Inclusion de Boost
include(FindBoost)
find_package(Boost COMPONENTS system filesystem regex REQUIRED)
link_directories(${Boost_SYSTEM_LIBRARY_DIR})
include_directories(${Boost_SYSTEM_INCLUDE_DIR})
link_directories(${Boost_FILESYSTEM_LIBRARY_DIR})
include_directories(${Boost_FILESYSTEM_INCLUDE_DIR})
link_directories(${Boost_REGEX_LIBRARY_DIR})
include_directories(${Boost_REGEX_INCLUDE_DIR})

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
	${SDL_TTF_LIBRARY}
	${SDL_MIXER_LIBRARY}
	${Boost_FILESYSTEM_LIBRARY}
	${Boost_SYSTEM_LIBRARY}
	${Boost_REGEX_LIBRARY}
	)

