require "./lib/LiteRGSS.so"
include LiteRGSS
w = Config::ScreenWidth = 1280
h = Config::ScreenHeight = 720
Config::Vsync = false
Config::Title = "Text Test"
$LOAD_PATH << "."
Graphics.start
@v = Viewport.new(w, h)
@v2 = Viewport.new(w, h)
@v.tone.set(0,0,0,0) #> Moins de pb avec les textes ?
bmp = Bitmap.new("cb.bmp")
arr = Array.new(1500) do #4000 -> down
  sp = Sprite.new(rand(2) == 0 ? @v : @v2)
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
txt = Array.new
Fonts.load_font(0, "PokemonDS.ttf")
Fonts.set_default_size(0, 13)
Fonts.define_fill_color(0, Color.new(240, 240, 240, 255))
Fonts.define_outline_color(0, Color.new(64, 64, 180, 255))
Fonts.define_shadow_color(0, Color.new(180, 128, 128, 255))
txt << Text.new(0, @v, 0, 0, 320, 16, "ç¤ù*§qaéèàïîê")
txt << Text.new(0, @v, 0, 16, 320, 16, "Test quelconque", 1, 2)
txt.last.nchar_draw = 9
txt << Text.new(0, @v, 0, 32, 320, 16, "Test quelconque", 2) #txt << Text.new(0, @v, 0, 32, 320, 16, "Nuri Yuri", 2)
txt.last.fill_color = Color.new(240, 0, 0)
txt << Text.new(0, @v, 0, 48, 320, 16, "Test quelconque", 2)
txt.last.fill_color = Color.new(240, 0, 0)
txt.last.draw_shadow = false
40.times do |i|
  txt << Text.new(0, @v, 0, 64 + i * 16, 1280, 16, rand(10**180).to_s(36), rand(3), rand(2))
end
i = 0
GC.start
Graphics.instance_variable_set(:@on_close, proc { @running = false }) # The block returns false, it's important !
Graphics.transition(120)
t = Time.new
while @running
  Graphics.update
  @a.angle = (@a.angle + (10 + 10 * Math::cos(i * Math::PI / 20))) % 360
  i += 1
end
dt = (Time.new - t)
p 1 / (dt / i)
Graphics.stop
=begin
@v.dispose
@v2.dispose
=end
