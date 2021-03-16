# Deck Soft-Processor TODO List

## Pipeline
- 5-stage pipeline? fetch, decode/conditional, execute, memory access,
  update registers
- https://github.com/openrisc/mor1kx/blob/master/doc/mor1kx.asciidoc
  - deck_fetch; instruction fetch: icache + immu
  - deck_decode; instruction decode
  - deck_exec; instruction execution unit ('ALU')
  - deck_lsu; load/store: dcache + dmmu
  - deck_wb; writeback
  - deck_regfile; register file
  - deck_ctrl; control - tmr, intr, ...
- 6-stage cappuccino pipeline
  - support for caches, MMUs, delay slot on jump and branch instructions, EVBAR
  - address
  - fetch
    - deck_icache
    - deck_immu
    - deck_ifetch
  - decode
    - deck_idecode; ALU/LSU and control operation signals, including exceptions
      such as illegal instruction and system-call or exceptions from earlier
      stages, e.g. bus error, are generated or passed through
  - execute
    - deck_alu
    - deck_wbmux; take ALU/LSU-result inputs; generate link address for JAL
    - deck_regfile; handle forwarding from control/memory and writeback stages
  A    to execute stage
    - deck_exec_ctrl; determine when ALU and LSU are done, control write-enable
      to register file for any result, propagate exception signals from execute
      or earlier stages
  - control/memory
    - deck_dcache
    - deck_dmmu
    - deck_lsu; access generic bus, to date cache or not, out to the selected
      bus; handles sign extension where required; checks alignment and handles
      bus error back to the exec_ctrl unit if not met
    - deck_ctrl; intr, dbg, tmr, pipectrl (advance/stall, exception-handling)
    - deck_branch; output branch + adr to the fetch stage
  - writeback

