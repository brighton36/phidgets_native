#!/usr/bin/env ruby
# encoding: UTF-8

require '%s/lib/common' % File.dirname(__FILE__)

additional_attribs = [
  %w(Digital\ Inputs input_count),
  %w(Digital\ Outputs output_count),
  %w(Analog\ Inputs sensor_count),
  %w(Change\ Triggers change_triggers),
  %w(Data\ Rates data_rates),
  %w(Data\ Rates\ Min data_rates_min),
  %w(Data\ Rates\ Max data_rates_max)
]
phidgets_example_for(PhidgetsNative::InterfaceKit, additional_attribs) do |ifkit|
  puts "TODO: Test all the setters" 
  puts "\nPolled Values:"

  i = 0
  ConsoleTable.new([
    'Sample Rate',
    'Ratiometric?',
    '%-40s' % 'sensors', 
    '%-20s' % 'inputs',
    '%-20s' % 'outputs'
  ]).output(:header => (i == 0), :separator => false) do |columns|
    i+=1

    #TODO Test the ratiometric set at some point

    [ ['%d Hz' % ifkit.sample_rate, ifkit.ratiometric?.inspect, 
      ifkit.sensors.inspect]+[ifkit.inputs, ifkit.outputs].collect{ |bools|
        (bools.kind_of? Array) ? 
          '[%s]' % bools.collect{|b| (b) ? '1' : '0'  }.join(', ') : bools.inspect}
    ]
  end while sleep(3)
end
