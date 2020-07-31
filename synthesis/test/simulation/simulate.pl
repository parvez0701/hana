#!/usr/bin/perl 

use Getopt::Std;
use Verilog::Netlist;
######Globar variables##########################
my $rtlfile;
my $netlistfile;
my $tbfile;
my $gentb;
my $simau;
my $simtest;
my $exe = "$ENV['HANA_HOME']/bin/synthsh";
my $vopts;
my $debug;
my $totalinputs;
######End global variables #####################

Init();
GenerateTestbench();

sub Init {
    getopts("r:n:b:dgha:t:", \%opts) or PrintHelp();
    PrintHelp() if $opts{h};
	$rtlfile = $opts{r};
	$netlistfile = $opts{n};
	$tbfile = $opts{b};
	$gentb = $opts{g};
	$simau = $opts{a};
	$simtest = $opts{t};
	$debug = $opts{d}
}	



sub PrintHelp {
    print "    simulate : options\n";
	print "    -r <rtl_file>\n";
	print "    -n <netlist_file>\n";
	print "    -b <testbench_file>\n";
	print "    -g generate testbench \n";
	print "    -a <simulation_au_file>\n";
	print "    -t <simulation_test_out_file>\n";
	print "    -d debug\n";
}	

sub GenerateTestbench {
    use Verilog::Getopt;
	return if !defined($gentb);

	die "Verilog testcase not specified" if !defined($rtlfile);
	my $opt = new Verilog::Getopt;
    $opt->parameter( "verilog");
	my $nl = new Verilog::Netlist (options => $opt,);
	$nl->read_file(filename=>$rtlfile);
	$nl->link();
	foreach my $top ($nl->top_modules_sorted) {
	    WriteTestbench($top);
	}	
}

sub WriteTestbench {
    my $module = shift;
	my @ports;

	foreach my $port ($module->ports_sorted) {
	   next if !($port->direction =~ /in/);
	    $ports[++$#ports] = $port;
	}
	#debug info
	if (defined $debug)
	{
	    foreach my $port (@ports) {
		    print "port ", $port->name;
		    print "\n";
	    }	
	}	
	open ($fh, ">$tbfile") 
	    or die "Can not open file $tbfile for writing";
	WriteModuleHeader($fh, $module);	
	my @portcount = WriteRegisters($fh, $module);
	WriteDUTInstance($fh, $module);
	WriteVectors($fh);
	WriteModuleFooter($fh);
	close $fh;
}	

sub WriteModuleHeader {
	my $fh = shift;
    my $module = shift;

	print $fh "module ";
	print $fh $module->name;
	print $fh "_tb();\n";
}

sub WriteModuleFooter {
	my $fh = shift;
	print $fh "endmodule";
}

sub WriteRegisters {
    my $fh = shift;
	my $module = shift;

	my @inports;
	my @outports;
	my @inoutports;

	my $incount = 0;
	my $outcount = 0;
	my $inoutcount = 0;

	foreach my $port ($module->ports_sorted) {
	    if ($port->direction =~ /inout/) 
		{
	        $inoutports[++$#inoutports] = $port;
			if(defined ($port->net->msb)) {
			    $inoutcount = 
				    $inoutcount + abs($port->net->msb - $port->net->lsb) + 1;
			}
			else {
			    ++$inoutcount;
			}	
	    }	
		elsif($port->direction =~ /out/) 
		{
		    $outports[++$#outports] = $port;
			if(defined ($port->net->msb)) {
			    $outcount = 
				    $outcount + abs($port->net->msb - $port->net->lsb) + 1;
			}
			else {
			    ++$outcount;
			}	
		}	
		elsif($port->direction =~ /in/) 
		{
		    $inports[++$#inports] = $port;
			if(defined ($port->net->msb)) {
			    $incount = 
				    $incount + abs($port->net->msb - $port->net->lsb) + 1;
			}
			else {
			    ++$incount;
			}	
		}	
	}
	if($incount) {--$incount; }
	if($outcount) {--$outcount; }
	if($inoutcount) { --$inoutcount;} 
	print $fh "reg [$incount:0] regin;\n";
	print $fh "wire [$outcount:0] regout;\n";
	print $fh "reg [$inoutcount:0] reginout;\n";
	print $fh "integer file;\n";
	$totalinputs = $incount + $inoutcount;
	return {$incount, $outcount, $inoutcount};
}

sub WriteDUTInstance {
    my $fh = shift;
	my $module = shift;
	my $incount = 0;
	my $outcount = 0;
	my $incount = 0;

	print $fh $module->name;
	print $fh " dut(";

	my $first = 1;
	foreach my $port ($module->ports_sorted) {
	    if($first) {
		    $first = 0;
		}	
		else {
		    print $fh ", ";
		}
		print $fh ".", $port->name, "(";
	    if ($port->direction =~ /inout/) {
			if(defined ($port->net->msb)) {
			    print $fh "{";
				my $first = 1;
			    my $msb = $port->net->msb;
				my $lsb = $port->net->lsb;
			    for(my $i = $lsb; ($msb > $lsb) ? $i <= $msb : $i >= lsb;
				    ($msb>$lsb) ? ++$i : --$i) {
					if($first) {
					    $first = 0;
					}
					else {
					    print $fh ", ";
					}
					print $fh "reginout[", $inoutcount++, "]";
				}
				print $fh "}";
			}
			else {
				print $fh "reginout[", $inoutcount++, "]";
			}
		}	
	    elsif ($port->direction =~ /out/) {
			if(defined ($port->net->msb)) {
			    print $fh "{";
				my $first = 1;
			    my $msb = $port->net->msb;
				my $lsb = $port->net->lsb;
			    for(my $i = $lsb; ($msb > $lsb) ? $i <= $msb : $i >= lsb;
				    ($msb>$lsb) ? ++$i : --$i) {
					if($first) {
					    $first = 0;
					}
					else {
					    print $fh ", ";
					}
					print $fh "regout[", $outcount++, "]";
				}
				print $fh "}";
			}
			else {
				print $fh "regout[", $outcount++, "]";
			}
		}	
	    elsif ($port->direction =~ /in/) {
			if(defined ($port->net->msb)) {
			    print $fh "{";
				my $first = 1;
			    my $msb = $port->net->msb;
				my $lsb = $port->net->lsb;
			    for(my $i = $lsb; ($msb > $lsb) ? $i <= $msb : $i >= lsb;
				    ($msb>$lsb) ? ++$i : --$i) {
					if($first) {
					    $first = 0;
					}
					else {
					    print $fh ", ";
					}
					print $fh "regin[", $incount++, "]";
				}
				print $fh "}";
			}
			else {
				print $fh "regin[", $incount++, "]";
			}
		}	

		print $fh ")";
	}	
	print $fh ");\n";
}

sub WriteVectors {
    my $fh = shift;

	print $fh "initial\n begin \n";
	print $fh "    file = \$fopen(\"$simtest\",  \"w\");\n\n";
	my $count = (2 ** ($totalinputs+1)) > 16384 ? 16384 : (2 ** ($totalinputs+1));

	print $fh "    \$fdisplay(file, \"input         output\");\n";
	print $fh "    \$fdisplay(file, \"-------------------------\");\n";
	for(my $i = 0; $i < $count; ++$i) {
	    print $fh "    {reginout, regin} = ", $i, ";\n";
		print $fh "    #10;\n";
		print $fh "    \$fdisplay(file, \"%b              %b\", regin, regout);\n";
		#print $fh "    \$fdisplay(file, \"output: %b\", regout);\n";
	}	
	print $fh "\n    \$fclose(file);\n";
	print $fh "end\n";
}

