data = nil
(Dir["doc/*.html"] + Dir["doc/*/*.html"]).each do |fn|
  puts fn
  File.open(fn, "rb") do |f| data = f.read(f.size) end
  #test_reg = /(?<=<pre class="code">).*?(?<=<\/pre>)/m
  test_reg = /(?=<table class="source_code">).*?(?<=<\/table>)/m
  data.gsub!(test_reg, "")
  data.gsub!(/[\r\n][ \t]+/, "\n")
  data.gsub!(/[\r\n][\r\n]+/, "\n")
  File.open(fn, "wb") do |f| f << data end
end
