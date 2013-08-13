# encoding: UTF-8

# Just a quickie hack-class I made to make the test scripts easier to understand
# and DRY
class ConsoleTable
  ERR_ROW_LENGTH = "All Rows must have the same number of columns"

  def initialize(column_labels)
    @columns = column_labels
  end

  def output(options = {}, &block)
    rows = block.call(@columns)
    if rows.length > 0
      raise StandardError, ERR_ROW_LENGTH if rows.any?{|r| r.length != @columns.length}
  
      # Find the widest cell widths:
      widths = ([@columns]+rows).inject(Array.new(@columns.length,0)){|ret, row| 
        (0...row.length).collect{|i| (ret[i] >= row[i].to_s.length) ? ret[i] : row[i].to_s.length }
      }

      # Construct the separator and end_cap rows:
      end_cap, separator = %w(+%s+ |%s|).collect{|fmt| 
        fmt % widths.collect{|width| "-" * (width+4) }.join('|') 
      }

      row_fmt = '|- %s -|' % widths.collect{|width| "%-#{width}s"}.join(' -|- ')

      puts [
        (options[:header] == false) ? nil : [ end_cap, row_fmt % @columns, separator],
        rows.to_enum(:each_with_index).collect{ |row,i | 
          [ (options[:separator] == false || i == 0) ? nil : separator, row_fmt % row ] }, 
        (options[:footer] == false) ? nil : end_cap
      ].flatten.compact.join("\n")
    end
  end
end

