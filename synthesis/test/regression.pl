#!/usr/bin/perl
#   ###################################################################
#	Master script for running the command and diffing files.
#	Author: Parvez Ahmad
#	All rights reserved.
#	###################################################################
use Cwd;
my $ref_file;
my $cmd;
my $options;
my @testfiles;
my @goldfiles;
my $line = 0;
my $dir = cwd;

#print "++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n";
print "$dir\n";
#print "----------------------------------------------------------------\n";

ReadTestFile();

if(scalar(@goldfiles) != scalar(@testfiles)){
	die "Number of gold files and test files don't match\n";
}
my $opt1 = "update";
my $opt2 = "commit";
if($ARGV[0] =~ m/$opt1/){
	Update();
}
elsif($ARGV[0] =~ m/$opt2/){
	Commit();
}
else{
	RunCmd($cmd, $options);
	CmpDiff();
	print "----------------------------------------------------------------\n";
}



sub ReadTestFile {

	open($ref_file, "<refs") || 
		die "Can't open reference file for reading.\n";

	my @content = <$ref_file>;
	my $size = scalar(@content);
	for (my $i = 0; $i < $size; ++$i){
		if($content[$i] =~ m/#/){
			#$i = skiptilleol($i, ($content[$i]));
			++$line;
			next;
		}
		my @entry = split /=/, ($content[$i]);
		if($entry[0] eq "CMD"){
			if(!defined($entry[1])) {
				die "Line $line: CMD not specified correctly.\n";
			}
			else{
				chomp $entry[1];
				$cmd = $entry[1];
			}
		}
		elsif($entry[0] eq "OPTIONS") {
			if(!defined($entry[1])) {
				die "Line $line: OPTIONS not specified correctly.\n";
			}
			else {
				chomp $entry[1];
				$options = $entry[1];
			}
		}
		elsif($entry[0] eq "FILES"){
			if(!defined($entry[1])) {
				die "Line $line: FILES not specified correctly.\n";
			}
			else {
				chomp $entry[1];
				my @files = split "[ \t]+", $entry[1];
				push @goldfiles, $files[0];
				push @testfiles, $files[1];
			}
		}
	}
}
		

sub RunCmd {
	my $cmd = shift;
	my $options = shift;
	#my $run_cmd = join "", ($cmd, $options);
	my $run_cmd = "$ENV{'HANA_HOME'}/bin/$cmd $options";
	system $run_cmd;
} 
	
sub skiptilleol {
	my $line = shift;
	my @content = shift;
	my @content = split "", @content;
	foreach my $entry (@content) {
		++$line;
		last if($entry == "\n");
	}

	return $line;
}

sub CmpDiff {
	my $size = scalar(@goldfiles);
	for(my $i = 0; $i < $size; ++$i){
		my $diff_cmd = "diff $goldfiles[$i] $testfiles[$i] > /dev/null";
		if(system($diff_cmd) == 0){
			print "PASSED: $testfiles[$i]\n";
		}
		else {
			print "FAILED: $testfiles[$i]\n";
		    $diff_cmd = "diff $goldfiles[$i] $testfiles[$i] > $testfiles[$i].diff";
			system($diff_cmd);
		}
	}
}

sub Update {
	my $size = scalar(@goldfiles);	
	for(my $i = 0; $i < $size; ++$i){
		my $cmd = "mv $testfiles[$i] $goldfiles[$i]";
		print "Copying $testfiles[$i] to $goldfiles[$i]\n";
		system($cmd);
	}
}

sub Commit {
	my $size = scalar(@goldfiles);	
	for(my $i = 0; $i < $size; ++$i){
		my $cmd = "cvs commit -m \" Checking in new GOLD file \" $goldfiles[$i]";
		system($cmd);
	}
	my $cmd = "cvs commit -m \" Checking in new refs file \" refs";
	system($cmd);
}
