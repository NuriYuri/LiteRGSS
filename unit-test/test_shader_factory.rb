# Load required library
require_relative "../lib/LiteRGSS_test"
include LiteRGSS
require "rspec"

# Start specification
RSpec.describe ShaderFactory do
  
  it 'toggle disabled' do
    shaderFcty = ShaderFactory.new()
    shaderFcty.disable
    expect(shaderFcty.enabled?).to eq(false)
  end

  it 'create when disabled' do
    shaderFcty = ShaderFactory.new()
    shaderFcty.disable
    shaderFcty.create
    expect(shaderFcty.shader_exists?).to eq(false)
  end

  # Those following tests will only work if your computer supports shaders :
  it 'create when enabled' do
    shaderFcty = ShaderFactory.new()
    shaderFcty.create
    expect(shaderFcty.shader_exists?).to eq(true)
  end
  
  it 'enabled by default' do
    shaderFcty = ShaderFactory.new()
    expect(shaderFcty.enabled?).to eq(true)
  end
end
