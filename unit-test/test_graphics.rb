#encoding: ASCII-8BIT
# Load required library
require_relative "../lib/LiteRGSS_test"
include LiteRGSS
require "rspec"

Graphics.start

# Start specification
RSpec.describe Graphics do
  
  it 'resize_screen' do
    Graphics.update
    Graphics.resize_screen(1280, 720)
    Graphics.update
  end
  
end
