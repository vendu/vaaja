# zen kernel design notes

# source modules

## hw - hardware interface

### trap         - interrupts, exceptions, aborts, faults

function                            | brief
------------------------------------|------
trapconf(tid, func, errfunc);       | set handler and error handler for trap #tid