#!/bin/env perl

# optional | recommended | required
# depencies

use strict;
use warnings;

use File::Find;

=head1 XXX

=cut

die "Usage: 'gen-mini-port.pl \$(arch)' into Mini's sources directory\n"
    if not defined $ARGV[0] or
       not -e 'mini-arch.h';

my $arch =  $ARGV[0];
my @cpu_mds    = <cpu-*.md>;
my @mini_archs = <mini-*.h>;

=head2 Instruction selection

=cut

my %extracted = ();
my @extracted = ();

find(\&extract_opcodes, @cpu_mds);

sub extract_opcodes
{
	return if not -f or not -r;
	return if $_ eq "cpu-$arch.md";

	open(my $file, "<$_") or die $!;

	foreach my $line (<$file>) {
		if ($line =~ m/^\s*(\w+):/) {
			$extracted{$1}->{$_} = 1;
		}
	}

	close $file;
}

foreach my $opcode (keys %extracted) {
	my $found = 1;
	foreach my $cpu_md (@cpu_mds) {
		next if $cpu_md eq "cpu-$arch.md";

		if (not exists $extracted{$opcode}->{$cpu_md}) {
			if (exists $extracted{"int_" . $opcode}->{$cpu_md}) {
				$found = 2;
			} else {
				$found = 0;
				last;
			}
		}
	}

	push @extracted, "$opcode"     if ($found == 1);
	push @extracted, "int_$opcode" if ($found == 2);
}

my $header = "# " . ucfirst($arch) . " cpu description file\n";
$header .= '
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
';

$header .= "# See the code in mini-$arch.c for more details on how the specifiers are used.\n#\n";

open(my $file, ">cpu-$arch.md") or die $!;
print $file $header;
print $file "# TODO $_:\n" foreach (sort @extracted);
close $file;

=head2 Native code emission

=cut

#TODO;

=head2 Call conventions and register allocation

=cut

my %description = (
	MONO_ARCH_AOT_PLT_OFFSET_REG =>
'This macro defines the register holding the PLT offset
for calls made from AOT code. This optional macro is used
only if the macro MONO_ARCH_HAVE_CREATE_TRAMPOLINE_FROM_TOKEN
is defined.',

	MONO_ARCH_AOT_SUPPORTED =>
'The support to AOT compilation will be enabled if this
macro is defined.',

	MONO_ARCH_BASEREG =>
'This macro defines the register holding the frame pointer.
This macro is only used by the x86 support but other
architectures should ignore it.',

	MONO_ARCH_BIGMUL_INTRINS =>
'This macro should be defined if your architecture supports
natively (i.e. not emulated) the "bigmul" opcode.',

	MONO_ARCH_CALLEE_FREGS =>
'Bitmask selecting the caller-saved floating-point registers,
these are usually used for local allocation. This macro is
mandatory even if the soft floating-point support is enabled.',

	MONO_ARCH_CALLEE_REGS =>
'Bitmask selecting the caller-saved general registers, these
are usually used for local allocation. This macro is mandatory.',

	MONO_ARCH_CALLEE_SAVED_FREGS =>
'Bitmask selecting the callee-saved floating-point registers,
these are usually used for global allocation. This macro is
mandatory even if the soft floating-point support is enabled.',

	MONO_ARCH_CALLEE_SAVED_REGS =>
'Bitmask selecting the callee-saved general registers, these
are usually used for global allocation. This macro is mandatory.',

	MONO_ARCH_CODE_ALIGNMENT =>
'This macro is not yet used.',

	MONO_ARCH_COMMON_VTABLE_TRAMPOLINE =>
'This macro should be defined if your architecture supports the
common vtable trampoline.',

	MONO_ARCH_CONTEXT_DEF =>
'TODO - CV (IA64 specific ?).',

	MONO_ARCH_CPU_SPEC =>
'This macro defines the name of the machine description generated
by genmdesc from cpu-<ARCH>.md during the build process. Obviously,
this macro is mandatory.',

	MONO_ARCH_EMIT_BOUNDS_CHECK =>
'This macro overrides the generic code emission for array bound
checking.',

	MONO_ARCH_EMULATE_CONV_R8_UN =>
'If this macro is defined, the opcode to convert an unsigned 32 bits
integer to a double float will be emulated.',

	MONO_ARCH_EMULATE_DIV =>
'If this macro is defined, the division opcodes will be emulated.',

	MONO_ARCH_EMULATE_FCONV_TO_I8 =>
'If this macro is defined, the opcode to convert a double float to
a 64 bits integer will be emulated.',

	MONO_ARCH_EMULATE_FREM =>
'Not used.',

	MONO_ARCH_EMULATE_LCONV_TO_R4 =>
'If this macro is defined, the opcode to convert a 64 bits integer
to a float will be emulated.',

	MONO_ARCH_EMULATE_LCONV_TO_R8 =>
'If this macro is defined, the opcode to convert a 64 bits integer
to a double float will be emulated.',

	MONO_ARCH_EMULATE_LCONV_TO_R8_UN =>
'If this macro is defined, the opcode to convert from a unsigned 64
bits integer to a float will be emulated.',

	MONO_ARCH_EMULATE_LMUL =>
'Not used.',

	MONO_ARCH_EMULATE_LONG_MUL_OPTS =>
'Not used.',

	MONO_ARCH_EMULATE_MUL_DIV =>
'If this macro is defined, the multiplication and division opcodes
will be emulated.',

	MONO_ARCH_ENABLE_EMIT_STATE_OPT =>
'It is recommended to define this macro because it enables
an optimization in emit_state() to avoid allocation of new
registers.',

	MONO_ARCH_ENABLE_GLOBAL_RA =>
'If this macro is defined, the new global regallocator will be enabled.
Currently, it works only on AMD64.',

	MONO_ARCH_ENABLE_MONO_LMF_VAR =>
'If this macro is defined, the current "Last Managed Frame"
is stored in a TLS variable instead of in jit_tls->lmf.',

	MONO_ARCH_ENABLE_NORMALIZE_OPCODES =>
'If this macro is defined, CEE_ and OP_ opcodes are replaced
with the corresponding OP_I or OP_L opcodes. It is recommended
to define this macro',

	MONO_ARCH_ENABLE_REGALLOC_IN_EH_BLOCKS =>
'Not used.',

	MONO_ARCH_ENCODE_LREG =>
'Not used.',

	MONO_ARCH_FPSTACK_SIZE =>
'This macro defines the size of the floating-point stack.
Use the value 0 if no floating-point stack is available.',

	MONO_ARCH_FP_RETURN_REG =>
'This macro defines the register holding the return value
of a floating-point operation. This optional macro is used
only if the macro MONO_ARCH_ENABLE_GLOBAL_RA is defined.',

	MONO_ARCH_FRAME_ALIGNMENT =>
'This macro defined the frame pointer alignment constraint.',

	MONO_ARCH_FRETREG1 =>
'Not used.',

	MONO_ARCH_HAVE_ATOMIC_ADD =>
'This macro should be defined if your architecture supports
natively (i.e. not emulated) the "atomic add" opcode.',

	MONO_ARCH_HAVE_ATOMIC_CAS_IMM =>
'This macro should be defined if your architecture supports
natively (i.e. not emulated) the "atomic compare/exchange" opcode.',

	MONO_ARCH_HAVE_ATOMIC_EXCHANGE =>
'This macro should be defined if your architecture supports
natively (i.e. not emulated) the "atomic exchange" opcode.',

	MONO_ARCH_HAVE_CMOV_OPS =>
'TODO - CV',

	MONO_ARCH_HAVE_CREATE_DELEGATE_TRAMPOLINE =>
'TODO - CV',

	MONO_ARCH_HAVE_CREATE_TRAMPOLINE_FROM_TOKEN =>
'TODO - CV',

	MONO_ARCH_HAVE_DECOMPOSE_LONG_OPTS =>
'TODO - CV',

	MONO_ARCH_HAVE_DECOMPOSE_OPTS =>
'TODO - CV',

	MONO_ARCH_HAVE_FULL_AOT_TRAMPOLINES =>
'TODO - CV',

	MONO_ARCH_HAVE_IMT =>
'TODO - CV',

	MONO_ARCH_HAVE_INVALIDATE_METHOD =>
'TODO - CV',

	MONO_ARCH_HAVE_IS_INT_OVERFLOW =>
'TODO - CV',

	MONO_ARCH_HAVE_NOTIFY_PENDING_EXC =>
'TODO - CV',

	MONO_ARCH_HAVE_SAVE_UNWIND_INFO =>
'TODO - CV',

	MONO_ARCH_HAVE_THROW_CORLIB_EXCEPTION =>
'TODO - CV',

	MONO_ARCH_HAVE_TLS_GET =>
'TODO - CV',

	MONO_ARCH_HAVE_UNWIND_TABLE =>
'TODO - CV',

	MONO_ARCH_IMT_REG =>
'TODO - CV',

	MONO_ARCH_INIT_TOP_LMF_ENTRY =>
'TODO - CV',

	MONO_ARCH_INST_FIXED_MASK =>
'TODO - CV',

	MONO_ARCH_INST_FIXED_REG =>
'This macro provides, from the machine-description of an instruction,
the fixed hreg corresponding to that value. This macro is mandatory.',

	MONO_ARCH_INST_IS_FLOAT =>
'This macro tests if a register is floating point register (from
the machine-description of an instruction). Usually it looks like
(desc == \'f\' || desc == \'g\'). This macro is mandatory even if
the soft floating-point support is enabled.',

	MONO_ARCH_INST_IS_REGPAIR =>
'This macro tests if a register denotes a register pair (from
the machine-description of an instruction). Usually it looks like
(desc == \'l\' || desc == \'L\'). This macro is mandatory.',

	MONO_ARCH_INST_REGPAIR_REG2 =>
'This macro provides, from the first register of a pair, the bitmask
selecting the hregs which can be used for allocating the second
register.',

	MONO_ARCH_INST_SREG2_MASK =>
'This macro is only used by the ARM support but other
architectures define it to "(0)".',

	MONO_ARCH_IS_GLOBAL_IREG =>
'TODO - CV',

	MONO_ARCH_IS_OP_MEMBASE =>
'TODO - CV',

	MONO_ARCH_LOCALLOC_ALIGNMENT =>
'TODO - CV',

	MONO_ARCH_NEED_DIV_CHECK =>
'TODO - CV',

	MONO_ARCH_NEED_GOT_VAR =>
'TODO - CV',

	MONO_ARCH_NOMAP32BIT =>
'TODO - CV',

	MONO_ARCH_NO_EMULATE_LONG_MUL_OPTS =>
'If this macro is *NOT* defined, the opcode to multiply 64 bits
integers (signed or not) will be emulated.',

	MONO_ARCH_NO_EMULATE_LONG_SHIFT_OPS =>
'If this macro is *NOT* defined, the opcode to shift 64 bits
integers (signed or not) will be emulated.',

	MONO_ARCH_NO_EMULATE_MUL =>
'If this macro is defined, allow ldelema2d optimization.',

	MONO_ARCH_NO_EMULATE_MUL_IMM =>
'If this macro is defined, allow the "OP_LMUL imm -> OP_LMUL_IMM"
conversion optimization.',

	MONO_ARCH_NO_IOV_CHECK =>
'TODO - CV',

	MONO_ARCH_RETREG1 =>
'TODO - CV',

	MONO_ARCH_RETREG2 =>
'TODO - CV',

	MONO_ARCH_RGCTX_REG =>
'TODO - CV',

	MONO_ARCH_SIGNAL_STACK_SIZE =>
'TODO - CV',

	MONO_ARCH_SIGSEGV_ON_ALTSTACK =>
'TODO - CV',

	MONO_ARCH_SOFT_FLOAT =>
'If this macro is defined, the software floating point support will
be enabled. This macro is required if your architecture does not
have hardware support for floating point.',

	MONO_ARCH_STACK_GROWS_UP =>
'TODO - CV',

	MONO_ARCH_USE_FPSTACK =>
'This macro defines whether the architecture uses a floating point
stack. Obviously, this macro is mandatory.',

	MONO_ARCH_USE_SIGACTION =>
'This macro defines whether the architecture supports POSIX sigactions.
This macro should be defined for all POSIX platforms.',

	MONO_ARCH_VARARG_ICALLS =>
'TODO - CV',

	MONO_ARCH_VTABLE_REG =>
'TODO - CV',

	MONO_ARM_REGSAVE_MASK =>
'TODO - CV',

	MONO_CONTEXT_GET_BP =>
'Get the frame pointer from a Mono context. This macro is
mandatory.',

	MONO_CONTEXT_GET_IP =>
'Get the instruction pointer from a Mono context. This macro is
mandatory.',

	MONO_CONTEXT_GET_SP =>
'Get the stack pointer from a Mono context. This macro is mandatory.',

	MONO_CONTEXT_SET_BP =>
'Set the frame pointer of a Mono context. This macro is
mandatory.',

	MONO_CONTEXT_SET_FUNC =>
'TODO - CV',

	MONO_CONTEXT_SET_IP =>
'Set the instruction pointer of a Mono context. This macro is
mandatory.',

	MONO_CONTEXT_SET_SP =>
'Set the stack pointer of a Mono context.',

	MONO_EMIT_NEW_BRANCH_BIREG_LABEL =>
'TODO - CV',

	MONO_EMIT_NEW_BRANCH_NONZERO_LABEL =>
'TODO - CV',

	MONO_EMIT_NEW_BRANCH_UNREG_LABEL =>
'TODO - CV',

	MONO_EMIT_NEW_COMPARE_BRANCH_BLOCK =>
'TODO - CV',

	MONO_EMIT_NEW_COMPARE_BRANCH_BLOCK_CHAINED =>
'TODO - CV',

	MONO_EMIT_NEW_COMPARE_BRANCH_LABEL =>
'TODO - CV',

	MONO_EMIT_NEW_COMPARE_BRANCH_LABEL_CHAINED =>
'TODO - CV',

	MONO_EMIT_NEW_COMPARE_EXC =>
'TODO - CV',

	MONO_EMIT_NEW_COMPARE_IMM_BRANCH_BLOCK =>
'TODO - CV',

	MONO_EMIT_NEW_COMPARE_IMM_BRANCH_BLOCK_CHAINED =>
'TODO - CV',

	MONO_EMIT_NEW_COMPARE_IMM_BRANCH_LABEL =>
'TODO - CV',

	MONO_EMIT_NEW_COMPARE_IMM_BRANCH_LABEL_CHAINED =>
'TODO - CV',

	MONO_EMIT_NEW_COMPARE_IMM_EXC =>
'TODO - CV',

	MONO_EMIT_NEW_HPPA_COND_EXC =>
'TODO - CV',

	MONO_EMIT_NEW_ICOMPARE_IMM_EXC =>
'TODO - CV',

	MONO_EMIT_NEW_MIPS_COND_EXC =>
'TODO - CV',

	MONO_EMIT_NEW_MOVE =>
'TODO - CV',

	MONO_GET_CONTEXT =>
'TODO - CV',

	MONO_INIT_CONTEXT_FROM_CURRENT =>
'TODO - CV',

	MONO_INIT_CONTEXT_FROM_FUNC =>
'This macro retrieves the Mono context from a function pointer. This
macro have to be defined if your architecture does not support
MONO_INIT_CONTEXT_FROM_CURRENT.',

	MONO_MAX_FREGS =>
'This macro defines the maximum number of floating point registers.
Obviously, this macro is mandatory.',

	MONO_MAX_IREGS =>
'This macro defines the maximum number of integer registers (even ones
which have a special meaning). Obviously, this macro is mandatory.',

	MONO_OUTPUT_VTR =>
'TODO - CV',

	MONO_OUTPUT_VTS =>
'TODO - CV',

	MONO_SAVED_FREGS =>
'TODO - CV',

	MONO_SAVED_FREGS_MASK =>
'TODO - CV',

	MONO_SAVED_GREGS =>
'TODO - CV',

	MONO_SAVED_GREGS_MASK =>
'TODO - CV',

	MONO_SPARC_STACK_BIAS =>
'TODO - CV',

	MONO_ZERO_REG =>
'TODO - CV',
);

######################################################################

my %value = (
	MONO_ARCH_CALLEE_FREGS       => ' 0xF',
	MONO_ARCH_CALLEE_REGS        => ' ((1<<sh4_r0) | (1<<sh4_r1) | (1<<sh4_r2) | (1<<sh4_r3) | (1<<sh4_r4) | (1<<sh4_r5) | (1<<sh4_r6) | (1<<sh4_r7))',
	MONO_ARCH_CALLEE_SAVED_FREGS => ' 0',
	MONO_ARCH_CALLEE_SAVED_REGS  => ' ((1<<sh4_r8) | (1<<sh4_r9) | (1<<sh4_r10) | (1<<sh4_r11) | (1<<sh4_r12) | (1<<sh4_r13) | (1<<sh4_r14) | (1<<sh4_r15))',
	MONO_ARCH_CODE_ALIGNMENT     => ' 32 /* not used */',
	MONO_ARCH_CPU_SPEC           => ' sh4_desc',
	MONO_ARCH_FPSTACK_SIZE       => ' 0',
	MONO_ARCH_FRAME_ALIGNMENT    => ' 32',
	MONO_ARCH_INST_FIXED_REG     => '(a) (-1)', # TODO - CV
	MONO_ARCH_INST_IS_REGPAIR    => '(a) FALSE',
	MONO_ARCH_INST_REGPAIR_REG2  => '(a, b) (-1)',
	MONO_ARCH_INST_SREG2_MASK    => '(a) (0)',
	MONO_ARCH_USE_FPSTACK        => ' FALSE',
	MONO_ARCH_USE_SIGACTION      => ' 1',
	MONO_CONTEXT_GET_BP          => '(context) ((gpointer)(context)->registers[sh4_fp])',
	MONO_CONTEXT_GET_IP          => '(context) ((gpointer)(context)->pc)',
	MONO_CONTEXT_GET_SP          => '(context) ((gpointer)(context)->registers[sh4_sp])',
	MONO_CONTEXT_SET_BP          => '(context, vbp) do { (context)->registers[sh4_fp] = (guint32)(vbp); } while (0);',
	MONO_CONTEXT_SET_IP          => '(context, vpc) do { (context)->pc = (guint32)(vpc); } while (0);',
	MONO_INIT_CONTEXT_FROM_FUNC  => '(context, func) do { g_assert(0); } while (0);', # TODO - CV
	MONO_MAX_FREGS               => ' 16',
	MONO_MAX_IREGS               => ' 16',
	MONO_ARCH_SOFT_FLOAT             => ' 1',
	MONO_ARCH_EMULATE_CONV_R8_UN     => ' 1',
	MONO_ARCH_EMULATE_DIV            => ' 1',
	MONO_ARCH_EMULATE_FCONV_TO_I8    => ' 1',
	MONO_ARCH_EMULATE_FREM           => ' 1',
	MONO_ARCH_EMULATE_LCONV_TO_R4    => ' 1',
	MONO_ARCH_EMULATE_LCONV_TO_R8    => ' 1',
	MONO_ARCH_EMULATE_LCONV_TO_R8_UN => ' 1',
	MONO_ARCH_EMULATE_LMUL           => ' 1',
	MONO_ARCH_EMULATE_LONG_MUL_OPTS  => ' 1',
	MONO_ARCH_EMULATE_MUL_DIV        => ' 1',
	MonoCompileArch => ' void *', # TODO - CV
	MonoContext     => '
	guint32 pc;
	guint32 registers[MONO_MAX_IREGS]; /* TODO - CV : should I save global registers only ? */
',
	MonoLMF         => '
	gpointer    previous_lmf;
	gpointer    lmf_addr;
	MonoMethod *method;
	guint32     pc;
	guint32     registers[MONO_MAX_IREGS];
	guint32     ebp; /* required by Mini but not used with the SH4. */
',
);

######################################################################

%extracted = ();
@extracted = ();

find(\&extract_macros, @mini_archs);

sub extract_macros
{
	return if not -f or not -r;
	open(my $file, "<$_") or die $!;
	foreach my $line (<$file>) {
		foreach my $macro ($line =~ m/#\s*define\s+(MONO_\w+)/) {
			$extracted{$macro}->{$_} = 1;
		}
	}
}

foreach my $macro (keys %extracted) {
	my $found = 1;
	foreach my $mini_arch (@mini_archs) {
		next if $mini_arch eq "mini-$arch.h";
		next if $mini_arch eq "mini-arch.h";
		next if $mini_arch eq "mini-ops.h";

		if (not exists $extracted{$macro}->{$mini_arch}
		    and not exists $value{$macro}) {
			$found = 0;
			last;
		}
	}

	push @extracted, "$macro" if ($found == 1);
}

open($file, ">mini-$arch.h") or die $!;

print $file "#include <mono/arch/$arch/$arch-codegen.h>\n";
print $file "#include <glib.h>\n";
print $file "\n";

foreach my $macro (sort @extracted) {
	$description{$macro} = "UNKNOWN MACRO" if not exists $description{$macro};

	print $file "/*\n";
	print $file " * $_\n" foreach split(/\n/, $description{$macro});
	print $file "*/\n";

	if (exists $value{$macro}) {
		print $file "#define $macro$value{$macro}\n";
	} else {
		print $file "/* TODO #define $macro */\n";
	}
	print $file "\n";
}

print $file "/* Structure where the arch-specific code can store\n";
print $file " * data during a compilation. */\n";
if (exists $value{MonoCompileArch}) {
	print $file "typedef $value{MonoCompileArch} MonoCompileArch;\n";
} else {
	print $file "/* TODO typedef MonoCompileArch; */\n";
}
print $file "\n";

print $file "/* The execution state of a thread during exception handling\n";
print $file " * is stored in this arch-specific structure. */\n";
if (exists $value{MonoContext}) {
	print $file "typedef struct { $value{MonoContext} } MonoContext;\n";
} else {
	print $file "/* TODO typedef MonoContext; */\n";
}
print $file "\n";

print $file "/* When managed code needs to call into native code, it does through\n";
print $file " * a managed->native wrapper function, which is generated by the JIT.\n";
print $file " * This wrapper is responsible for saving the machine into the\n";
print $file " * following arch-specific structure. */\n";
if (exists $value{MonoLMF}) {
	print $file "struct MonoLMF { $value{MonoLMF} };\n";
} else {
	print $file "/* TODO struct MonoLMF { }; */\n";
}
print $file "\n";

close $file;

=head2 Method trampolines

=cut

#TODO;

=head2 Exception handling

=cut

#TODO;

=head2 Minor helper methods

#TODO;

=cut
