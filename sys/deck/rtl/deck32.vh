`define DECK_INST_WIDTH         32      // instruction parcel size in bits
`define DECK_IMM_WIDTH          32      // size of immediate after instruction

// fields for 'standard' format instructions
`define DECK_INST_OP            5:0     // instruction ID
`define DECK_INST_COND          8:6     // execution condition
`define DECK_INST_SRC           11:9    // source register ID (U)
`define DECK_INST_DEST          15:12   // destination register ID (U or S)
`define DECK_INST_ARG           18:16   // argument register (U)
`define DECK_INST_ADR           21:19   // address mode bits; IMM, BASE, NDX
`define DECK_INST_FOLD          23:22   // fold-instruction; NONE, SHL, SHR, SAR
`define DECK_INST_PARM          31:24   // instruction-specific 8-bit parameter

// fields for jump/branch instructions
`define DECK_JMP_INST_OP        5:0     // instruction ID
`define DECK_JMP_INST_COND      8:6     // execution condition
`define DECK_JMP_INST_OFS       31:9    // PC-relative offset in 32-bit words

// fields for [register] map instructions
`define DECK_MAP_INST_OP        5:0     // instruction ID
`define DECK_MAP_INST_COND      8:6     // execution condition
`define DECK_MAP_INST_PARM      15:9    // parameter field
`define DECK_MAP_INST_BITS      30:16   // register-bits (MF is static)
`define DECK_MAP_INST_STORE     31      // 1 for STM, 0 for LDM

// fields for I/O instructions
`define DECK_IO_INST_OP         5:0     // instruction ID
`define DECK_IO_INST_COND       8:6     // execution condition
`define DECK_IO_INST_REG        11:9    // source or destination register (U)
`define DECK_IO_INST_CMD        14:12   // command register (U)
`define DECK_IO_INST_FLG        15      // 1 if command in IMM, 0 if in CMD
`define DECK_IO_INST_PORT       27:16   // source or destination port (12 bits)
`define DECK_IO_INST_IMM        31:28   // up to 4-bit command

