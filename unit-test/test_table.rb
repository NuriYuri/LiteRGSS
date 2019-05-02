#encoding: ASCII-8BIT
# Load required library
require_relative "../lib/LiteRGSS"
require "rspec"

# Start specification
RSpec.describe Table do
  data_table_111_dim_3 = "\x04\bu:\nTable\e\x03\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x00\x00"
  data_table_11_dim_2 = "\x04\bu:\nTable\e\x02\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x00\x00"
  data_table_1_dim_1 = "\x04\bu:\nTable\e\x01\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x01\x00\x00\x00\x00\x00"
  data_table_125_dim_3 = "\x04\bu:\nTable-\x03\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x05\x00\x00\x00\n\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
  data_table_with_values = "\x04\bu:\nTable%\x03\x00\x00\x00\x01\x00\x00\x00\x02\x00\x00\x00\x03\x00\x00\x00\x06\x00\x00\x00\x00\x00\x01\x00\x02\x00\x03\x00\x04\x00\x05\x00"
  data_table_filled_with_0 = "\x04\bu:\nTable)\x03\x00\x00\x00\x02\x00\x00\x00\x02\x00\x00\x00\x02\x00\x00\x00\b\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00"
  data_table_filled_with_minus_1 = "\x04\bu:\nTable)\x03\x00\x00\x00\x02\x00\x00\x00\x02\x00\x00\x00\x02\x00\x00\x00\b\x00\x00\x00\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF"
  data_table_filled_with_maxshort = "\x04\bu:\nTable)\x03\x00\x00\x00\x02\x00\x00\x00\x02\x00\x00\x00\x02\x00\x00\x00\b\x00\x00\x00\xFF\x7F\xFF\x7F\xFF\x7F\xFF\x7F\xFF\x7F\xFF\x7F\xFF\x7F\xFF\x7F"
  it 'load table from Serialized data' do
    expect(Marshal.load(data_table_111_dim_3).class).to eq(Table)
  end
  
  it 'load the correct dim (1)' do
    expect(Marshal.load(data_table_1_dim_1).dim).to eq(1)
  end
  
  it 'load the correct dim (2)' do
    expect(Marshal.load(data_table_11_dim_2).dim).to eq(2)
  end
  
  it 'load the correct dim (3)' do
    expect(Marshal.load(data_table_111_dim_3).dim).to eq(3)
  end
  
  it 'gives the correct ysize when dim = 1' do
    expect(Marshal.load(data_table_1_dim_1).ysize).to eq(1)
  end
  
  it 'gives the correct zsize when dim = 1' do
    expect(Marshal.load(data_table_1_dim_1).zsize).to eq(1)
  end
  
  it 'gives the correct zsize when dim = 2' do
    expect(Marshal.load(data_table_11_dim_2).zsize).to eq(1)
  end
  
  it 'gives the correct xsize' do
    expect(Marshal.load(data_table_125_dim_3).xsize).to eq(1)
  end
  
  it 'gives the correct ysize' do
    expect(Marshal.load(data_table_125_dim_3).ysize).to eq(2)
  end
  
  it 'gives the correct zsize' do
    expect(Marshal.load(data_table_125_dim_3).zsize).to eq(5)
  end
  
  it 'return nil when loading thing outside of the table' do
    table = Marshal.load(data_table_111_dim_3)
    arr = []
    arr << table[-1]
    arr << table[0, -1]
    arr << table[0, 0, -1]
    arr << table[-1, 0, 0]
    arr << table[0, -1, 0]
    arr << table[1]
    arr << table[0, 1]
    arr << table[0, 0, 1]
    arr << table[1, 0, 0]
    arr << table[0, 1, 0]
    50.times do arr << table[1 + rand(100), 0, 0] end
    50.times do arr << table[1 + rand(100), 1 + rand(100), 0] end
    50.times do arr << table[0, 1 + rand(100), 0] end
    50.times do arr << table[0, 0, 1 + rand(100)] end
    expect(arr).to eq(Array.new(arr.size))
  end
  
  it 'loads the correct values' do
    table = Marshal.load(data_table_with_values)
    arr = []
    arr << table[0, 0, 0]
    arr << table[0, 1, 0]
    arr << table[0, 0, 1]
    arr << table[0, 1, 1]
    arr << table[0, 0, 2]
    arr << table[0, 1, 2]
    expect(arr).to eq(Array.new(arr.size) { |i| i })
  end
  
  it 'saves the table with the right data with value' do
    expect(Marshal.dump(Marshal.load(data_table_with_values))).to eq(data_table_with_values)
  end
  
  it 'save the table with the right data 125 dim 3' do
    expect(Marshal.dump(Marshal.load(data_table_125_dim_3))).to eq(data_table_125_dim_3)
  end
  
  it 'save the table with the right data dim 1' do
    expect(Marshal.dump(Marshal.load(data_table_1_dim_1))).to eq(data_table_1_dim_1)
  end
  
  it 'save the table with the right data dim 2' do
    expect(Marshal.dump(Marshal.load(data_table_11_dim_2))).to eq(data_table_11_dim_2)
  end
  
  it 'save the table with the right data 111 dim 3' do
    expect(Marshal.dump(Marshal.load(data_table_111_dim_3))).to eq(data_table_111_dim_3)
  end
  
  it 'fill the table with 0' do
    expect(Marshal.dump(Table.new(2, 2, 2).fill(0))).to eq(data_table_filled_with_0)
  end
  
  it 'fill the table with -1' do
    expect(Marshal.dump(Table.new(2, 2, 2).fill(-1))).to eq(data_table_filled_with_minus_1)
  end
  
  it 'fill the table with 0x7FFF' do
    expect(Marshal.dump(Table.new(2, 2, 2).fill(0x7FFF))).to eq(data_table_filled_with_maxshort)
  end
  
  it 'makes the table smaller with the right size' do 
    table = Table.new(5, 5, 5)
    table.resize(1, 2, 3)
    arr = [table.xsize, table.ysize, table.zsize]
    expect(arr).to eq([1, 2, 3])
  end
  
  it 'makes the table bigger with the right size' do
    table = Table.new(1)
    table.resize(1, 2, 3)
    arr = [table.xsize, table.ysize, table.zsize]
    expect(arr).to eq([1, 2, 3])
  end
  
  it 'preserve the value on smaller resize (z)' do
    table = Marshal.load(data_table_with_values)
    table.resize(1, 2, 2)
    arr = []
    arr << table[0, 0, 0]
    arr << table[0, 1, 0]
    arr << table[0, 0, 1]
    arr << table[0, 1, 1]
    expect(arr).to eq(Array.new(arr.size) { |i| i })
  end
  
  it 'preserve the value on smaller resize (y)' do
    table = Marshal.load(data_table_with_values)
    table.resize(1, 1, 3)
    arr = []
    arr << table[0, 0, 0]
    arr << table[0, 0, 1]
    arr << table[0, 0, 2]
    expect(arr).to eq(Array.new(arr.size) { |i| i * 2 })
  end
  
  it 'preserve the value on bigger resize' do
    table = Marshal.load(data_table_with_values)
    table.resize(5, 5, 5)
    arr = []
    arr << table[0, 0, 0]
    arr << table[0, 1, 0]
    arr << table[0, 0, 1]
    arr << table[0, 1, 1]
    arr << table[0, 0, 2]
    arr << table[0, 1, 2]
    expect(arr).to eq(Array.new(arr.size) { |i| i })
  end
  
  it "doesn't copy the offset is invalid (< 0 || > x/ysize) : x<" do
    table = Table.new(1, 1, 1)
    expect(table.copy(table, -1, 0)).to be_falsey
  end
  
  it "doesn't copy the offset is invalid (< 0 || > x/ysize) : x>" do
    table = Table.new(1, 1, 1)
    expect(table.copy(table, 1, 0)).to be_falsey
  end
  
  it "doesn't copy the offset is invalid (< 0 || > x/ysize) : y<" do
    table = Table.new(1, 1, 1)
    expect(table.copy(table, 0, -1)).to be_falsey
  end
  
  it "doesn't copy the offset is invalid (< 0 || > x/ysize) : y>" do
    table = Table.new(1, 1, 1)
    expect(table.copy(table, 0, 1)).to be_falsey
  end
  
  it "copy the table" do
    src_table = Marshal.load(data_table_with_values)
    dst_table = Table.new(1, 2, 3).fill(0)
    dst_table.copy(src_table, 0, 0)
    arr = []
    arr << dst_table[0, 0, 0]
    arr << dst_table[0, 1, 0]
    arr << dst_table[0, 0, 1]
    arr << dst_table[0, 1, 1]
    arr << dst_table[0, 0, 2]
    arr << dst_table[0, 1, 2]
    expect(arr).to eq(Array.new(arr.size) { |i| i })
  end
  
  it "copy the table with an offset" do
    src_table = Marshal.load(data_table_with_values)
    dst_table = Table.new(2, 3, 3).fill(0)
    dst_table.copy(src_table, 1, 1)
    arr = []
    arr << dst_table[1, 1, 0]
    arr << dst_table[1, 2, 0]
    arr << dst_table[1, 1, 1]
    arr << dst_table[1, 2, 1]
    arr << dst_table[1, 1, 2]
    arr << dst_table[1, 2, 2]
    expect(arr).to eq(Array.new(arr.size) { |i| i })
  end
  
  it "copy the table with an offset and keep everything clean" do
    src_table = Marshal.load(data_table_with_values)
    dst_table = Table.new(3, 4, 4).fill(0)
    dst_table.copy(src_table, 1, 1)
    arr = []
    4.times do |z|
      3.times do |x|
        4.times do |y|
          next if z != 3 && x == 1 && y.between?(1, 2)
          arr << dst_table[x, y, z]
        end
      end
    end
    expect(arr).to eq(Array.new(arr.size, 0))
  end
  
  it "doesn't copy_modulo the dest offset is invalid (< 0 || > x/ysize) : x<" do
    table = Table.new(1, 1, 1)
    expect(table.copy_modulo(table, 0, 0, -1, 0, 1, 1)).to be_falsey
  end
  
  it "doesn't copy_modulo the dest offset is invalid (< 0 || > x/ysize) : x>" do
    table = Table.new(1, 1, 1)
    expect(table.copy_modulo(table, 0, 0, 1, 0, 1, 1)).to be_falsey
  end
  
  it "doesn't copy_modulo the dest offset is invalid (< 0 || > x/ysize) : y<" do
    table = Table.new(1, 1, 1)
    expect(table.copy_modulo(table, 0, 0, 0, -1, 1, 1)).to be_falsey
  end
  
  it "doesn't copy_modulo the dest offset is invalid (< 0 || > x/ysize) : y>" do
    table = Table.new(1, 1, 1)
    expect(table.copy_modulo(table, 0, 0, 0, 1, 1, 1)).to be_falsey
  end
  
  it "modulate copy a table w/out offset -> x" do
    src_table = Marshal.load(data_table_with_values)
    dst_table = Table.new(2, 2, 3).fill(0)
    dst_table.copy_modulo(src_table, 0, 0, 0, 0, 2, 2)
    arr = []
    2.times do |x|
      arr << dst_table[x, 0, 0]
      arr << dst_table[x, 1, 0]
      arr << dst_table[x, 0, 1]
      arr << dst_table[x, 1, 1]
      arr << dst_table[x, 0, 2]
      arr << dst_table[x, 1, 2]
    end
    expect(arr).to eq((Array.new(6) { |i| i }) * 2)
  end
  
  it "modulate copy a table w/out offset -> y" do
    src_table = Marshal.load(data_table_with_values)
    dst_table = Table.new(1, 4, 3).fill(0)
    dst_table.copy_modulo(src_table, 0, 0, 0, 0, 1, 4)
    arr = []
    0.step(2, 2) do |y|
      arr << dst_table[0, 0 + y, 0]
      arr << dst_table[0, 1 + y, 0]
      arr << dst_table[0, 0 + y, 1]
      arr << dst_table[0, 1 + y, 1]
      arr << dst_table[0, 0 + y, 2]
      arr << dst_table[0, 1 + y, 2]
    end
    expect(arr).to eq((Array.new(6) { |i| i }) * 2)
  end
  
  it "modulate copy a table w/ src offset -> x" do
    src_table2 = Marshal.load(data_table_with_values)
    src_table = Table.new(2, 2, 3).fill(-1)
    src_table.copy(src_table2, 1, 0) # result should be [ 6x -1, src_table2 ]
    dst_table = Table.new(2, 2, 3).fill(0)
    dst_table.copy_modulo(src_table, 1, 0, 0, 0, 2, 2) # So here because of the offset & modulo effect we should have [ src_table2, 6x -1 ]
    arr = []
    2.times do |x|
      arr << dst_table[x, 0, 0]
      arr << dst_table[x, 1, 0]
      arr << dst_table[x, 0, 1]
      arr << dst_table[x, 1, 1]
      arr << dst_table[x, 0, 2]
      arr << dst_table[x, 1, 2]
    end
    expect(arr).to eq((Array.new(6) { |i| i }) + Array.new(6, -1))
  end
  
  it "modulate copy a table w/ src offset -> y" do
    src_table = Marshal.load(data_table_with_values)
    dst_table = Table.new(1, 4, 3).fill(0)
    dst_table.copy_modulo(src_table, 0, 1, 0, 0, 1, 4)
    arr = []
    0.step(2, 2) do |y|
      arr << dst_table[0, 0 + y, 0]
      arr << dst_table[0, 1 + y, 0]
      arr << dst_table[0, 0 + y, 1]
      arr << dst_table[0, 1 + y, 1]
      arr << dst_table[0, 0 + y, 2]
      arr << dst_table[0, 1 + y, 2]
    end
    expect(arr).to eq((Array.new(6) { |i| i ^ 1 }) * 2) # Pure lazyness, sorry :v
    # The result we expect to have is the copy of the table with modulo effect but starting at y = 1 in src_table when y = 0 in dest_table
  end
  
  it "modulate copy a table w/ src(y) & dest(x,y) offset" do
    src_table = Marshal.load(data_table_with_values)
    dst_table = Table.new(2, 5, 3).fill(0)
    dst_table.copy_modulo(src_table, 0, 1, 1, 1, 1, 4)
    arr = []
    0.step(2, 2) do |y|
      arr << dst_table[1, 0 + y + 1, 0]
      arr << dst_table[1, 1 + y + 1, 0]
      arr << dst_table[1, 0 + y + 1, 1]
      arr << dst_table[1, 1 + y + 1, 1]
      arr << dst_table[1, 0 + y + 1, 2]
      arr << dst_table[1, 1 + y + 1, 2]
    end
    expect(arr).to eq((Array.new(6) { |i| i ^ 1 }) * 2) # Pure lazyness, sorry :v
    # The result we expect to have is the copy of the table with modulo effect but starting at y = 1 in src_table when y = 1 in dest_table
  end
  
  it "keeps the rest of the table clean with modulate copy" do
    src_table = Marshal.load(data_table_with_values)
    dst_table = Table.new(3, 6, 4).fill(0)
    dst_table.copy_modulo(src_table, 0, 1, 1, 1, 1, 4)
    arr = []
    4.times do |z|
      6.times do |y|
        3.times do |x|
          next if z != 3 && x == 1 && y.between?(1, 4)
          arr << dst_table[x, y, z]
        end
      end
    end
    expect(arr).to eq(Array.new(arr.size, 0))
  end
  
  # Note : there's stuff that cannot be tested on Ruby side so it'll be some kind of crash test to see if the C++ part doesn't segfault
  it "Crash test copy & copy_modulate function" do
    src_table = Table.new(500, 500, 1)
    dst_table = Table.new(1, 1, 1)
    500.times do
      dst_table.copy(src_table, 0, 0)
    end
    src_table = Table.new(1, 1, 1)
    500.times do 
      dst_table.copy_modulo(src_table, 0, 0, 0, 0, 500, 500)
    end
    expect(true).to be_truthy
  end
  
  it "Crash test loading tables" do
    arr = Array.new(1000) { Marshal.load(data_table_filled_with_maxshort) }
    expect(true).to be_truthy
  end
  
  it "Crash test saving tables" do
    table = Marshal.load(data_table_filled_with_maxshort)
    arr = Array.new(1000) { Marshal.dump(table) }
    expect(true).to be_truthy
  end
end
