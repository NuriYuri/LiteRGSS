require 'mkmf'
test_name = 'LiteRGSS_test'
have_library('sfml-graphics')
have_library('sfml-window')
have_library('sfml-system')

LIBDIR     = RbConfig::CONFIG['libdir']
INCLUDEDIR = RbConfig::CONFIG['includedir']

HEADER_DIRS = [INCLUDEDIR]

LIB_DIRS = [LIBDIR, File.expand_path(File.join(File.dirname(__FILE__), ["lib"]))]

# array of all libraries that the C extension should be compiled against
libs = ['-lLiteRGSS']

dir_config(test_name, HEADER_DIRS, LIB_DIRS)

# iterate though the libs array, and append them to the $LOCAL_LIBS array used for the makefile creation
libs.each do |lib|
  $LIBS << "#{lib} "
end


create_makefile(test_name)
