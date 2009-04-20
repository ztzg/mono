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
#	z  sh4_r0 register
#	Z  sh4_r0:1 registers
#	b  base register (used in address references)
#	f  floating point register
#	y  sh4_dr0 register
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
#	z  clobbers the register sh4_r0
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

int_add_imm: clob:1 dest:i src1:i len:2
add_imm: clob:1 dest:i src1:i len:2
int_add: clob:1 dest:i src1:i src2:i len:2
addcc: clob:1 dest:i src1:i src2:i len:4
int_addcc: clob:1 dest:i src1:i src2:i len:4
adc: clob:1 dest:i src1:i src2:i len:2
int_adc: clob:1 dest:i src1:i src2:i len:2
int_sub: clob:1 dest:i src1:i src2:i len:2
subcc: clob:1 dest:i src1:i src2:i len:4
int_subcc: clob:1 dest:i src1:i src2:i len:4
sbb: clob:1 dest:i src1:i src2:i len:2
int_sbb: clob:1 dest:i src1:i src2:i len:2
sh4_or_imm_R0: clob:1 dest:z src1:z len:2
int_or: clob:1 dest:i src1:i src2:i len:2
sh4_xor_imm_R0: clob:1 dest:z src1:z len:2
int_xor: clob:1 dest:i src1:i src2:i len:2
sh4_and_imm_R0: clob:1 dest:z src1:z len:2
int_and: clob:1 dest:i src1:i src2:i len:2
sh4_shll1: clob:1 dest:i src1:i len:2
sh4_shll2: clob:1 dest:i src1:i len:2
sh4_shll8: clob:1 dest:i src1:i len:2
sh4_shll16: clob:1 dest:i src1:i len:2
sh4_shar1: clob:1 dest:i src1:i len:2
sh4_shlr1: clob:1 dest:i src1:i len:2
sh4_shlr2: clob:1 dest:i src1:i len:2
sh4_shlr8: clob:1 dest:i src1:i len:2
sh4_shlr16: clob:1 dest:i src1:i len:2
int_shl: clob:1 dest:i src1:i src2:i len:2
int_shr_un: clob:1 dest:i src1:i src2:i len:4
int_shr: clob:1 dest:i src1:i src2:i len:4
int_mul: dest:i src1:i src2:i len:4
sh4_cmpeq: src1:i src2:i len:2
sh4_cmpeq_imm_R0: src1:z len:2
sh4_cmpgt: src1:i src2:i len:2
sh4_cmphi: src1:i src2:i len:2
sh4_cmpge: src1:i src2:i len:2
sh4_cmphs: src1:i src2:i len:2
sh4_not_cmpge: src1:i src2:i len:2
sh4_not_cmphs: src1:i src2:i len:2
sh4_fcmpeq: src1:f src2:f len:2
sh4_fcmpgt: src1:f src2:f len:2
sh4_movt: dest:i len:2
sh4_push_arg: src1:i len:2
sh4_push_f32arg: src1:f len:6
sh4_push_f64arg: src1:f len:4
sh4_storei1_membase_R0: src1:z dest:b len:2
sh4_storei1: src1:i dest:b len:2
sh4_storei2_membase_R0: src1:z dest:b len:2
sh4_storei2: src1:i dest:b len:2
sh4_storei4_membase: src1:i dest:b len:2
sh4_storei4: src1:i dest:b len:2
sh4_loadi1_membase_R0: dest:z src1:b len:2
sh4_loadi1: dest:i src1:b len:2
sh4_loadi2_membase_R0: dest:z src1:b len:2
sh4_loadi2: dest:i src1:b len:2
sh4_loadu1_membase_R0: dest:z src1:b len:4
sh4_loadu1: dest:i src1:b len:4
sh4_loadu2_membase_R0: dest:z src1:b len:4
sh4_loadu2: dest:i src1:b len:4
sh4_loadi4_membase: dest:i src1:b len:2
sh4_loadi4: dest:i src1:i len:2
iconst: dest:i len:12
fcall: dest:y clob:c len:30
voidcall: clob:c len:30
vcall: clob:c len:30
vcall2: clob:c len:30
lcall: dest:Z clob:c len:30
call: dest:z clob:c len:30
fcall_reg: dest:y src1:i clob:c len:18
voidcall_reg: src1:i clob:c len:18
lcall_reg: dest:Z src1:i clob:c len:18
call_reg: dest:z src1:i clob:c len:18
call_handler: len:16
throw: src1:i len:18
rethrow: src1:i len:18
int_conv_to_i4: dest:i src1:i len:2
int_conv_to_u4: dest:i src1:i len:2
move: dest:i src1:i len:2
int_conv_to_i1: dest:i src1:i len:2
int_conv_to_u1: dest:i src1:i len:2
int_conv_to_i2: dest:i src1:i len:2
int_conv_to_u2: dest:i src1:i len:2
start_handler: len:8
endfilter: src1:z len:10
endfinally: len:10
br: len:16
br_reg: src1:i len:4
sh4_bt: len:18
sh4_bf: len:18
sh4_clrt: len:2
sh4_negc: dest:i src1:i len:2
sh4_rotcl: dest:i src1:i len:2 clob:1
sh4_tst: src1:i src2:i len:2
int_not: dest:i src1:i len:2
int_neg: dest:i src1:i len:2
label: len:0
checkthis: src1:b len:2
store_membase_reg: dest:b src1:i len:16
load_membase: dest:i src1:b len:16
nop: len:0
dummy_use: len:0
dummy_store: len:0
not_null: len:0
not_reached: len:2
break: len:2
jump_table: dest:i len:12
setlret: src1:i src2:i len:0
sh4_bzero4: dest:i src1:b src2:i len:18
fmove: dest:f src1:f len:4
r8const: dest:f clob:z len:24
r4const: dest:f clob:z len:20
int_conv_to_r4: dest:f src1:i len:4
int_conv_to_r8: dest:f src1:i len:4
float_conv_to_i: dest:i src1:f len:4
float_conv_to_i4: dest:i src1:f len:4
float_conv_to_i2: dest:i src1:f len:6
float_conv_to_u2: dest:i src1:f len:6
float_conv_to_i1: dest:i src1:f len:6
float_conv_to_u1: dest:i src1:f len:6
sh4_fcnvsd: dest:f src1:f len:4
float_conv_to_r4: dest:f src1:f len:0
float_conv_to_r8: dest:f src1:f len:0
float_conv_to_i8: dest:l src1:f len:0
float_conv_to_u8: dest:l src1:f len:0
float_add: clob:1 dest:f src1:f src2:f len:2
float_sub: clob:1 dest:f src1:f src2:f len:2
float_mul: clob:1 dest:f src1:f src2:f len:2
float_div: clob:1 dest:f src1:f src2:f len:2
float_neg: dest:f src1:f len:2
storer8_membase_reg: dest:b src1:f len:16
loadr8_membase: dest:f src1:b len:16
storer4_membase_reg: dest:b src1:f len:18
loadr4_membase: dest:f src1:b len:18
ckfinite: dest:f src1:f len:0
jmp: len:0
long_conv_to_ovf_i4_2: dest:i src1:i src2:i len:0
