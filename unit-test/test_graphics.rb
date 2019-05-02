# Load required library
require_relative "../lib/LiteRGSS_test"
include LiteRGSS
require "rspec"
require "base64"

# Start specification
RSpec.describe Graphics do
  
  it 'start then start doesn\'t crash' do
    Graphics.start
    Graphics.start
  end

  it 'stop then stop (error)' do
    Graphics.start
    Graphics.stop
    expect{Graphics.stop}.to raise_error(Graphics::StoppedError)
  end

  it 'resize_screen' do
    Graphics.start
    @a = Sprite.new()
    rawBmp = Base64.decode64("iVBORw0KGgoAAAANSUhEUgAAAAUA
    AAAFCAYAAACNbyblAAAAHElEQVQI12P4//8/w38GIAXDIBKE0DHxgljNBAAO
        9TXL0Y4OHwAAAABJRU5ErkJggg==")
    @a.bitmap = Bitmap.new(rawBmp, true)

    Graphics.update
    Graphics.resize_screen(1280, 720)    
    Graphics.update
  end
  
  it 'stop Graphics then update (error)' do
    Graphics.start
    Graphics.stop
    expect{Graphics.update}.to raise_error(Graphics::StoppedError)
  end

  it 'stop Graphics then restart & update' do
    Graphics.start
    Graphics.stop
    Graphics.start
    Graphics.update
  end

end
