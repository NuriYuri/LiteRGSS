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

Rake::ExtensionTask.new(ext_name, spec)
Rake::ExtensionTask.new(test_name, spec)
