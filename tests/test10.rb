require "./lib/LiteRGSS.so"
include LiteRGSS
res = Graphics.list_resolutions
res << [640, 480] if res.size == 0
res2 = res.select { |i| i[0] * 3 / 4 == i[1] }
res2 = res if res2.size == 0
w = Config::ScreenWidth = res2[0][0]
h = Config::ScreenHeight = res2[0][1]
Config::Vsync = false
#Config::FullScreen = true
Config::Title = "Test Mouse"
$LOAD_PATH << "."
Graphics.start
@v = Viewport.new(w, h)
bmp = Bitmap.new("cb.bmp")
@a = Sprite.new(@v)
@a.bitmap = bmp
@a.ox = 75
@a.oy = 60
#@a.angle = 45
@a.x = w / 2
@a.y = h / 2
@a.opacity = 200
@running = true
GC.start
Graphics.instance_variable_set(:@on_close, proc { @running = false }) # The block returns false, it's important !
Graphics.transition(120)
t = Time.new
i = 0
while @running
  Graphics.update
  if(Input.trigger?(:B))
    @running = false
  end
  if(Mouse.press?(:left))
    @a.x = Mouse.x
    @a.y = Mouse.y
  elsif(Mouse.trigger?(:right))
    p Mouse.wheel
    Mouse.wheel = 0
  end
  @a.angle = Mouse.wheel % 360
end
Graphics.stop
