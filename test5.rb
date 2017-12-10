require "./lib/LiteRGSS.so"
include LiteRGSS
w = Config::ScreenWidth = 1280
h = Config::ScreenHeight = 720
Config::Title = "Test"
$LOAD_PATH << "."
Graphics.start
@v = Viewport.new(w, h)
#@v.ox = w / 4
x = @v.ox = w / 2 - 50
y = @v.oy = h / 2 - 40
@v.rect.set(x, y, 100, 80)
#> Graphic Test
bmp = Bitmap.new("cb.bmp")
@a = Sprite.new(@v)
@a.bitmap = bmp
@a.ox = 75
@a.oy = 60
@a.angle = 45
@a.x = w / 2
@a.y = h / 2
@a.opacity = 50
@running = true
i = 0
Graphics.instance_variable_set(:@on_close, proc { @running = false }) # The block returns false, it's important !
t = Time.new
while @running
  Graphics.update
  add = 1 - rand(3)
  @a.angle = (@a.angle + (10 + 10 * Math::cos(i * Math::PI / 20))) % 360
  i += 1
end
dt = (Time.new - t)
p 1 / (dt / i)
p @v.disposed?, @a.disposed? # false false
@v.dispose
p @a.disposed? #true
20.times do Graphics.update end
Graphics.stop
#@a.dispose #crash ? => test5.rb:38:in `dispose': Disposed Sprite. (LiteRGSS::Error)
