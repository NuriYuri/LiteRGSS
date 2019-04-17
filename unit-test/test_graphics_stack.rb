#encoding: ASCII-8BIT
# Load required library
require_relative "../lib/LiteRGSS_test"
include LiteRGSS
require "rspec"

Graphics.start

class DrawableStack
  # Sort the z sprites inside the viewport
  def sort_z
    #@__elementtable.delete_if do |el| el.disposed? end
    @__elementtable.sort! do |a, b| 
      s = a.z <=> b.z
      next(a.__index__ <=> b.__index__) if s == 0
      next(s)
    end
  end
end

# Start specification
RSpec.describe DrawableStack do
  
  it 'add drawable to stack' do
    stack = DrawableStack.new()
    @a = Sprite.new()
    expect(stack.size).to eq(0)
    stack.add(@a)
    expect(stack.size).to eq(1)
  end
  
  it 'add & remove drawable to stack' do
    stack = DrawableStack.new()
    @a = Sprite.new()
    stack.add(@a)
    stack.remove(@a)
    expect(stack.size).to eq(0)
  end

  it 'delete a sprite added' do
    stack = DrawableStack.new()
    @a = Sprite.new()
    stack.add(@a)
    @a = nil
    #TODO : fix this
    #GC.start()
    #expect(stack.size).to eq(0)
  end

  it 'sync stack' do
    stack = DrawableStack.new()
    @a = Sprite.new()
    @b = Sprite.new()
    @c = Sprite.new()

    a_index = @a.__index__
    b_index = @b.__index__
    c_index = @c.__index__

    stack.add(@c)
    stack.add(@a)
    stack.add(@b)
    
    stack.sort_z

    expect(stack.getindex(0)).to eq(c_index)
    expect(stack.getindex(1)).to eq(a_index)
    expect(stack.getindex(2)).to eq(b_index)

    stack.syncstack

    expect(stack.getindex(0)).to eq(a_index)
    expect(stack.getindex(1)).to eq(b_index)
    expect(stack.getindex(2)).to eq(c_index)
  end

end
