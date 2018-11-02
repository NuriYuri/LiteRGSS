require "./lib/LiteRGSS.so"
include LiteRGSS
Config::ScreenWidth = 160
Config::ScreenHeight = 120
Graphics.start
puts " AX AY L U D R 4"
loop do
  if ARGV.size == 0
    print(sprintf("\r %02d %02d %d %d %d %d %d", 
      Input.joy_axis_position(0, Input::JoyAxisX).to_i,
      Input.joy_axis_position(0, Input::JoyAxisY).to_i,
      Input.press?(:LEFT) ? 1 : 0,
      Input.press?(:UP) ? 1 : 0,
      Input.press?(:DOWN) ? 1 : 0,
      Input.press?(:RIGHT) ? 1 : 0,
      Input.dir4
    ))
  end
  Graphics.update
end