#encoding: ASCII-8BIT
# Load required library
require_relative "../lib/LiteRGSS_test"
include LiteRGSS
require "rspec"

Graphics.start

# Start specification
RSpec.describe DrawableStack do
  
  stack = DrawableStack.new()

  it 'add drawable to stack' do
    @a = Sprite.new()
    expect(stack.size()).to eq(0)
    stack.add(@a)
    expect(stack.size()).to eq(1)
  end
  
  it 'sync stack' do
    stack.syncstack  
  end
end
