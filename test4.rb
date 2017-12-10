require "./lib/LiteRGSS.so"
include LiteRGSS
w = Config::ScreenWidth = 1280
h = Config::ScreenHeight = 720
Config::Title = "Test"
$LOAD_PATH << "."
Graphics.start
puts "stx"
@b = Sprite.new
t = Time.new
@b.bitmap = Bitmap.new(20, 20)
puts "bmp"
puts Time.new - t
#> rect Test
puts @b.src_rect
puts rc = Marshal.load(Marshal.dump(@b.src_rect))
puts @b.src_rect.clone
puts rc == @b.src_rect
rc.width = -rc.width
puts rc == @b.src_rect

puts Rect.new(20, 40) # 0, 0, 20, 40
puts Rect.new(5, 20, 40) #5, 0, 20, 40
puts rc = Rect.new(5, 6, 20, 40) #5, 6, 20, 40
puts rc.set(nil, nil, 30, nil) #5, 6, 30, 40
puts rc.set(0, nil, nil, nil) #0, 6, 30, 40
puts rc.set(1, 2, 3, 4) #1, 2, 3, 4
puts rc == [1, 2, 3, 4] # true
puts rc == [0, 1, 2, 3] # false
puts rc == [1, 2, 3, 4, 5] #false
puts rc == [1] # false

#> Graphic Test
bmp = Bitmap.new("cb.bmp")
@a = Sprite.new
@a.bitmap = bmp.clone
@a.ox = 75 / 2
@a.oy = 60 / 2
@a.angle = 45
@a.x = w / 2
@a.y = h / 2
@a.opacity = 128
#@a.src_rect.set(nil, nil, 75, 60) OK
@a.src_rect.set(75 / 2, 30, 75, 60)
@a.src_rect.clone.set(0, 0, 150, 120) # N'affecte pas le sprite
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
Graphics.stop
