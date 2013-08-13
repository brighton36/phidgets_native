# File: extconf.rb
require 'mkmf'

extension_name = 'phidgets_native'

HEADER_DIRS = [ '/Library/Frameworks/Phidget21.framework/Headers' ]

find_header 'phidget21.h', *HEADER_DIRS
find_library 'libphidget21', 'CPhidget_getLibraryVersion', '/usr/lib/'

have_framework 'Phidget21' 
have_library('phidget21') 

dir_config extension_name

$CFLAGS << ' -Wno-unused-variable -std=gnu99'

create_makefile extension_name
