use strict;

my @backtrace;
while(<>) {
    if(/Trace .* \[([0-9a-f]*)] (.*)/) {
	my $addr = $1;
	my $fname = $2;
	if($backtrace[$#backtrace] ne $fname) {
	    if($backtrace[$#backtrace - 1] eq $fname) {
		pop @backtrace;
	    } else {
		push @backtrace, $fname;
	    }
	}
	print "Trace [$addr] f";
	foreach my $fn (@backtrace) {
	    print ".$fn";
	}
	print "\n";
    } else {
	print;
    }
}
