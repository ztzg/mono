#!/bin/env perl

# This script extracts from the file 'mini-sh4.c' information to build the file
# 'cpu-sh4.md'. The information should have the following format :
#
#     /* MD: <description> */
#
# where "<description>" is a suitable line for the machine description parser.

use strict;
use warnings;

my $header = << 'EOF';
# SH4 cpu description file
#
# This file is read by genmdesc to produce a table with all the relevant information
# about the cpu instructions that may be used by the register allocator, the scheduler
# and other parts of the arch-dependent part of mini.
#
# An opcode name is followed by a colon and optional specifiers.
# A specifier has a name, a colon and a value. Specifiers are separated by white space.
# Here is a description of the specifiers valid for this file and their possible values.
#
# dest:register       describes the destination register of an instruction
# src1:register       describes the first source register of an instruction
# src2:register       describes the second source register of an instruction
#
# register may have the following values:
#	i  integer register
#	0  sh4_r0 register
#	t  sh4_temp register
#	I  any integer register but sh4_temp
#	b  base register (used in address references)
#	f  floating point register
#
# len:number         describe the maximun length in bytes of the instruction
# 		     number is a positive integer.  If the length is not specified
#                    it defaults to zero.   But lengths are only checked if the given opcode
#                    is encountered during compilation. Some opcodes, like CONV_U4 are
#                    transformed into other opcodes in the brg files, so they do not show up
#                    during code generation.
#
# cost:number        describe how many cycles are needed to complete the instruction (unused)
#
# clob:spec          describe if the instruction clobbers registers or has special needs
#
# spec can be one of the following characters:
#	c  clobbers local registers
#	0  clobbers the register sh4_r0
#
# flags:spec        describe if the instruction uses or sets the flags (unused)
#
# spec can be one of the following chars:
# 	s  sets the flags
#       u  uses the flags
#       m  uses and modifies the flags
#
# res:spec          describe what units are used in the processor (unused)
#
# delay:            describe delay slots (unused)
#
# the required specifiers are: len, clob (if registers are clobbered), the registers
# specifiers if the registers are actually used, flags (when scheduling is implemented).
#
# See the code in mini-sh4.c for more details on how the specifiers are used.

EOF

open(my $desc, ">cpu-sh4.md") or die $!;
open(my $core, "mini-sh4.c") or die $!;

print $desc $header;

foreach my $line (<$core>) {
	print $desc "$1\n" if $line =~ /\/\* MD: (.*) \*\//;
}
