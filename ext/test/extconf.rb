require 'mkmf'
require 'fileutils'

ext_name = 'LiteRGSS'
test_name = ext_name + '_test'
current_dir_path = File.expand_path(File.dirname(__FILE__)) 
root_dir = File.expand_path(current_dir_path + '/../..')

src_dir = root_dir + '/ext/' + ext_name
test_dir = root_dir + '/ext/' + test_name

#TODO copy this folder to LiteRGSS_test and start compiling
#FileUtils.cp(root_dir + '/lib/LiteRGSS.so', root_dir + '/tmp/' + ARCH + '/' + ext_name + '/test/2.5.3/LiteRGSS.dll' )
#FileUtils.cp_r(src_dir, test_dir)


C_EXT = %w[c m]
CXX_EXT = %w[cc mm cxx cpp]
unless File.exist?(File.join(*File.split(__FILE__).tap {|d, b| b.swapcase}))
    CXX_EXT.concat(%w[C])
end
HDR_EXT = %w[h hpp]
ALL_EXT = C_EXT + CXX_EXT + HDR_EXT

all_files = Dir[File.join(src_dir, "**/*.{#{ALL_EXT.join(%q{,})}}")].sort

FileUtils.cp_r(all_files, test_dir)
all_files_here = Dir[File.join(test_dir, "**/*.{#{ALL_EXT.join(%q{,})}}")].sort



have_library('sfml-graphics')
have_library('sfml-window')
have_library('sfml-system')

create_makefile(test_name)

FileUtils.rm_r(all_files_here)