#!/usr/bin/env ruby
# encoding: UTF-8

require '%s/lib/common' % File.dirname(__FILE__)

additional_attribs = [
  %w(Motor\ Count motor_count),
  %w(Acceleration\ Max acceleration_max),
  %w(Acceleration\ Min acceleration_min),
  %w(Velocity\ Max velocity_max),
  %w(Velocity\ Min velocity_min)
]

phidgets_example_for(PhidgetsNative::AdvancedServo, additional_attribs) do |servo|

  puts "Servo-specific Attributes:"
  ConsoleTable.new([
    'Acceleration', 'Velocity Limit', 'Position Max', 'Position Min', 
    'Speed_Ramping?', 'Engaged?', 'Stopped?', 'Servo Type' ]).output do 
    (0...servo.motor_count).collect{|i| [
      servo.acceleration(i),servo.velocity_limit(i), servo.position_max(i),
      servo.position_min(i), servo.speed_ramping?(i), servo.engaged?(i),
      servo.stopped?(i), servo.servo_type(i)
    ] } 
  end

  puts "\nPolled Values:"
  i = 0
  ConsoleTable.new([
    'Sample Rates',
    'Currents',
    'Positions',
    'Velocities'
  ]).output(:header => (i == 0), :separator => false) do |columns|
    i+=1

    [ [servo.sample_rates, servo.currents, servo.positions, servo.velocities].collect(&:inspect) ]
  end while sleep(3)
end
