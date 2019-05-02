ext_name = 'LiteRGSS'
test_name = ext_name + '_test'
require 'rake/extensiontask'

spec = Gem::Specification.new do |s|
  s.name = ext_name
  s.platform = Gem::Platform::RUBY
  s.version = '0.1.3'
  s.summary = "LiteRGSS library, make Game with ruby easily"
  s.description = "LiteRGSS is a Game library that allows you to easly make Games with Ruby. LiteRGSS uses SFML to get Inputs and to display Graphics."
  s.authors = ["Nuri Yuri"]
  s.homepage = "https://psdk.pokemonworkshop.fr/litergss"
  s.extensions = FileList["ext/LiteRGSS/extconf.rb"]
  s.files = FileList["ext/LiteRGSS/*.h", "ext/LiteRGSS/*.cpp", "ext/LiteRGSS/*.hpp", "ext/LiteRGSS/*.c"]
  s.metadata    = { "source_code_uri" => "https://github.com/NuriYuri/LiteRGSS", "documentation_uri" => "https://psdk.pokemonworkshop.fr/litergss" }
  s.required_ruby_version = '>= 2.5.0'
end

Gem::PackageTask.new(spec) do |pkg|
end

namespace :test do
  # partial-loads-ok and undef-value-errors necessary to ignore
  # spurious (and eminently ignorable) warnings from the ruby
  # interpreter
  VALGRIND_BASIC_OPTS = "--num-callers=50 --error-limit=no \
                          --partial-loads-ok=yes --undef-value-errors=no"
  COMMAND = ENV['cmd']
  desc "run #{COMMAND} under valgrind with basic ruby options"
  task :valgrind do
      cmdline = "valgrind #{VALGRIND_BASIC_OPTS} ruby #{COMMAND}"
      puts cmdline
      system cmdline
  end
end

namespace :test do
  desc "run #{COMMAND} under gdb"
  task :gdb do
      system "gdb --args ruby #{COMMAND}"
  end

  desc "clean the unit-test build folder"
  task :clean do
      system "rm -r tmp/i386-mingw32/LiteRGSS_test/*"
  end

  desc "start unit-test"
  task :unit do
      system "rspec unit-test/*.rb"
  end
end

Rake::ExtensionTask.new(ext_name, spec)
Rake::ExtensionTask.new(test_name, spec)
