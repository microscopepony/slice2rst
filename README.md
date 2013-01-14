slice2rst
=========

slice2rst is a simple Slice to restructured text transformer, based on
slice2xml (http://code.google.com/p/slice2xml/, GPL v3 license).
It is built using the [unsupported] Slice parser code in the Ice code base, and
has been tested with Ice 3.3.1 and 3.4.2. It is written in C++.

Usage: slice2rst [-I\<dir> [-I...]] \<slice file>

The -I\<dir> option lets you add directories for other .ice files you may
choose to include in your project (exactly as slice2cpp does it).

The program outputs the to stdout.

To build you may need to set ICE_HOME and MCPP_HOME depending on your default
paths, invoke make from the src/ directory.

Warning
-------
* This uses an undocumented Slice Parser API.
* This is incomplete
