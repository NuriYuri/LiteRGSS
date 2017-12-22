## LiteRGSS

`LiteRGSS` is a little Ruby Game library that reproduces some features of the RGSS. It has been written with `SFML` for the Graphical part.

Nuri Yuri wrote LiteRGSS during his free time, that's why [SFML](https://www.sfml-dev.org/index-fr.php"SFML") has been used (easy to use, nothing to learn) and wanted to have a RGSS like library that was usable directly with the MRI (to allow thing like `require "socket"` which is impossible with `mkxp` and other RGSS like library)

### Dependencies

LiteRGSS (Graphic) depends on `SFML` and Ruby (`MRI`). On an debian system (Debian, /.*buntu/) you can install these by writting the following commands :
```sudo apt install libsfml-dev ruby```

**Note** : Don't forget to credit SFML with a little splash or something, that would be great :)

### Building under Linux

I currently cannot build the LiteRGSS for all platforms (because I don't have these and the version of Ruby and SFML differ on every platforms). You would also want to modify some things inside the LiteRGSS core so here is the way to build LiteRGSS :

- Install [rake-compiler](https://github.com/rake-compiler/rake-compiler"rake-compiler") : `sudo gem install rake-compiler`
- Clone the repo
- Go to the folder that contains the "Rakefile"
- run `rake compile`

The result will be inside the lib directory as `LiteRGSS.so` move it to your project and don't forget to give the dependencies with it.

### Building under Windows

- Install [Ruby](https://rubyinstaller.org/ "Ruby Installer")
- Install the dev kit using `ridk install` (chose 3 and when everything is done press enter)
- Enable the dev kit using `ridk enable`
- Move the SFML file to msys64/mingw64 (download the MinGW version of SFML that match with your ruby architecture x64/x86 and move the lib, include and bin directories)
- Go to the LiteRGSS directory
- run `rake compile` (it'll probably takes a lot of time)

### Basic example of the LiteRGSS use

Your main script :

    require 'LiteRGSS'
    require 'Yourscripts.rb'
    require 'Configurations.rb' #> Set the LiteRGSS::Config constants here
    
    Graphics.start #> Display the main window
    
    $scene = Scene_Title.new
    $scene.main while $scene
    
    Graphics.stop #> Close the main window
   Inside your `$scene.update` method :

    def update
      Graphics.update #> Show the last frame and fetch the new inputs for the current frame
      do_things_with_inputs
    end
