#include <mach/param.h>
#include <env/cdefs.h>
#include <sys/zen/api.h>

kintrdesc_t                     k_intrvec[VOIMA_MAX_TRAPS] C_ALIGNED(MACH_PAGE_SIZE);
kpagedesc_t                     k_pagedir[MACH_PAGE_DIR_SIZE] C_ALIGNED(MACH_PAGE_SIZE);

