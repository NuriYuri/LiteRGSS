require "./lib/LiteRGSS.so"
include LiteRGSS
Config::ScreenWidth = 320
Config::ScreenHeight = 240
Config::Title = "Test"
$LOAD_PATH << "."
Graphics.start
@a = Sprite.new
@a.bitmap = Bitmap.new("cb.bmp")
Graphics.update while true
