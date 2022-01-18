#!/usr/bin/perl

use strict;
use File::Temp qw/ tempfile tempdir /;
use File::Basename;
use File::Copy;
use File::Spec;
use Cwd;
use FindBin qw($Bin);
use Getopt::Std;
use Data::Dumper;

# usage example
# 1) go to student mg dir
# 2) create an output dir kk
# then
# $ ~/grafikoak/mg++/test/do_test.pl kk

my %opts;
getopts("pvo:g:",\%opts);

my $opt_v = $opts{'v'};

&usage("Usage $0 outDir") unless @ARGV == 1;

my $mg_dir = getcwd();
&usage("Can't open $mg_dir") unless -d $mg_dir;

my $test_dir = "$Bin/Math";
&usage("Can't open $test_dir") unless -d $test_dir;

my $gs_dir = "$Bin/gs";
$gs_dir = $opts{'g'} if defined $opts{'g'};
&usage("Can't open $gs_dir") unless -d $gs_dir;

my $output_dir = shift @ARGV;
#my $output_dir = defined $opts{'o'} ? $opts{'o'} : File::Temp->newdir( CLEANUP => 1 );
&usage("Can't open $output_dir") unless -d $output_dir;

&do_test_student($mg_dir, $test_dir, $output_dir);

# read outputs

my %s;  # { file -> { funcName => { gs => output, st => output } } }
&read_outputs($gs_dir, "gs", \%s);
&read_outputs($output_dir, "st", \%s);

my $file_ok = 0;
my $file_N = 0;
my @file_totals;
foreach my $file (sort keys %s) {
  my $h = $s{$file};
  $file_N++;
  my $func_ok = 0;
  my $func_N = 0;
  my @str_func;
  foreach my $func (sort keys %{ $h } ) {
	my $hh = $h->{$func};
	$func_N++;
	if ($hh->{gs} eq $hh->{st}) {
	  push @str_func, sprintf ("\t%s ... PASS", $func);
	  $func_ok++;
	} else {
	  push @str_func, sprintf ("\t%s ... FAIL", $func);
	}
  }
  if (!$func_N) {
	print STDERR "Error in file $file: no functions!!\n";
	next;
  }
  my $factor = sprintf("%.2f", ($func_ok / $func_N) * 100);
  printf ("File: %s: %s (%d / %d)\n", $file, $factor, $func_ok, $func_N);
  print join("\n", @str_func)."\n";
  push @file_totals, $factor;
}
my $ss;
foreach (@file_totals) { $ss += $_; }

printf ("Total: %.2f%%\n", $ss / $file_N);


sub read_outputs {

  my ($dir, $key, $h) = @_;

  my @names;
  opendir(my $D, $dir) or die $!;
  foreach my $file (grep {/\.txt$/} readdir($D)) {
	push @names, $file;
  }
  foreach my $name (@names) {
	&read_output("$dir/$name", $name, $key, $h);
  }
}

sub read_output {

  my ($fname, $file, $key, $h) = @_;

  open(my $fh, $fname) or die "Can't open $fname:$!\n";
  while(<$fh>) {
	last if /^--\*\*--/;
  }
  while(1) {
	my ($f, $o) = &next_record($fh);
	last unless $f;
	$h->{$file}->{$f}->{$key} = $o;
  }
}

sub next_record {

  my $fh = shift;

  my $out = "";

  my $f = <$fh>;
  return undef unless $f;
  chomp($f);
  while(<$fh>) {
	last if /^--\*\*--/;
	$out .= $_;
  }
  return ($f, $out);
}

sub do_test_student {

  my ($mg_dir, $test_dir, $output_dir) = @_;

  my $exec = "$Bin/test_create_output.pl";
  $exec .= " -v" if $opts{'v'};
  $exec .= " -p" if $opts{'p'};
  my $cmd = "$exec $mg_dir $test_dir $output_dir";
  eval {
	system($cmd);
  };
  if ($?) {
	die "Error executing \"$cmd\": $!\n";
  }
}


sub usage {

  my $str = shift;
  my $script = basename($0);

  print STDERR "ERROR: $str\n" if $str;
  die <<DEOF
 Usage: $script [-v] [-g gs_dir] [-o output_dir] student_mg_dir test_dir
        -v be verbose (and do not remove temp dir)
        -g gold_standard directory
        -o output directory
DEOF
}
