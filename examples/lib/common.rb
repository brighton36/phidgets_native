# encoding: UTF-8

$:.push *['%s/../../lib/', '%s'].collect{|p| p % File.dirname(__FILE__) }

require 'console_table'
require 'phidgets_native'

# To keep things DRY, I went ahead and put this here:
def phidgets_example_for(device, &block)
  trap("SIGINT"){ device.close; exit }

  device.wait_for_attachment 10000

  puts "Using Library version: "+PhidgetsNative::LIBRARY_VERSION

  puts "\nDevice Attributes:"

  ConsoleTable.new(%w(Attribute Value)).output do
    [ ["Type", device.type.inspect],
      ["Name", device.name.inspect],
      ["Label", device.label.inspect],
      ["Serial", device.serial_number.inspect],
      ["Version", device.version.inspect],
      ["Device class", device.device_class.inspect],
      ["Device id", device.device_id.inspect] ]
  end

  block.call(device)
end
