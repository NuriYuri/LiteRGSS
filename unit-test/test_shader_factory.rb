# Load required library
require_relative "../lib/LiteRGSS_test"
include LiteRGSS
require "rspec"

# Start specification
RSpec.describe ShaderFactory do
  
  it 'toggle disabled' do
    shaderFcty = ShaderFactory.new()
    shaderFcty.disable
    expect(shaderFcty.enabled).to eq(false)
  end

end
