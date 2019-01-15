require "./lib/LiteRGSS.so"
include LiteRGSS
Config::ScreenWidth = 1280
Config::ScreenHeight = 960
Config::Title = "Test"
Config::Vsync = true
Config::FrameRate = 60
shader_string = <<-EOF
uniform float param;
uniform sampler2D texture;
uniform sampler2D transition;
const float sensibilite = 0.05;
const float scale = 1.0 + sensibilite;
void main()
{
  vec4 frag = texture2D(texture, gl_TexCoord[0].xy);
  vec4 tran = texture2D(transition, gl_TexCoord[0].xy);
  float pixel = max(max(tran.r, tran.g), tran.b);
  pixel -= (param * scale);
  if(pixel < sensibilite)
  {
    frag.a = max(0.0, sensibilite + pixel / sensibilite);
  }
  gl_FragColor = frag;
}
EOF


$LOAD_PATH << "."
Graphics.start
@a = ShaderedSprite.new
@a.bitmap = Bitmap.new("test.jpg")
@bmp = Bitmap.new("battle2.png")
@shader = Shader.new(shader_string, 2)
@shader.set_texture_uniform("transition", @bmp)
@a.shader = @shader
@running = true
Graphics.instance_variable_set(:@on_close, proc { @running = false }) # The block returns false, it's important !
GC.start
i = 0
t = Time.new
while @running
  @shader.set_float_uniform("param", (i % 121) / 120.0)
  Graphics.update
  i += 1
end
dt = (Time.new - t)
p 1 / (dt / i)
Graphics.stop
