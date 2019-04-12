#encoding: ASCII-8BIT
# Load required library
require "./lib/LiteRGSS_test"
require "rspec"

# Start specification
RSpec.describe Toto do
  it 'test titi' do
    toto = Toto.new()
    toto.titi.ysize
    
  end
end
