# File: extconf.rb
require 'mkmf'
require 'rbconfig'

extension_name = 'phidgets_native'

HEADER_DIRS = [ '/Library/Frameworks/Phidget21.framework/Headers' ]

find_header 'phidget21.h', *HEADER_DIRS
find_library 'libphidget21', 'CPhidget_getLibraryVersion', '/usr/lib/'

case RbConfig::CONFIG['host_os']
  when /mswin|msys|mingw|cygwin|bccwin|wince|emc/
    raise StandardError, "Though possible to compile in win32, this extconf wasn't designed for it.... Fix me?"
  when /darwin|mac os/
    have_framework 'Phidget21' 
  else
    have_library('phidget21') 
end

dir_config extension_name

$CFLAGS << ' -Wno-unused-variable -Wno-declaration-after-statement -std=gnu99'

create_makefile extension_name
