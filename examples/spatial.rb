#!/usr/bin/env ruby
# encoding: UTF-8

require '%s/lib/common' % File.dirname(__FILE__)

additional_attribs = [
  %w(Compass\ Correction compass_correction),
  %w(Data\ Rate data_rate),
  %w(Data\ Rate\ Max data_rate_max),
  %w(Data\ Rate\ Min data_rate_min),
  %w(Accelerometer\ Axes accelerometer_axes),
  %w(Accelerometer\ Min accelerometer_min),
  %w(Accelerometer\ Max accelerometer_max),
  %w(Gyroscope\ Axes gyro_axes),
  %w(Gyroscope\ Min gyro_min),
  %w(Gyroscope\ Max gyro_max),
  %w(Compass\ Axes compass_axes),
  %w(Compass\ Min compass_min),
  %w(Compass\ Max compass_max)
]

phidgets_example_for( PhidgetsNative::Spatial, additional_attribs ) do |spatial|
  # This is worth doing before you start to use a gyro: 
  spatial.zero_gyro!
 
  # This is worth doing as well: 
  spatial.compass_correction = [ 0.441604, 0.045493, 0.176548, 0.002767, 1.994358, 
    2.075937, 2.723117, -0.019360, -0.008005, -0.020036, 0.007017, -0.010891, 0.009283 ]

  # This is almost never worth doing. I just put it in here for demonstration
  # and test purposes
  spatial.reset_compass_correction!

  # Probably you'll never need to do this either:
  spatial.data_rate = 16

  puts "\nPolled Values:"

  i = 0
  ConsoleTable.new([
    '%-40s' % 'Accelerometer', 
    '%-40s' % 'Gyroscope',
    '%-40s' % 'Compass'
  ]).output(:header => (i == 0), :separator => false) do |columns|
    i+=1
    [ [ spatial.accelerometer, spatial.gyro, spatial.compass ].collect{ |axes|
      (axes.kind_of? Array) ? 
        '[%s]' % axes.collect{|a| '%0.6f' % a}.join(', ') : axes.inspect
    } ]
  end while sleep(3)
end
