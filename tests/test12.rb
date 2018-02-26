require "./lib/LiteRGSS.so"
include LiteRGSS
Config::ScreenWidth = 1280
Config::ScreenHeight = 960
Config::Title = "Test"
Config::Vsync = true
Config::FrameRate = 60
shader_string = <<-EOF
uniform sampler2D texture;
uniform sampler2D texture2;
uniform vec4 tone;
const vec3 lumaF = vec3(.299, .587, .114);
void main()
{
  vec4 frag = texture2D(texture, gl_TexCoord[0].xy);
  int x = int(gl_TexCoord[0].x * 3);
  int y = int(gl_TexCoord[0].y * 3);
  x = x - (x / 2) * 2;
  y = y - (y / 2) * 2;
  if(y == x)
  {
    float luma = dot(frag.rgb, lumaF);
    frag.rgb = mix(frag.rgb, vec3(luma), tone.w);
    frag.a *=  gl_Color.a;
  }
  else
  {
    vec4 frag2 = texture2D(texture2, gl_TexCoord[0].xy);
    frag.rgb *= frag2.rgb;
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
@shader.set_float_uniform("tone", [0.0, 0.0, 0.0, 1.0])
@shader.set_texture_uniform("texture2", @bmp)
@a.shader = @shader
@running = true
Graphics.instance_variable_set(:@on_close, proc { @running = false }) # The block returns false, it's important !
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
