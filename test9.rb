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
Config::Title = "Test Input"
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
  if(Input.press?(:B))
    i -= 1
    @a.angle = i % 360
  elsif(Input.press?(:A))
    i += 1
    @a.angle = i % 360
  end
  case Input.dir4
  when 8
    @a.y -= 1
  when 2
    @a.y += 1
  when 4
    @a.x -= 1
  when 6
    @a.x += 1
  end
  if(Input.trigger?(:X))
    @running = false
  elsif(Input.repeat?(:Y))
    i += 36
    @a.angle = i % 360
  elsif(Input.trigger?(:L))
    i += 180
    @a.angle = i % 360
  end
end
Graphics.stop
