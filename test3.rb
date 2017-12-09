require "./lib/LiteRGSS.so"
include LiteRGSS
w = Config::ScreenWidth = 1280
h = Config::ScreenHeight = 720
Config::Title = "Test"
$LOAD_PATH << "."
Graphics.start
bmp = Bitmap.new("cb.bmp")
@b = Array.new(20_000) {
    sp = Sprite.new
    sp.bitmap = bmp
    sp.x = rand(w)
    sp.y = rand(h)
    sp.ox = 75
    sp.oy = 60
    sp.angle = rand(360)
    sp.zoom_x = sp.zoom_y = rand() / 2
    sp.opacity = rand(255)
    next(sp)
}
@a = Sprite.new
@a.bitmap = bmp
@a.ox = 75
@a.oy = 60
@a.angle = 45
@a.x = w / 2
@a.y = h / 2
@a.opacity = 128
@running = true
i = 0
Graphics.instance_variable_set(:@on_close, proc { @running = false }) # The block returns false, it's important !
t = Time.new
while @running
  Graphics.update
  add = 1 - rand(3)
  @a.angle = (@a.angle + (10 + 10 * Math::cos(i * Math::PI / 20))) % 360
  #@b.each do |sp| sp.set_position(sp.x + add, sp.y - add).angle += 1 end
  i += 1
end
dt = (Time.new - t)
p 1 / (dt / i)
Graphics.stop
