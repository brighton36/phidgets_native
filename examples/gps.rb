#!/usr/bin/env ruby
# encoding: UTF-8

require '%s/lib/common' % File.dirname(__FILE__)

phidgets_example_for(PhidgetsNative::GPS.new(284771)) do
  puts "\nPolled Values:"

  gps_attribs = ['Sample Rate', 'Is Fixed', '%-18s' % 'Latitude', '%-18s' % 'Longitude', '%-18s' % 'Altitude', 
    'Heading', '%-18s' % 'Velocity', '%-23s' % 'Time (UTC)']
  i = 0

  ConsoleTable.new(gps_attribs).output(:header => (i == 0), :separator => false) do |columns|
    i+=1
    [ [ gps.sample_rate, gps.is_fixed?, gps.latitude, gps.longitude, gps.altitude,
      gps.heading, gps.velocity].collect(&:inspect)+[
      (gps.now_at_utc) ? gps.now_at_utc.strftime("%Y-%m-%d %H:%M:%S.%3N") : nil.inspect ] ]
  end while sleep(3)
end
