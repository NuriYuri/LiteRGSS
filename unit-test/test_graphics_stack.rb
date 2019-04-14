#encoding: ASCII-8BIT
# Load required library
require_relative "../lib/LiteRGSS_test"
include LiteRGSS
require "rspec"

Graphics.start

# Start specification
RSpec.describe DrawableStack do
  
  it 'add drawable to stack' do
    stack = DrawableStack.new()
    @a = Sprite.new()
    expect(stack.size()).to eq(0)
    stack.add(@a)
    expect(stack.size()).to eq(1)
  end
  
  it 'sync stack' do
    stack = DrawableStack.new()
    @a = Sprite.new()
    stack.add(@a)
    stack.syncstack  
    expect(stack.size()).to eq(1)
  end
end
