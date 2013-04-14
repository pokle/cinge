cinge
=====

Extracts and transforms LDIF files for loading into Elasticsearch.

Benchmarks
==================

To count the number of newlines in an LDIF file (to understand the scale):

    12seconds to process a 4gb file on a 2.3Ghz Intel Core i7, 16gb, SSD
    40seconds to process a 4gb file on a 1.8Ghz Intel Core i7, 4gb, SSD

To extract the cn's from a 4gb file (already cached by the OS in its FS cache). This was on the 2.3Ghz Intel Core i7 with 16gb and SSD

	6.5 seconds by cinge
	3mins, 11 seconds by fgrep!!!!