system('rake compile')
File.copy_stream('lib/LiteRGSS.so', '../pokemonsdk/lib/LiteRGSS.so')