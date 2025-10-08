Requirements:

C++20

[SDL3](https://github.com/libsdl-org/SDL/releases/tag/release-3.2.10)

[SDL3 Image](https://github.com/libsdl-org/SDL_image/releases)

[SDL3 TTF](https://github.com/libsdl-org/SDL_ttf/releases/preview-3.1.0)

[LibXML2](https://github.com/GNOME/libxml2)

libsndfile

libogg

libvorbis

[OpenAL](https://www.openal.org/downloads/)

Just including a Visual Studio project at the moment, as that's what I'm developing this with.  It should build with any C++20 compiler that supports the above packages though.

SDL3 Mixer doesn't have an official release yet, so you'll have to build from the main repository.

You will also need the resources file (MainResources.rsrc) from the creator of the original program this work is based off of.  Place it in the bin folder of this program.

There are also a few files which didn't seem to be uploaded to the repo, but you can find them in the zip under the Releases.

https://github.com/beastie/ultima3

*******************************************************************************

While it initially supported SDL Mixer 3, due to there being no official version yet, development on that has been paused, and went to a purely OpenAL approach.

[SDL3 Mixer](https://github.com/libsdl-org/SDL_mixer/)

*******************************************************************************

Building for Mac

I'm not familiar with the ecosystem, so this is what I did to get it to compile.  Unfortunately, I can't distribute this, as it wants to sandbox everything.  The only way I know how to get it up and running is to do a local build.

Open up the Terminal.

Install Brew:

```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

Install Brew packages:

```
brew install llvm
brew install cmake
brew install libogg
brew install libsndfile
brew install libvorbis
brew install openal-soft
```
Download and install the necessary SDL3 Libraries (SDL3, SDL3Image, SDL3TTF) according to their directions.
It's basically downloading the appropriate library, and placing them in ~/Library/Frameworks.

You may have to also remove the quarantine attributes:
Go to the the library directory:
```
cd ~/Library/Frameworks

xattr -r -d com.apple.quarantine SDL3.xcframework
xattr -r -d com.apple.quarantine SDL3_image.xcframework
xattr -r -d com.apple.quarantine SDL3_ttf.xcframework
xattr -r -d com.apple.quarantine share
```
At this point, you should now have everything necessary to build.

*******************************************************************************

Building for non Visual Studio:

Navigate to the directory of CMakeLists.txt

mkdir build
cd build
cmake ../
make

*******************************************************************************

Adding new tilesets:

In the graphics directory, provided the image is a multiple of 12x16 (e.g. 192x256 for 16x16 tiles), and ends with **-Tiles**.gif/png, the game should pick it up automatically.  The same holds true for Font, Mini, and UI images.  But, you will need a Tiles image for the others to work.

TO DO: In the future, something similar will be there to allow easy replacement of audio.  For the time being, only the default ogg files are played, but you can replace them.  The ogg files currently need to be 22050 Hz and Mono channel in order to work with OpenAL.  (I believe the SDL Mixer version has more support.)

*******************************************************************************

Controls:

Escape quits the game regardless of where you are.  Yes, this is often a cancel selection or a menu hotkey in most games, but for the time being, it just exits.

To bring up the Options screen, press tab.

For debugging purposes, press Alt-F to show the fps.
