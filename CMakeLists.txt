cmake_minimum_required(VERSION 3.10) # Specifies the minimum required CMake version

project(U3Lairware CXX) # Defines the project name and language (C++ in this case)

set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL3Test")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS FALSE)

find_package(SDL3 REQUIRED)
find_package(SDL3_ttf CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)
find_package(LibXml2 REQUIRED)
find_package(Ogg REQUIRED)
find_package(Sndfile REQUIRED)
find_package(OpenAL REQUIRED)
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

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} /opt/homebrew/lib)

target_link_libraries(U3Lairware PRIVATE SDL3::SDL3 SDL3_ttf::SDL3_ttf SDL3_image::SDL3_image
					LibXml2::LibXml2 Ogg::ogg OpenAL::OpenAL SndFile::sndfile
					/opt/homebrew/lib/libvorbis.a
					/opt/homebrew/lib/libvorbisfile.a
					/opt/homebrew/lib/libvorbisenc.a
)

target_include_directories(U3Lairware PRIVATE ${SDL3_INCLUDE_DIRS})

add_compile_definitions(HAVE_OPEN_AL=1)
add_compile_definitions(NDEBUG)