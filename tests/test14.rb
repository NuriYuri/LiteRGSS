require "./lib/LiteRGSS.so"
include LiteRGSS
Config::ScreenWidth = 1280
Config::ScreenHeight = 960
Config::Title = "Test"
Config::Vsync = true
Config::FrameRate = 60
shader_string = <<-EOF
uniform float param;
uniform float param2;
uniform float xfactor;
uniform sampler2D texture;
void main()
{
  vec4 frag = texture2D(texture, gl_TexCoord[0].xy);
  float x = gl_TexCoord[0].x - 0.5;
  float y = gl_TexCoord[0].y - 0.5;
  x *= xfactor;
  x *= x;
  y *= y;
  float r2 = x + y;
  if(param < r2)
  {
    frag.a = 0;
  }
  else if(param2 < r2)
  {
    frag.r = 0;
  }
  gl_FragColor = frag;
}
EOF


$LOAD_PATH << "."
Graphics.start
Graphics.set_window_framerate(0)
@a = ShaderedSprite.new
@a.bitmap = Bitmap.new("test.jpg")
@bmp = Bitmap.new("cb.bmp")
@shader = Shader.new(shader_string, 2)
@shader.set_float_uniform("xfactor", @a.bitmap.width.to_f / (h = @a.bitmap.height))
@a.shader = @shader
@running = true
Graphics.instance_variable_set(:@on_close, proc { @running = false }) # The block returns false, it's important !
GC.start
i = 0
t = Time.new
while @running
  @shader.set_float_uniform("param", (r = (120.0 - (i % 120)) / 120) ** 2)
  @shader.set_float_uniform("param2", ((r * h - 40) / h) ** 2)
  Graphics.update
  i += 1
end
dt = (Time.new - t)
p 1 / (dt / i)
Graphics.stop
