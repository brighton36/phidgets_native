# File: Rakefile

require 'rake/extensiontask'
require 'rdoc/task'

Rake::ExtensionTask.new('phidgets_native')


RDOC_FILES = FileList["README.rdoc", "ext/phidgets_native/phidgets_native.c"]

Rake::RDocTask.new do |rd|
  rd.main = "README.rdoc"
  rd.rdoc_dir = "doc/site/api"
  rd.rdoc_files.include(RDOC_FILES)
end

Rake::RDocTask.new(:ri) do |rd|
  rd.main = "README.rdoc"
  rd.rdoc_dir = "doc/ri"
  rd.options << "--ri-system"
  rd.rdoc_files.include(RDOC_FILES)
end

