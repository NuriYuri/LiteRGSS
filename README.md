Lite RGSS
==
What is Lite RGSS ?
--
It's a library like the original RGSS that allows the Game creation in Ruby. Lite RGSS will focus on things that are easy and simple to do, that's why for no there wont be bending options, tone, wave on sprite and stuff like that.

I'm writting the library on my free time so I will not do spectacular thing ^^

What is currently done
--
* The Graphics.update function
* The creation of a Bitmap (+dispose)
* The creation of a Sprite (+dispose +bitmap=)
The rest will be done later.

What does Lite RGSS need to run ?
--
For now : Ruby and SFML.

How to build Lite RGSS ? (Under Linux)
--
First, install the dependencies :

    sudo gem install rake-compiler
    sudo apt-get install libsfml-dev
Then if it's the first time you build or if you updated the library, execute the following command where the "Rakefile" is present :

    rake clean
   And then compile the library using :
   

    rake compile

The library will be in the ./lib directory as an .so file, you can require it from a ruby script (using relative path).

How to build Lite RGSS under Windows ?
--

- Install [Ruby](https://rubyinstaller.org/ "Ruby Installer")
- Install the dev kit using `ridk install` (chose 3 and when everything is done press enter)
- Enable the dev kit using `ridk enable`
- Move the SFML file to msys64/mingw64 (download the MinGW version of SFML that match with your ruby architecture x64/x86 and move the lib, include and bin directories)
- Go to the LiteRGSS directory
- run `rake compile` (it'll probably takes a lot of time)

How to start correctly Lite RGSS ?
--
If you loaded the LiteRGSS library nothing can be shown until you called `LiteRGSS::Graphics.start`. This function takes the parameters of `LiteRGSS::Config` to build the Window.
**List of parameters :**
* ScreenWidth : The number of pixel wide the screen should be (between 160 and the max fullscreen resolution). / Default: 640
* ScreenHeight : The number of pixel high the screen should be (between 144 and the max fullscreen resolution). / Default: 480
* FrameRate : The number of frame per second. / Default: 60
* Title : The title of the window. / Default : LiteRGSS
* Vsync : If the game uses Vertical Synchronization. / Default : true

How to stop Lite RGSS ?
--
If the player did not hit the close button, you should call `LiteRGSS::Graphics.stop`. It'll destroy the game window. (But not the sprites, viewport and stuff.)

Specials Thanks
--
* SFML for their library https://www.sfml-dev.org
* NetSurf Browser for their gif library :) https://netsurf-browser.org
