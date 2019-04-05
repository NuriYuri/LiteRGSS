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
end