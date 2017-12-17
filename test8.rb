require "./lib/LiteRGSS.so"
include LiteRGSS
w = Config::ScreenWidth = 1280
h = Config::ScreenHeight = 720
Config::Title = "Text Test"
$LOAD_PATH << "."
Graphics.start
@v = Viewport.new(w, h)
#@v.tone.set(0,0,0,240)
bmp = Bitmap.new("cb.bmp")
=begin
arr = Array.new(10000) do #4000 -> down
  sp = Sprite.new(@v)
  sp.bitmap = bmp
  sp.zoom = rand / 2
  sp.set_position(rand(w), rand(h))
  sp
end
=end
@a = Sprite.new(@v)
@a.bitmap = bmp
@a.ox = 75
@a.oy = 60
@a.angle = 45
@a.x = w / 2
@a.y = h / 2
@a.opacity = 200
@running = true
txt = Array.new
Fonts.load_font(0, "VLGOT.ttf")
Fonts.set_default_size(0, 16)
Fonts.define_fill_color(0, Color.new(240, 240, 240, 255))
Fonts.define_outline_color(0, Color.new(255, 0, 0, 255))
txt << Text.new(0, @v, 0, 0, 320, 16, "Test quelconque")
txt << Text.new(0, @v, 0, 16, 320, 16, "Test quelconque", 1, 1)
txt << Text.new(0, @v, 0, 32, 320, 16, "Test quelconque", 2) #txt << Text.new(0, @v, 0, 32, 320, 16, "Nuri Yuri", 2)
txt.last.fill_color = Color.new(240, 0, 0)
txt << Text.new(0, @v, 0, 48, 320, 16, "Test quelconque", 2)
txt.last.fill_color = Color.new(240, 0, 0)
20.times do |i|
  txt << Text.new(0, @v, 0, 64 + i * 16, 320, 16, "Test quelconque", 2)#rand(10**20).to_s(36), rand(3), rand(2))
end
txt << (@testtex = Text.new(0, @v, 320, 0, 320, 16, "Test quelconque"))
i = 0
GC.start
Graphics.instance_variable_set(:@on_close, proc { @running = false }) # The block returns false, it's important !
Graphics.transition(120)
t = Time.new
while @running
  Graphics.update
  @a.angle = (@a.angle + (10 + 10 * Math::cos(i * Math::PI / 20))) % 360
  i += 1
  if(i%60 == 0)
    @testtex.y += 1
    @testtex.text = "#{@testtex.y}"
  end
end
dt = (Time.new - t)
p 1 / (dt / i)
t = Time.new
@v.dispose
p Time.new - t
Graphics.update
#sleep(1)
Graphics.stop
