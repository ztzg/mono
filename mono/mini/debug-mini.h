#ifndef __DEBUG_MINI_H__
#define __DEBUG_MINI_H__

#include <mono/metadata/class-internals.h>
#include <mono/metadata/mono-debug-debugger.h>

#include "mini.h"

MonoDebugMethodAddressList *
mono_debugger_insert_method_breakpoint (MonoMethod *method, guint64 idx);

int
mono_debugger_remove_method_breakpoint (guint64 index);

typedef struct _MonoDebuggerThreadInfo MonoDebuggerThreadInfo;
extern MonoDebuggerThreadInfo *mono_debugger_thread_table;

void
mono_debugger_thread_created (gsize tid, MonoJitTlsData *jit_tls);

void
mono_debugger_thread_cleanup (MonoJitTlsData *jit_tls);

/*
 * Debugger breakpoint interface.
 */

typedef struct {
	/* This is intentionally a bitfield to allow the debugger to write
	 * both `enabled' and `opcode' in one single atomic operation. */
	guint64 enabled	  : 1;
	guint64 opcode    : 8;
	guint64 unused    : 55;
	guint64 address;
} MonoDebuggerBreakpointInfo;

#define MONO_DEBUGGER_BREAKPOINT_TABLE_SIZE	256
extern volatile const MonoDebuggerBreakpointInfo _mono_debugger_breakpoint_info_area [MONO_DEBUGGER_BREAKPOINT_TABLE_SIZE];
extern volatile const MonoDebuggerBreakpointInfo *mono_debugger_breakpoint_table [MONO_DEBUGGER_BREAKPOINT_TABLE_SIZE];

gboolean
mono_debugger_remove_breakpoints_from_memory (const guint8 *orig_address, guint8 *code, int size);

/*
 * This is the old breakpoint interface.
 * It isn't used by the debugger anymore, but still when using the `--break' command
 * line argument.
 */

int             mono_debugger_insert_breakpoint_full      (MonoMethodDesc *desc);
int             mono_debugger_remove_breakpoint           (int breakpoint_id);
void            mono_debugger_breakpoint_callback         (MonoMethod *method, guint32 idx);

#endif
