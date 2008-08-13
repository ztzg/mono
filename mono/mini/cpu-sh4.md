# Sh4 cpu description file

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
#
# TODO adc:
# TODO add_imm:
# TODO addcc:
# TODO and_imm:
# TODO aot_const:
# TODO bigmul:
# TODO bigmul_un:
# TODO br:
# TODO br_reg:
# TODO break:
# TODO call:
# TODO call_handler:
# TODO call_membase:
# TODO call_reg:
# TODO ceq:
# TODO cgt:
# TODO checkthis:
# TODO ckfinite:
# TODO clt:
# TODO compare:
# TODO compare_imm:
# TODO div_imm:
# TODO div_un_imm:
# TODO endfilter:
# TODO endfinally:
# TODO fcall:
# TODO fcall_membase:
# TODO fcall_reg:
# TODO fcompare:
# TODO float_add:
# TODO float_beq:
# TODO float_bge:
# TODO float_bge_un:
# TODO float_bgt:
# TODO float_bgt_un:
# TODO float_ble:
# TODO float_ble_un:
# TODO float_blt:
# TODO float_blt_un:
# TODO float_bne_un:
# TODO float_ceq:
# TODO float_cgt:
# TODO float_cgt_un:
# TODO float_clt:
# TODO float_clt_un:
# TODO float_conv_to_i:
# TODO float_conv_to_i1:
# TODO float_conv_to_i2:
# TODO float_conv_to_i4:
# TODO float_conv_to_i8:
# TODO float_conv_to_u:
# TODO float_conv_to_u1:
# TODO float_conv_to_u2:
# TODO float_conv_to_u4:
# TODO float_conv_to_u8:
# TODO float_div:
# TODO float_div_un:
# TODO float_mul:
# TODO float_neg:
# TODO float_not:
# TODO float_rem:
# TODO float_rem_un:
# TODO float_sub:
# TODO iconst:
# TODO int_add:
# TODO int_and:
# TODO int_beq:
# TODO int_bge:
# TODO int_bgt:
# TODO int_ble:
# TODO int_blt:
# TODO int_div:
# TODO int_mul:
# TODO int_neg:
# TODO int_not:
# TODO int_or:
# TODO int_rem:
# TODO int_shl:
# TODO int_shr:
# TODO int_sub:
# TODO int_xor:
# TODO jmp:
# TODO label:
# TODO lcall:
# TODO lcall_membase:
# TODO lcall_reg:
# TODO load_membase:
# TODO loadi1_membase:
# TODO loadi2_membase:
# TODO loadi4_membase:
# TODO loadr4_membase:
# TODO loadr8_membase:
# TODO loadu1_membase:
# TODO loadu2_membase:
# TODO loadu4_mem:
# TODO loadu4_membase:
# TODO localloc:
# TODO long_conv_to_ovf_i:
# TODO memory_barrier:
# TODO move:
# TODO mul_imm:
# TODO oparglist:
# TODO or_imm:
# TODO outarg:
# TODO r4const:
# TODO r8const:
# TODO rem_imm:
# TODO rem_un_imm:
# TODO rethrow:
# TODO sbb:
# TODO setret:
# TODO shl_imm:
# TODO shr_imm:
# TODO shr_un_imm:
# TODO start_handler:
# TODO store_membase_reg:
# TODO storei1_membase_reg:
# TODO storei2_membase_reg:
# TODO storei4_membase_reg:
# TODO storer4_membase_reg:
# TODO storer8_membase_reg:
# TODO sub_imm:
# TODO subcc:
# TODO throw:
# TODO vcall:
# TODO vcall_membase:
# TODO vcall_reg:
# TODO voidcall:
# TODO voidcall_membase:
# TODO voidcall_reg:
# TODO xor_imm:
