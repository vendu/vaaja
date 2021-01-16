#include <netinet/in.h>
#include <zero/cdefs.h>
#include <mach/param.h>

const struct in6_addr in6addr_any ALIGNED(CLSIZE) = IN6ADDR_ANY_INIT;
const struct in6_addr in6addr_loopback ALIGNED(CLSIZE) = IN6ADDR_LOOPBACK_INIT;

