require_relative 'mkmf_unit_test'
require 'fileutils'

have_library('sfml-graphics')
have_library('sfml-window')
have_library('sfml-system')

current_dir_path = File.expand_path(File.dirname(__FILE__)) 
CONFIG["codedir"] = current_dir_path + '/../LiteRGSS'

create_makefile("LiteRGSS_test", nil, 'LiteRGSS')
