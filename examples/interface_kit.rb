#!/usr/bin/env ruby
# encoding: UTF-8

require '%s/lib/common' % File.dirname(__FILE__)

#PhidgetsNative.enable_logging! :verbose

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
  # Here's how you set digital outputs:
  (0...ifkit.output_count).each{ |i| ifkit.output i, i.even? }

  # Let's adjust the data rates on each input a bit:
  (0...ifkit.sensor_count).each do |i| 
    case (i % 4)
      when 0
      when 1
        ifkit.data_rate i, ifkit.data_rates_max[i]
      when 2
        ifkit.change_trigger i, 1
      when 3
        ifkit.data_rate i, ifkit.data_rates_min[i]
    end
  end

  # Test sensor_raw
  puts "Raw Analog sensor values:"
  ConsoleTable.new((0...ifkit.sensor_count).collect{|i| "Sensor #{i}"}).output do 
    [(0...ifkit.sensor_count).collect{|i| ifkit.sensor_raw(i).inspect } ]
  end

  puts "\nPolled Values:"
  i = 0
  ConsoleTable.new([
    'Input Sample Rates',
    'Sensor Sample Rates',
    'Ratiometric?',
    '%-40s' % 'sensors', 
    '%-20s' % 'inputs',
    '%-20s' % 'outputs'
  ]).output(:header => (i == 0), :separator => false) do |columns|
    i+=1

    #TODO Test the ratiometric set at some point

    [ [ifkit.input_sample_rates, ifkit.sensor_sample_rates, ifkit.ratiometric?.inspect, 
      ifkit.sensors.inspect]+[ifkit.inputs, ifkit.outputs].collect{ |bools|
        (bools.kind_of? Array) ? 
          '[%s]' % bools.collect{|b| (b) ? '1' : '0'  }.join(', ') : bools.inspect}
    ]
  end while sleep(3)
end
