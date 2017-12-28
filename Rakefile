ext_name = 'LiteRGSS'
require 'rake/extensiontask'

spec = Gem::Specification.new do |s|
  s.name = ext_name
  s.platform = Gem::Platform::RUBY
  s.version = '0.1.1'
  s.summary = "LiteRGSS library, make Game with ruby easily"
  s.description = "LiteRGSS is a Game library that allows you to easly make Games with Ruby. LiteRGSS uses SFML to get Inputs and to display Graphics."
  s.authors = ["Nuri Yuri"]
  s.homepage = "https://psdk.pokemonworkshop.com/litergss"
  s.extensions = FileList["ext/LiteRGSS/extconf.rb"]
  s.files = FileList["ext/LiteRGSS/*.h", "ext/LiteRGSS/*.cpp", "ext/LiteRGSS/*.hpp"]
  s.metadata    = { "source_code_uri" => "https://github.com/NuriYuri/LiteRGSS" }
end

Gem::PackageTask.new(spec) do |pkg|
end

Rake::ExtensionTask.new(ext_name, spec)  
