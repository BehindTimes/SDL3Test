cmake_minimum_required(VERSION 3.10) # Specifies the minimum required CMake version

project(U3Lairware CXX) # Defines the project name and language (C++ in this case)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL3Test")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(Ogg REQUIRED)
find_package(Sndfile REQUIRED)
find_package(OpenAL REQUIRED)
find_package(PkgConfig REQUIRED)

pkg_check_modules(Vorbis REQUIRED IMPORTED_TARGET Vorbis)
pkg_check_modules(Vorbisenc REQUIRED IMPORTED_TARGET Vorbisenc)
pkg_check_modules(Vorbisfile REQUIRED IMPORTED_TARGET Vorbisfile)

add_executable(U3Lairware)

target_sources(U3Lairware PRIVATE
	${SOURCE_DIR}/Main.cpp
	${SOURCE_DIR}/MiscCommands.cpp
	${SOURCE_DIR}/U3Button.cpp
	${SOURCE_DIR}/U3Dialog.cpp
	${SOURCE_DIR}/U3Misc.cpp
	${SOURCE_DIR}/U3Resources.cpp
	${SOURCE_DIR}/U3ScrollArea.cpp
	${SOURCE_DIR}/U3Utilities.cpp
	${SOURCE_DIR}/UltimaAutocombat.cpp
	${SOURCE_DIR}/UltimaDungeon.cpp
	${SOURCE_DIR}/UltimaGraphics.cpp
	${SOURCE_DIR}/UltimaIncludes.cpp
	${SOURCE_DIR}/UltimaMacIF.cpp
	${SOURCE_DIR}/UltimaNewMap.cpp
	${SOURCE_DIR}/UltimaSound.cpp
	${SOURCE_DIR}/UltimaSpellCombat.cpp
)

target_link_libraries(U3Lairware PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image
					LibXml2::LibXml2 Ogg::ogg OpenAL::OpenAL SndFile::sndfile
					PkgConfig::Vorbis PkgConfig::Vorbisenc PkgConfig::Vorbisfile
)

target_include_directories(U3Lairware PRIVATE ${SDL3_INCLUDE_DIRS})

add_compile_definitions(HAVE_OPEN_AL=1)
add_compile_definitions(NDEBUG)