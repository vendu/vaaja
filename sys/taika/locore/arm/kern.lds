OUTPUT_FORMAT("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")

PAGESIZE = 4K;
BOOTBASE = 0x00000000;
PAGETAB = 4K;
PAGETABSIZE = 4096;

ENTRY(_start)

SECTIONS {

    . = BOOTBASE;

    /* identity-mapped low kernel segment */
    .boot : AT(BOOTBASE) {
        setup.o(.text)
        trap.o(.text)
        main.o(.text)
	vm.o(.text)
	vm.o(.bss)
	. = ALIGN(PAGESIZE);
        _elocore = .;
    }

    . = PAGETAB;

    .pagetab : AT(PAGETAB) {
        _l2pagetab = .;
	. += PAGETABSIZE;
    }

    .text : AT(PAGETAB + PAGETABSIZE) {
	*(.text)
	. = ALIGN(PAGESIZE);
	_etext = .;
    }

    /* read-write data segment; initialised global structures */
    .data : AT(PAGETAB + PAGETABSIZE + SIZEOF(.text)) {
    	_data = _etext;
	*(.data)
	. = ALIGN(PAGESIZE);
    }

    /* bss segment; runtime-allocated, uninitialised data */
    .bss : AT(PAGETAB + PAGETABSIZE + SIZEOF(.text) + SIZEOF(.data)) {
        _bss = _data + SIZEOF(.data);
	*(.bss)
	*(COMMON)
	. = ALIGN(PAGESIZE);
        _ebss = _bss + SIZEOF(.bss);
    }
}

