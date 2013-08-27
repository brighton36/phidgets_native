Gem::Specification.new do |s|
  s.name    = "phidgets_native"
  s.version = "0.2.0"
  s.summary = "Native C-extension gem for the Phidgets library in ruby"
  s.author  = "Chris DeRose"
  s.description = s.summary
  s.author = "Chris DeRose, DeRose Technologies, Inc."
  s.email = 'cderose@derosetechnologies.com'
  s.homepage = 'http://www.derosetechnologies.com/community/phidgets_native'
  
  s.files = Dir.glob("ext/**/*.{c,h,rb}") +
            Dir.glob("examples/*.rb") +
            Dir.glob("examples/lib/*.rb") +
            Dir.glob("lib/**/*.rb")+
            %w(Rakefile README.rdoc Gemfile Gemfile.lock phidgets_native.gemspec)
  
  s.extensions << "ext/phidgets_native/extconf.rb"
  
  s.add_development_dependency "rake-compiler"
end

