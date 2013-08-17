# encoding: UTF-8

$:.push *['%s/../../lib/', '%s'].collect{|p| p % File.dirname(__FILE__) }

require 'console_table'
require 'phidgets_native'

# To keep things DRY, I went ahead and put this here:
def phidgets_example_for(device_klass, extended_attribs = [], &block)

  serial_number = $1.to_i if ARGV.length == 1 && /\A([\d]+)\Z/.match(ARGV[0])

  unless serial_number
    puts "Invalid or non-existant device serial number. This is a required parameter"
    puts
    puts "Usage: %s [device_serial]" % File.basename($PROGRAM_NAME)
    exit 1
  end

  device = device_klass.new serial_number

  trap("SIGINT"){ device.close; exit }

  begin
    device.wait_for_attachment 10000
  rescue PhidgetsNative::TimeoutError
    puts "Unable to enumerate the phidget of type %s with the serial number %d." % [
      device.class.to_s, device.serial_number]
    exit
  end

  puts "Using Library version: "+PhidgetsNative::LIBRARY_VERSION

  puts "\nDevice Attributes:"


  ConsoleTable.new(%w(Attribute Value)).output do
    [ ["Type", device.type.inspect],
      ["Name", device.name.inspect],
      ["Label", device.label.inspect],
      ["Serial", device.serial_number.inspect],
      ["Version", device.version.inspect],
      ["Device class", device.device_class.inspect],
      ["Device id", device.device_id.inspect] ]+extended_attribs.collect{|pairs|
        label, attr = pairs
        [label, device.send(attr.to_sym)]
      }
  end

  block.call(device)
end
