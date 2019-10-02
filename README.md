#  LibVLC-GLUT #
## Description ##
This is a simple test application I put together in order to help me develop and improve support using LibVLC to playback video into a dynamically sized memory buffer.

It renders video to a memory buffer that is subsequently used as a texture for some OpenGL geometry (okay, just 2 triangles) that fill the screen via an orthographic projection. This isn't interesting or useful visually but it does mean that it is easy to experiment with playback and scaling by simply resizing the application window.
## Building ##
This is the recipe I use to build a working version. There are lots of corners you can cut once you are familiar with how it works. 
* Grab a version of VLC you want to build against from the [VideoLAN](http://download.videolan.org/pub/videolan/vlc/) site. I used v3.0.8 from August 2019 for initial development.
  * For Windows builds at least, be sure to grab the .7z version of the package - some of the other versions don't appear to have everything you need. You can use a tool like [7-Zip](https://www.7-zip.org/) to unpack it if you don't already have a tool installed.
  * Unpack the archive and place the contents in a well specified location. A good place to put it is in the `vlc` folder inside this project repository but it can be located anywhere. II used `vlc/vlc-3.0.8` for initial development.
* Run the `make.bat` helper script with a parameter of the VLC package you downloaded. I used `make.bat vlc/vlc-3.0.8` for initial development.
* This will run CMake with appropriate parameters and generate a Visual Studio solution file you can load and run (`F5`) in the usual way. Pressing the `ESC` will exit. 
* It's a really simple batch file and easy to modify if you want to use a different CMake generator or include other parameters.
## Future ##
* It plays back video streams if you specify the URL of a supported format.
* Resizing the window (and therefore scaling the stream) does not work - that's next on the list and the main reason for this test.
* Would be helpful to add a way to visit lots of different streams, one after the other for testing. FreeGLUT doesn't (I think) offer much in the way of UI buck clicking on the left or right half of the screen might be an easy way to go back and forwards through a list.
* When the resizing code is working robustly, I would like to understand how to make the transport controls (play, stop, slow, reverse, seek etc.) work properly.
## FAQ ##
* Does it work on Windows?
  * It was developed on Windows 10 64bit using Visual Studio 2017. You can specify a 32bit Windows generator for CMake and build against a 32bit version of VLC if you want a 32bit version.
* Which version of Visual Studio is supported?
  * It was built using Visual Studio 2017 but I expect it works with 2013 and 2019. Give it a try and let me know!
* Does it work on Linux or macOS?
  * Not right now but since it uses CMake and GLUT, it should be very straightforward to port.
* Why use FreeGLUT?
  * [GLUT](https://en.wikipedia.org/wiki/OpenGL_Utility_Toolkit) is useful because it makes it easy to display textured OpenGL geometry in a simple, cross platform way. [FreeGLUT](http://freeglut.sourceforge.net/) is a more modern alternative that is actively maintained
## Contact ##
I always love to hear from people with fixes, ideas for improvements and suggestions etc. so please do get in touch via my [web site](https://callum.com). Cheers!
