/*
 * func1(vm, src, dest);            // dual-register instruction
 * func2(vm, adr, dest);            // instruction with source address
 * func3(vm, src, adr);             // instruction with destination address
 * func4(vm, src1, src2, adr);      // atomic instructions
 */

/*
 * v0casop(want, need, val);
 * - returns 0 on success, non-0 on failure
 */
int32_t
v0casop(struct v0vm * C_RESTRICT vm,
        int_fast8_t sreg1,
        int_fast8_t sreg2,
        int32_t destadr)
{
    int32_t                     want = vm->intregs[sreg1];
    int32_t                     need = vm->intregs[sreg2];
    int32_t                     old;
    int32_t                     ret;

    v0lkbus(vm);
    old = *(int32_t *)vm->mem[destadr];
    if (old == want) {
        *(int32_t *)vm->mem[destadr] = need;
    }
    v0unlkbus(vm);
    ret = old - want;

    return ret;
}

/*
 * fetch next instruction
 * - store possible immediate value/offset through *immptr; pay attention to
 *   have 32- or 64-bit storage available at the location
 */
static C_INLINE struct v0inst
v0fetchinst(struct v0vm * C_RESTRICT vm,
            void *immret)
{
    uint_fast8_t                pc = vm->pc;
    int32_t                    *op = (int32_t *)&vm->mem[pc];

    if (v0ophasimm(op)) {

    }
}

