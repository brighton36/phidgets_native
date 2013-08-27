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
  servo.acceleration 0, servo.acceleration_max[0]
  servo.velocity_limit 0, servo.velocity_max[0]
  # NOTE that these doesn't seem to be supported on all servo types
  servo.position_max 0, 220 
  servo.position_min 0, 0 
  # /NOTE
  servo.servo_type 0, :default
  servo.speed_ramping 0, false
  servo.engaged 0, true

  servo_range = servo.position_max(0) - servo.position_min(0)
  servo_middle = servo.position_min(0) + servo_range/2
  servo_left = servo.position_min(0) + servo_middle - servo_range / 4
  servo_right = servo.position_min(0) + servo_middle + servo_range / 4

  servo.position 0, servo_middle

  puts "\nServo-specific Attributes:"
  ConsoleTable.new([
    'Servo Offset', 'Acceleration Max', 'Acceleration Min','Velocity Limit', 'Position Max', 'Position Min', 
    'Speed_Ramping?', 'Engaged?', 'Stopped?', 'Servo Type' ]).output do 
    (0...servo.motor_count).collect do |i| 
      ([i, servo.acceleration_max[i], servo.acceleration_min[i]]+%w( 
      velocity_limit position_max position_min speed_ramping? engaged? 
      stopped? servo_type).collect{|attr|
        begin
          servo.send(attr, i) 
        rescue PhidgetsNative::UnknownValError
          nil
        end
      }).collect(&:inspect)
    end 
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

    servo.position 0, (i % 2 == 0) ? servo_left : servo_right if (i != 0)
    [ [servo.sample_rates, servo.currents, servo.positions, servo.velocities].collect(&:inspect) ]
  end while sleep(3)
end
