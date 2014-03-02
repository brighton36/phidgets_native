# File: Rakefile

require 'rake'
require 'rake/extensiontask'
require 'rake/packagetask'
require 'rdoc/task'
require 'rspec/core/rake_task'

spec = Gem::Specification.load('phidgets_native.gemspec')
Rake::ExtensionTask.new 'phidgets_native', spec
Gem::PackageTask.new spec

RDOC_FILES = FileList["README.rdoc", "ext/phidgets_native/phidgets_native_ruby.c", 
  "ext/phidgets_native/*_ruby.c"]

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

RSpec::Core::RakeTask.new('spec')

task :spec => :compile

