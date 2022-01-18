#!/usr/bin/perl

use strict;
use File::Temp qw/ tempfile tempdir /;
use File::Basename;
use File::Copy;
use File::Spec;
use Cwd;
use Getopt::Std;

my %opts;
getopts("pv",\%opts);

my $opt_v = $opts{'v'} ? 1 : 0;

&usage("Too few parameters") unless @ARGV == 3;

my $mg_dir = shift @ARGV;
$mg_dir = File::Spec->rel2abs($mg_dir) unless File::Spec->file_name_is_absolute($mg_dir);
my $test_dir = shift @ARGV;
$test_dir = File::Spec->rel2abs($test_dir) unless File::Spec->file_name_is_absolute($test_dir);
my $output_dir = shift @ARGV;
$output_dir = File::Spec->rel2abs($output_dir) unless File::Spec->file_name_is_absolute($output_dir);

&usage("$mg_dir does not exist") unless -d $mg_dir;
&usage("$test_dir does not exist") unless -d $test_dir;
&usage("$output_dir does not exist") unless -d $output_dir;

my $exec_dir = File::Temp->newdir( CLEANUP => $opt_v == 0 );
my @test_src = &get_test_source($test_dir);

warn "Temp dir (no removed): $exec_dir\n" if $opt_v;
if (&compile_tests($exec_dir, $test_dir, $mg_dir, \@test_src)) {
  &exec_tests(\@test_src, $exec_dir, $output_dir);
}

sub get_test_source {

  my ($dir) = @_;

  my @ema;
  my $fname = "$dir/Makefile";
  open(my $fh, $fname) or die "Can't open $fname:$!\n";
  while (<$fh>) {
	chomp;
	next unless /^SOURCEMAIN\s*=\s*/;
	my $s = $';
	foreach my $f (split(/\s+/, $s)) {
	  push @ema, basename($f, '.cc');
	}
  }
  return @ema;
}

sub compile_tests {

  my ($exec_dir, $test_dir, $src_dir, $sources) = @_;

  my $ok = 1;

  # first, copy makefile and .cc files to exec_dir
  copy("$test_dir/Makefile", $exec_dir);
  foreach my $test_c (@{ $sources }) {
	copy("$test_dir/$test_c.cc", $exec_dir);
  }
  my $th_include = "$test_dir/.."; # for t.h
  # next, go to exec_dir and compile
  my $curdir = getcwd();
  chdir($exec_dir) or die "chdir $exec_dir: $!\n";
  my $cmd = "make TH_DIR=$th_include MG_DIR=$src_dir";
  $cmd .= "> /dev/null 2>&1" unless $opt_v;
  eval {
	system($cmd);
  };
  my $err = $?;
  chdir($curdir);
  if ($err) {
	$exec_dir->unlink_on_destroy(0) if $opts{'p'};
	warn "Error in compilation\ncmd: \"$cmd\"\ndir:$exec_dir\n";
	$ok = 0;
  }
  return $ok;
}

sub exec_tests {

  my ($tests, $exec, $odir) = @_;

  foreach my $name (@{ $tests }) {
	my $cmd = "$exec/$name $odir/$name.txt";
	#$cmd .= ">/dev/null 2>/dev/null" unless $opt_v;
	$cmd .= "> /dev/null 2>&1" unless $opt_v;
	print "$cmd\n" if $opt_v;
	eval {
	  system($cmd);
	};
	if ($?) {
	  $exec->unlink_on_destroy(0) if $opts{'p'};
	  warn "Error executing \"$cmd\"\n";
	}
  }
}

sub usage {

  my $str = shift;
  my $script = basename($0);

  print STDERR "ERROR: $str\n" if $str;
  die <<DEOF
 Usage: $script [-v] student_mg_dir test_dir output_dir
        -v be verbose (and do not remove temp dir)
DEOF
}
