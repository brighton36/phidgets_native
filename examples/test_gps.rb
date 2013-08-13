#!/usr/bin/env ruby
# encoding: UTF-8

$:.push *['%s/../lib/', '%s/lib/'].collect{|p| p % File.dirname(__FILE__) }
require 'console_table'
require 'phidgets_native'

puts "Using Library version: "+Phidgets::LIBRARY_VERSION

gps = Phidgets::GPS.new(284771)
gps.wait_for_attachment 10000

trap("SIGINT") do
  puts "Sigint!"
  gps.close
  exit
end

# Print the basic attributes:
puts "\nDevice Attributes:"
ConsoleTable.new(%w(Attribute Value)).output do
  [ ["Type", gps.type.inspect],
    ["Name", gps.name.inspect],
    ["Label", gps.label.inspect],
    ["Serial", gps.serial_number.inspect],
    ["Version", gps.version.inspect],
    ["Device class", gps.device_class.inspect],
    ["Device id", gps.device_id.inspect] ]
end

# And this updates every 5 seconds:
gps_attribs = ['Sample Rate', 'Is Fixed', '%-18s' % 'Latitude', '%-18s' % 'Longitude', '%-18s' % 'Altitude', 
  'Heading', '%-18s' % 'Velocity', '%-23s' % 'Time (UTC)']
i = 0

puts "\nPolled Values:"
ConsoleTable.new(gps_attribs).output(:header => (i == 0), :separator => false) do |columns|
  i+=1
  [ columns[0...-1].collect{|attr| gps.send(attr.strip.tr(' ','_').downcase.to_sym).inspect }+
    [(gps.now_at_utc) ? gps.now_at_utc.strftime("%Y-%m-%d %H:%M:%S.%3N") : nil] ]
end while sleep(3)

gps.close
