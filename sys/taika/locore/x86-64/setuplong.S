#define __ASSEMBLER__ 1
#include <kern/unit/x86-64/boot.h>
#include <kern/unit/x86-64/vm.h>
/*
 * enable long mode
 */

.extern kernpagemapl4tab

.text	32

.align	4

	/*
	 * - paging is disabled
	 * - set CR4PAE-bit in CR4
	 * - load CR3 with physical address of PML4 (kernpmaplvl4)
	 * - set EFERLME-bit at EFERADR
	 * - enable paging
	 */
pginitlong:
	movl	$kernpagemapl4tab, %edi
	orl	$CR4PAE, %eax
	movl	%eax, %cr4
	movl	%edi, %cr3
	movl	$(EFERADR | EFELME), %eax
	movl	%eax, EFERADR
	movl	%cr4, %eax
