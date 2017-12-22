require 'mkmf'
ext_name = 'LiteRGSS'

have_library('sfml-graphics')
have_library('sfml-window')
have_library('sfml-system')

create_makefile(ext_name)
