#include "sh4-codegen.h"
#include <stdio.h>

/* don't run the resulting program, it will destroy your computer,
 * just objdump -d it to inspect we generated the correct assembler.
 */

int main() {
	guint8 code [16000];
	guint8 *p = code;
	guint8 *cp;

	printf (".text\n.align 4\n.globl main\n.type main,@function\nmain:\n");

	/* TODO - CV */

	for (cp = code; cp < p; cp++) {
		printf (".byte 0x%x\n", *cp);
	}

	return 0;
}
