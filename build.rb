system('rake compile')
File.copy_stream('lib/LiteRGSS.so', '../PSDK/lib/LiteRGSS.so')