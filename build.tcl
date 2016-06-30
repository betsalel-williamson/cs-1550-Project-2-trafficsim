#! /usr/bin/env tclsh8.5

source common.tcl

if {[file isdirectory $BUILD_DIR]} {
  # this is a directory
} else {
  # not a directory
  exec mkdir $BUILD_DIR
}

cd $BUILD_DIR

set result [exec cmake ..]

set chan [open build.log a]
set timestamp [clock format [clock seconds]]
puts $chan "$timestamp\n"

puts $chan "$result"

set result [exec cmake --build .]
puts $chan "$result"
puts $chan "\n"

close $chan

