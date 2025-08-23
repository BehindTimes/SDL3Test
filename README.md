Requirements:

C++20

[SDL3](https://github.com/libsdl-org/SDL/releases/tag/release-3.2.10)

[SDL3 Image](https://github.com/libsdl-org/SDL_image/releases)

[SDL3 TTF](https://github.com/libsdl-org/SDL_ttf/releases/preview-3.1.0)

[SDL3 Mixer](https://github.com/libsdl-org/SDL_mixer/)

[LibXML2](https://github.com/GNOME/libxml2)

Just including a Visual Studio project at the moment, as that's what I'm developing this with.  It should build with any C++20 compiler that supports the above packages though.

SDL3 Mixer doesn't have an official release yet, so you'll have to build from the main repository.

You will also need the resources file (MainResources.rsrc) from the creator of the original program this work is based off of.  Place it in the bin folder of this program.

There are also a few files which didn't seem to be uploaded to the repo, but you can find them in the zip under the Releases.

https://github.com/beastie/ultima3

For debugging purposes, press Alt-F to show the fps.

*******************************************************************************

As an alternative to SDL Mixer 3, I added some barebones support for OpenAL.

I tried compiling on a Mac, and everything seems to work, but I haven't done thorough testing.  I'm still not sure how to use that ecosystem properly, so I'm not encluding an XCode project or release.

For OpenAL, you'll need

libsndfile

libogg

libvorbis

OpenAL
