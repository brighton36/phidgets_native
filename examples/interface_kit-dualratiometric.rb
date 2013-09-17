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

  ifkit.ratiometric = true
  ifkit.ratiometric 6, false
  ifkit.ratiometric 7, false

  # Test sensor_raw
  puts "Ratiometric states:"
  ConsoleTable.new((0...ifkit.sensor_count).collect{|i| "Sensor #{i}"}).output do 
    [(0...ifkit.sensor_count).collect{|i| ifkit.ratiometric(i).inspect } ]
  end

  puts "\nPolled Values:"
  i = 0
  ConsoleTable.new([
    'Sensor Sample Rates',
    '%-40s' % 'sensors', 
  ]).output(:header => (i == 0), :separator => false) do |columns|
    i+=1
    
    values = ifkit.sensors
    if values
      values[0] = '%.1f %% (RH)' % [values[0].to_f * 0.1906 - 40.2]
      values[1] = '%.1f deg (C)' % [values[1].to_f * 0.22222 - 61.111]
      values[2] = '%.1f deg (C)' % [values[2].to_f * 0.22222 - 61.111]

      values[6] = '%.3f (V)' % [(values[6].to_f / 200 - 2.5) / 0.0681]
      values[7] = '%.3f (V)' % [(values[7].to_f / 200 - 2.5) / 0.0681]
    end

    [ [ ifkit.sensor_sample_rates, values.inspect] ]
  end while sleep(1)
end
