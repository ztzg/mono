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
#	c  clobbers caller-save registers
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

sh4_icompare_imm_R0: src1:0 len:2
icompare: src1:i src2:i len:2
store_membase_imm: clob:0 dest:b len:14
storei4_membase_imm: clob:0 dest:b len:14
store_membase_reg: clob:0 dest:b src1:i len:16
storei4_membase_reg: clob:0 dest:b src1:i len:16
load_membase: dest:i src1:b len:2
loadu4_membase: dest:i src1:b len:2
loadi4_membase: dest:i src1:b len:2
sh4_load_membase: dest:i src1:b len:2
iconst: dest:i len:12
voidcall: clob:c len:16
call: clob:c len:16
move: dest:i src1:i len:2
voidcall_reg: src1:i clob:c len:4
call_reg: src1:i clob:c len:4
start_handler: clob:0 len:16
endfilter:  clob:0 len:18
endfinally:  clob:0 len:18
br: clob:0 len:16
int_beq: clob:0 len:18
beq: clob:0 len:18
int_bne_un:
int_cgt_un: dest:i len:16
bne.un: len:4
label: len:0
compare_imm: src1:i len:4
