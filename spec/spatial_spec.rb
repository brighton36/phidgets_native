require 'spec_helper'
 
describe PhidgetsNative::Spatial do
  def reference_direction_cosine_matrix(around_x, around_y, around_z, in_order = 'XYZ')
    rotations = {
      # X-rotation:
      :x => Matrix.rows([
        [1.0, 0, 0],
        [0, Math.cos(around_x), Math.sin(around_x) * (-1.0)],
        [0, Math.sin(around_x), Math.cos(around_x)]
      ]), 

      # Y-rotation:
      :y => Matrix.rows([
        [Math.cos(around_y), 0, Math.sin(around_y)],
        [0, 1.0, 0],
        [Math.sin(around_y) * (-1.0), 0, Math.cos(around_y)]
      ]), 

      # Z-rotation:
      :z => Matrix.rows([ 
        [Math.cos(around_z), Math.sin(around_z) * (-1.0), 0],
        [Math.sin(around_z), Math.cos(around_z), 0],
        [0, 0, 1.0]
      ]) 
    }

    in_order.chars.collect{ |axis| rotations[axis.downcase.to_sym] }.reduce(:*)
  end 

  it "direction_cosine_matrix should transform euler angles to dcms" do
    # This is kind of a crappy non-deterministic test, but, since I already had 
    # a well performing ruby equivalent, I'm leveraging this asset. 
    spatial = PhidgetsNative::Spatial.new 123456

    orders = ['xyz', 'xzy', 'yxz', 'yzx', 'zyx', 'zxy', nil]
    range = (-2.0*Math::PI)..(2.0*Math::PI)
    rotations = 10000.times.collect{
      ((0..2).collect{|i| rand range }+[orders[rand(orders.length)]]).flatten.compact
    }

    # This is a special case, might as well test it:
    rotations << [0,0,0]

    rotations.each do |rotation|
      spatial.instance_eval{ direction_cosine_matrix(*rotation) }.should eq(
        reference_direction_cosine_matrix(*rotation))
    end
  end
end
