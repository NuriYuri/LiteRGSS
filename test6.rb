require "./lib/LiteRGSS.so"
include LiteRGSS
w = Config::ScreenWidth = 1280
h = Config::ScreenHeight = 720
Config::Title = "Test"
$LOAD_PATH << "."
Graphics.start
@v = Viewport.new(w, h)
@v.rect.width = w * 2 / 3
#@v.tone.set(255, -255, 0, 0)
#@v.tone.set(-255, -255, 0, 0)
@v.tone.set(-255, 0, -255, 255)

#> Graphic Test
bmp = Bitmap.new("cb.bmp")
arr = Array.new(10000) do #4000 -> down
  sp = Sprite.new(@v)
  sp.bitmap = bmp
  sp.zoom = rand / 2
  sp.set_position(rand(w), rand(h))
  sp
end
@a = Sprite.new(@v)
@a.bitmap = bmp
@a.ox = 75
@a.oy = 60
@a.angle = 45
@a.x = w / 2
@a.y = h / 2
@a.opacity = 200


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
tone = @v.tone
@v.dispose
tone.red = 0
Graphics.stop
