/*
 * load low kernel at 1M physical
 * DMA buffers (8 * 128K) at 4M physical
 * locate high kernel at 3G virtual
 */

MPENTRY		= 0x9f000;
HICORE		= 1M;
USYSINFOSIZE    = PAGESIZE;
DMABUF		= 8M;
DMABUFSIZE	= 8M;
PAGETAB		= 16M;
PAGESIZE	= 4K;
PAGETABSIZE	= 4M;
UPAGETAB	= 20M;
VIRTBASE	= 0xc0000000;

OUTPUT_FORMAT("elf32-i386")

ENTRY(_start)

SECTIONS {

/*
    . = MPENTRY;

    .mp : AT(MPENTRY) {
        mpentry.o(.text)
        mpentry.o(.data)
        mpentry.o(.bss)
        mp.o(.text)
        mp.o(.data)
        mp.o(.bss)
    }
*/

    . = HICORE;

    /* identity-mapped low kernel segment */
    .boot : AT(HICORE) {
    	boot.o(.text)
    	boot.o(.text.*)
    	boot.o(.data)
	setup.o(.text)
	setup.o(.text.*)
	setup.o(.data)
	real.o(.text)
	real.o(.text.*)
	real.o(.data)
	main.o(.text)
	main.o(.text.*)
	main.o(.data)
	main.o(.bss)
/*
        mp.o(.text)
        mp.o(.data)
        mp.o(.bss)
*/
	seg.o(.text)
	seg.o(.text.*)
	seg.o(.data)
	trap.o(.text)
	trap.o(.text.*)
	trap.o(.data)
	m_trap.o(.text)
	m_trap.o(.text.*)
	m_trap.o(.data)
	pic.o(.text)
	pic.o(.text.*)
	pic.o(.data)
	protmode.o(.text)
	protmode.o(.text.*)
	protmode.o(.data)
	vm.o(.text)
	vm.o(.text.*)
	vm.o(.data)
	sig.o(.text)
	sig.o(.text.*)
	sig.o(.data)
	sig.o(.bss)
/* 	vm.o(.bss) */
	util.o(.text)
	util.o(.text.*)
	util.o(.data)
	ps2.o(.text)
	ps2.o(.text.*)
	ps2.o(.data)
	vbe.o(.text)
	vbe.o(.text.*)
	vbe.o(.data)
	vbe.o(.bss)
        _eboot = .;
    }

    .usysinfo : {
        _usysinfo = .;
	. += USYSINFOSIZE;
	_eusysinfo = .;
    }

    . = DMABUF;

    /* DMA buffers (below 16 megabytes) */
    .dma : AT(DMABUF) {
        _dmabuf = ABSOLUTE(.);
	. += DMABUFSIZE;
        _edmabuf = ABSOLUTE(.);
    }

    . = PAGETAB;

    /* identity-mapped set of page tables */
    .pgtab : AT(PAGETAB) {
    	_pagetab = ABSOLUTE(.);
  	. += PAGETABSIZE;
    	_epagetab = ABSOLUTE(.);
    }

/*    . = UPAGETAB; */

    . = VIRTBASE;

    /* sections mapped by virtual addresses */

    /* read-only segment; code and some data */
    .text : AT(PAGETAB + PAGETABSIZE) {
    	_text = ABSOLUTE(.);
	*(EXCLUDE_FILE(*usr.o) .text*)
	*(.text)
	*(.text.*)
	*(.eh*)
	. = ALIGN(PAGESIZE);
	_etext = ABSOLUTE(.);
    }

    .rodata : AT(_etext) {
    	_rodata = ABSOLUTE(.);
	*(.rodata*)
	*(.rodata.*)
	. = ALIGN(PAGESIZE);
    	_erodata = ABSOLUTE(.);
    }

    /* read-write data segment; initialised global structures */
    .data : AT(_erodata) {
    	_data = ABSOLUTE(.);
	*(.data)
	*(.data.*)
	*(.got.*)
	. = ALIGN(PAGESIZE);
    	_edata = ABSOLUTE(.);
    }

    .debug : AT(_edata) {
        _debug = ABSOLUTE(.);
        *(.comment*)
        *(.debug*)
	. = ALIGN(PAGESIZE);
        _edebug = ABSOLUTE(.);
    }

    /* bss segment; runtime-allocated, uninitialised data */
    .bss : AT(_edebug) {
    	_bss = ABSOLUTE(.);
	*(.bss)
	. = ALIGN(PAGESIZE);
    	_ebss = ABSOLUTE(.);
    }

/*
    .usr : AT(_ebss) {
    	_usr = ABSOLUTE(.);
	usr.o(.text)
	. = ALIGN(PAGESIZE);
	_eusr = ABSOLUTE(.);
    }
*/
}

