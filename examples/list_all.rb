#!/usr/bin/env ruby
# encoding: UTF-8

$:.push *['%s/../lib/', '%s/lib/'].collect{|p| p % File.dirname(__FILE__) }
require 'console_table'
require 'phidgets_native'

#PhidgetsNative.enable_logging! :verbose

puts "Using Library version: "+PhidgetsNative::LIBRARY_VERSION

ConsoleTable.new(%w(Serial\ No. Type Class Name Version)).output do
  PhidgetsNative.all.enum_for(:each_with_index).collect{ |p,i|
    %w(serial_number type device_class name version).collect{|attr| p.send attr}
  }
end
