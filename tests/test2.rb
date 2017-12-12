require "./lib/LiteRGSS.so"
include LiteRGSS
Config::ScreenWidth = 320
Config::ScreenHeight = 240
Config::Title = "Test"
$LOAD_PATH << "."
Graphics.start
@a = Sprite.new
@a.bitmap = Bitmap.new("cb.bmp")
@a.ox = 75
@a.oy = 60
@a.angle = 45
@a.x = 160
@a.y = 120
120.times do
Graphics.update
@a.x += 1
@a.y += 1
@a.angle += 1
end
Graphics.stop
