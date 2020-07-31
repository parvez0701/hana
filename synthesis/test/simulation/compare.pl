#!/usr/bin/perl

print $ARGV[2];
print "\n====================================================\n";
if(system("diff $ARGV[0] $ARGV[1]") == 0 ) {
    print "PASSED\n";
}	
else {
    print "FAILED\n";
}	
