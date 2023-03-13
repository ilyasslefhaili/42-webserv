#!/usr/bin/perl

use strict;
use warnings;
use CGI qw(:standard);

# Get the Perl script from the query string
my $script = param('script');

# Execute the script using eval and capture its output
my $output;


# Print the output as HTML
print header();
print start_html(-title => 'Script Output');
print h1('Script Output');
print pre($output);
print end_html();