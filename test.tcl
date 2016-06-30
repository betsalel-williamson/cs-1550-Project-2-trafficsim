#! /usr/bin/env tclsh8.5

source common.tcl

if {[file isdirectory $TEST_EXE]} {
  # this is a directory
} else {
  # not a directory
  exec $BUILD_SCRIPT_FILE
}

set chan [open test.log a]
set timestamp [clock format [clock seconds]]
puts $chan "$timestamp\n"

if { [catch {set result [exec $TEST_EXE]} reason] } {

puts $chan "Failed: $reason"

} else {

puts $chan "$result"

}

puts $chan "\n\n"

close $chan

proc searchResultsForPassed {result} {
    if {[lsearch -all -inline $result *PASSED*] >= 0} {
        return -code ok
     } else { 
        return -code error
    }
}



