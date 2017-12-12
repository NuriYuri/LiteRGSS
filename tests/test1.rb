require "./lib/LiteRGSS.so"
include LiteRGSS
Config::ScreenWidth = 320
Config::ScreenHeight = 240
Config::Title = "Test"
$LOAD_PATH << "."
Graphics.start
@a = Sprite.new
@a.bitmap = Bitmap.new("cb.bmp")
@running = true
Graphics.instance_variable_set(:@on_close, proc { @running = false }) # The block returns false, it's important !
arr = []
sum = 0
20.times do 
  t = Time.new
  Bitmap.new(8192, 8192)
  dt = Time.new-t
  arr << dt
  sum += dt
end
p sum / arr.size, sum
p arr
GC.start
i = 0
t = Time.new
while @running
  Graphics.update
  i += 1
end
dt = (Time.new - t)
p 1 / (dt / i)
Graphics.stop
