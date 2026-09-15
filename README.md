# simple cli temp converter

it's not exactly lightweight, but it *is* in C (whereas previously for a class i implemented this in C++)

use the makefile provided in /src to compile. it's only one file, though, so that's not all that necessary

tested only in a unix environment, but likely compatible with a windows environment

usage: `./<binary_name> NUMBER -FLAG` where NUMBER is any real number and FLAG is one of CcFfKk
unfortunately, the program doesn't currently support negative numbers for input
