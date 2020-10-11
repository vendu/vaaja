#include <limits.h>
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <zero/cdefs.h>
#include <mt/mtx.h>
//#include <zero/trix.h>
//#include <v0/mach32.h>
#include <voima/vm/vm.h>
#include <v0/bsp/papilio-pro.h>

#define V0_PC_LK_BIT        (INT32_C(1) << V0_PC_LK_BIT_OFS)
#define V0_PC_LK_BIT_OFS    1
#define V0_READ_BARRIER     1
#define V0_WRITE_BARRIER    2
#define V0_FULL_BARRIER     (V_READ_BARRIER | V0_WRITE_BARRIER)

/* op(dest, src1, src2) */
typedef void *            (*v0opfunc)(const void * C_RESTRICT dest,
                                      const void * C_RESTRICT src1,
                                      const void * C_RESTRICT src2);
typedef (*v0unitfunc)(struct v0vm *vm, const struct v0inst *inst);
v0unitfunc                 *vmunitfunctab[V0_UNITS];
v0opfunc                   *vmopfunctab[V0_UNITS][V0_UNIT_INSTRUCTIONS];
static const unsigned char  vmham32tab[256]
= {
    0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    1, 2, 2, 3, 2, 3, 3, 4, 2, 3, 3, 4, 3, 4, 4, 5,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    2, 3, 3, 4, 3, 4, 4, 5, 3, 4, 4, 5, 4, 5, 5, 6,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    3, 4, 4, 5, 4, 5, 5, 6, 4, 5, 5, 6, 5, 6, 6, 7,
    4, 5, 5, 6, 5, 6, 6, 7, 5, 6, 6, 7, 6, 7, 7, 8
};
static const unsigned char  vmctz32tab[32]
= {
    31, 0, 27, 1, 28, 13, 23, 2, 29, 21, 19, 14, 24, 16, 3, 7,
    30, 26, 12, 22, 20, 18, 15, 6, 25, 11, 17, 5, 10, 4, 9, 8
};
static const unsigned char  vmclz32tab[129]
= {
    1, 2, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5, 5,
    6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
    9
};
static uint8_t
struct v0vm                 g_vm C_ALIGNED(MACH_CL_SIZE);

#define v0setopfunc(tab, unit, op, func)                                \
    (v0opfunctab[(unit)][(op)] = (func))
void
v0initops(v0opfunc *tab[V0_UNITS][V0_UNIT_INSTRUCTIONS])
{
    /* LOGIC-unit */
    v0setopfunc(tab, V0_LOGIC_UNIT, V0_NOT_OP, v0_not32);
    v0setopfunc(tab, V0_LOGIC_UNIT, V0_ORR_OP, v0_orr32);
    v0setopfunc(tab, V0_LOGIC_UNIT, V0_XOR_OP, v0_xor32);
    v0setopfunc(tab, V0_LOGIC_UNIT, V0_AND_OP, v0_and32);
    v0setopfunc(tab, V0_LOGIC_UNIT, V0_NOR_OP, v0_nor32);
    v0setopfunc(tab, V0_LOGIC_UNIT, V0_XNOR_OP, v0_xnor32);
    v0setopfunc(tab, V0_LOGIC_UNIT, V0_NAND_OP, v0_nand32);
    /* SHIFT-unit */
    v0setopfunc(tab, V0_LOGIC_UNIT, V0_SLR_OP, v0_slr32);
    v0setopfunc(tab, V0_LOGIC_UNIT, V0_SAR_OP, v0_sar32);
    v0setopfunc(tab, V0_LOGIC_UNIT, V0_SLL_OP, v0_sll32);
    v0setopfunc(tab, V0_LOGIC_UNIT, V0_ROR_OP, v0_ror32);
    v0setopfunc(tab, V0_LOGIC_UNIT, V0_ROL_OP, v0_rol32);
    v0setopfunc(tab, V0_LOGIC_UNIT, V0_RCR_OP, v0_rcr32);
    v0setopfunc(tab, V0_LOGIC_UNIT, V0_RCL_OP, v0_rcl32);
    /* ADDER-unit */
    v0setopfunc(tab, V0_ADDER_UNIT, V0_INC_OP, v0_inc32);
    v0setopfunc(tab, V0_ADDER_UNIT, V0_UINC_OP, v0_uinc32);
    v0setopfunc(tab, V0_ADDER_UNIT, V0_DEC_OP, v0_dec32);
    v0setopfunc(tab, V0_ADDER_UNIT, V0_UDEC_OP, v0_udec32);
    v0setopfunc(tab, V0_ADDER_UNIT, V0_ADD_OP, v0_add32);
    v0setopfunc(tab, V0_ADDER_UNIT, V0_UADD_OP, v0_uadd32);
    v0setopfunc(tab, V0_ADDER_UNIT, V0_ADC_OP, v0_adc32);
    v0setopfunc(tab, V0_ADDER_UNIT, V0_SUB_OP, v0_sub32);
    v0setopfunc(tab, V0_ADDER_UNIT, V0_USUB_OP, v0_usub32);
    v0setopfunc(tab, V0_ADDER_UNIT, V0_SBB_OP, v0_sbb32);
    /* MULDIV-unit */
    v0setopfunc(tab, V0_MULDIV_UNIT, V0_MUL_OP, v0_mul32);
    v0setopfunc(tab, V0_MULDIV_UNIT, V0_UMUL_OP, v0_umul32);
    v0setopfunc(tab, V0_MULDIV_UNIT, V0_MHI_OP, v0_mhi32);
    v0setopfunc(tab, V0_MULDIV_UNIT, V0_UMHI_OP, v0_umhi32);
    v0setopfunc(tab, V0_MULDIV_UNIT, V0_DIV_OP, v0_div32);
    v0setopfunc(tab, V0_MULDIV_UNIT, V0_UDIV_OP, v0_udiv32);
    v0setopfunc(tab, V0_MULDIV_UNIT, V0_REM_OP, v0_rem32);
    v0setopfunc(tab, V0_MULDIV_UNIT, V0_UREM_OP, v0_urem32);
    v0setopfunc(tab, V0_MULDIV_UNIT, V0_IRP_OP, v0_irp32);
    v0setopfunc(tab, V0_MULDIV_UNIT, V0_UIRP_OP, v0_uirp32);
    /* BIT-unit */
    v0setopfunc(tab, V0_BIT_UNIT, V0_ZEXB_OP, v0_zexb32);
    v0setopfunc(tab, V0_BIT_UNIT, V0_ZEXW_OP, v0_zexw32);
    v0setopfunc(tab, V0_BIT_UNIT, V0_ZEX_OP, v0_zex32);
    v0setopfunc(tab, V0_BIT_UNIT, V0_SEXB_OP, v0_sexb32);
    v0setopfunc(tab, V0_BIT_UNIT, V0_SEXW_OP, v0_sexw32);
    v0setopfunc(tab, V0_BIT_UNIT, V0_SEX_OP, v0_sex32);
    v0setopfunc(tab, V0_BIT_UNIT, V0_NEG_OP, v0_neg32);
    v0setopfunc(tab, V0_BIT_UNIT, V0_PAR_OP, v0_par32);
    v0setopfunc(tab, V0_BIT_UNIT, V0_HAM_OP, v0_ham32);
    v0setopfunc(tab, V0_BIT_UNIT, V0_CTZ_OP, v0_ctz32);
    v0setopfunc(tab, V0_BIT_UNIT, V0_CLZ_OP, v0_clz32);
    v0setopfunc(tab, V0_BIT_UNIT, V0_HSH_OP, v0_hsh32);
    v0setopfunc(tab, V0_BIT_UNIT, V0_HUN_OP, v0_hun32);

    return;
}

struct v0vm *
v0initvm(struct v0vm *vm, int flg, size_t bramsize, size_t dramsize)
{
    void                       *ptr;

    v0initopfunctab();
    mtlkfmtx(&vm->mtx);
    if (flg & V0_VM_READY) {
        mtunlkfmtx(&vm->mtx);

        return vm;
    }
    ptr = calloc(bramsize, sizeof(int8_t));
    if (!ptr) {
        fprintf(stderr, "failed to allocate VM BRAM...\n");
        abort();
    }
    vm->bram = ptr;
    vm->bramsize = bramsize;
    ptr = calloc(dramsize, sizeof(int8_t));
    if (!ptr) {
        fprintf(stderr, "failed to allocate VM DRAM...\n");
        abort();
    }
    vm->mem = ptr;
    vm->memsize = dramsize;
    vm->flg |= V0_VM_READY;
    mtunlkfmtx(&vm->mtx);

    return vm;
}

/*
 * - return fetched instruction word/structure
 * - store pointer to possible immediate operand via immret
 */
static const struct v0inst
v0fetchinst(struct v0vm * C_RESTRICT vm,
            void **immret)
{
    int32_t                     pc = vm->genregs[V0_PC_REGISTER];
    const struct v0inst        *iptr = &vm->mem[pc];
    int_fast8_t                 parm = iptr->parm;
    int8_t                      immsft;
    int8_t                     *immptr;
    int8_t                      immsz;

    pc += sizeof(struct v0inst);
    if (parm & V0_PARM_IMM) {
        immsft = parm & V0_PARM_IMM_SIZE_MASK;
        immptr = &iptr->imm32;
        immsz = 1 << immsft;
        if (imm && immsz <= 4) {                    // immediate fits in 32 bits
            pc += sizeof(int32_t);                  // adjust instruction size
        } else if (parm & V0_PARM_IMM_ALN) {        // aligned by 32 bits to 64
            immptr += sizeof(int32_t);              // align immediate operand
            pc += sizeof(int32_t) + sizeof(int64_t); // adjust instruction size
        } else {                                    // 64-bit aligned
            pc += sizeof(int64_t);                  // adjust instruction sizeM
        }
        *(void *)immret = immptr;                   // store immediate address
    } else if (parm) {
        *(void *)immret = NULL;                     // no imm32/16/8/64 present
    }
    vm->genregs[V0_PC_REGISTER] = pc;               // adjust program counter

    return *iptr;
}

/* decode source address */
static C_INLINE void *
v0decsrcadr(struct v0vm * C_RESTRICT vm,
            struct v0inst * C_RESTRICT inst,
            void * C_RESTRICT immptr)
{
    int_fast8_t             parm = inst->parm;
    int_fast8_t             dest = inst->dest;
    int_fast8_t             src1 = inst->src;
    int_fast8_t             src2 = inst->src2;
    int8_t                 *ptr = NULL;
    void                   *s1ptr = &vm->genregs[src1];
    void                   *s2ptr = &vm->genregs[src2];

    /* address operand */
    if (parm & V0_PARM_ADR_BASE) {
        ptr += *(uint32_t *)s1ptr;
    }
    if ((src2) && parm & V0_PARM_ADR_NDX) {
        ptr += *(uint32_t *)s2ptr;
    }
    if (parm & V0_PARM_ADR_OFS) {
        ptr += *(int32_t *)immptr;
    }

    return ptr;
}

static C_INLINE void *
v0decdestadr(struct v0vm * C_RESTRICT vm,
             struct v0inst * C_RESTRICT inst,
             void * C_RESTRICT immptr)
{
    int_fast8_t             parm = inst->parm;
    int_fast8_t             dest = inst->dest;
    int_fast8_t             src1 = inst->src;
    int_fast8_t             src2 = inst->src2;
    void                   *dptr = &vm->genregs[dest];
    int8_t                 *ptr = NULL;
    void                   *s1ptr = &vm->genregs[src1];

    /* address operand */
    if (parm & V0_PARM_ADR_BASE) {
        ptr += *(uint32_t *)dptr;
    }
    if (parm & V0_PARM_ADR_NDX) {
        ptr += *(uint32_t *)s1ptr;
    }
    if (parm & V0_PARM_ADR_OFS) {
        ptr += *(int32_t *)immptr;
    }

    return ptr;
}

static struct v0inst
v0decinst(struct v0vm * C_RESTRICT vm,
          struct v0inst * C_RESTRICT inst)
{
    do {
        const void             *immptr;
        size_t                  immsize;
        const struct v0inst     inst = v0fetchinst(vm, &immptr);
        void                   *dptr;
        void                   *s1ptr;
        void                   *s2ptr;
        int_fast8_t             dest = inst.dest;
        int_fast8_t             src1 = inst.src1;
        int_fast8_t             src2 = inst.src2;
        int_fast8_t             unit = inst.unit;
        int_fast9_t             op = inst.op;
        int_fast8_t             parm = inst.parm;
        int_fast8_t             fold;
        void                   *ptr;

        func = v0opfunctab[unit][op];
        dptr = &vm->genregs[dest];
        s1ptr = &vm->genregs[src1];
        s2ptr = &vm->genregs[src2];
        if (unit == V0_LOGIC_UNIT) {
            if (!src1) {
                s1ptr = dptr;
            }
            if (inst.cond) {
                cond = vm->genregs[src2];
                if (cond == V0_ANY_COND) {
                } else {
                }
            }
        } else if (unit >= V0_BRANCH_UNIT && unit <= V0_ATOMIC_UNIT) {
            if (parm) {
                if (parm & V0_PARM_ADR_MASK) {
                    if (parm & V0_PARM_SRC_ADR) {
                        s1ptr = v0decsrcadr(vm, inst, immptr);
                        if (!s1ptr) {
                            v0crash("missing source-address operand for %d(%d)\n",
                                    op, unit);
                        }
                    } else {
                        dptr = v0decdestadr(vm, inst, immptr);
                        if (!dptr) {
                            v0crash("missing destination-address operand for %d(%d)\n",
                                    op, unit);
                        }
                    }
                }
            }
            func(dptr, s1ptr, s2ptr);
        } else if (parm) {
            if (parm & V0_PARM_SHIFT_MASK) {
                parm &= V0_PARM_SHIFT_MASK;
                if (unit == V0_SHIFT_UNIT) {
                    /* shift or rotate instruction */
                    func(dptr, s1ptr, &parm);
                } else {
                    /* instruction folded with a shift */
                    int32_t     val;
                    uint32_t    uval;

                    fold = parm & V0_PARM_FOLD_MASK;
                    func(val, s1ptr, s2ptr);
                    uval = *(uint32_t *)&val;
                    if (fold == V0_PARM_FOLD_SLL) {
                        uval <<= parm;
                        *(uint32_t *)dptr = uval;
                    } else if (fold == V0_PARM_FOLD_SRL) {
                        uval >>= parm;
                        *(uint32_t *)dptr = uval;
                    } else if (fold == V0_PARM_FOLD_SAR) {
                        val >>= parm;
                        *(int32_t *)dptr = val;
                    } else if (fold == V0_PARM_FOLD_ROR) {
                        int32_t     lo = uval >> parm;

                        uval <<= -parm & 31;
                        uval |= lo;
                        *(uint32_t *)dest = uval;
                    }
                }
            } else {
                parm &= V0_PARM_COND_MASK;
                switch (parm) {
                    case V0_ANY_COND:
                        func(dptr, s1ptr, s2ptr);

                        break;
                    case V0_EQ_COND:
                        if (vm->sysregs[V0_MSW_REGISTER] & V0_MSW_ZF_BIT) {
                            func(dptr, s1ptr, s2ptr);
                        }

                        break;
                    case V0_NE_COND:
                        func(dptr, s1ptr, s2ptr);

                        break;
                    case V0_GT_COND:
                        func(dptr, s1ptr, s2ptr);

                        break;
                    case V0_LT_COND:
                        func(dptr, s1ptr, s2ptr);

                        break;
                    case V0_GE_COND:
                        func(dptr, s1ptr, s2ptr);

                        break;
                    case V0_LE_COND:
                        func(dptr, s1ptr, s2ptr);

                        break;
                    case V0_HS_COND:
                        func(dptr, s1ptr, s2ptr);

                        break;
                    case V0_LO_COND:
                        func(dptr, s1ptr, s2ptr);

                        break;
                    case V0_MI_COND:
                        func(dptr, s1ptr, s2ptr);

                        break;
                    case V0_PL_COND:
                        func(dptr, s1ptr, s2ptr);

                        break;
                    case V0_OF_COND:
                        func(dptr, s1ptr, s2ptr);

                        break;
                    case V0_NO_COND:
                        func(dptr, s1ptr, s2ptr);

                        break;
                    case V0_HI_COND:
                        func(dptr, s1ptr, s2ptr);

                        break;
                    case V0_LS_COND:
                        func(dptr, s1ptr, s2ptr);

                        break;
                }
            }
        } else {
            func(dptr, s1ptr, s2ptr);
        }
    } while (!vm->status);

    exit(vm->status);
}

static struct v0inst
v0procinst(struct v0vm *vm, const struct v0inst inst)
{
    int_fast32_t                unit = inst.unit;
    int_fast32_t                dest = inst.dest;
    int_fast32_t                src1 = inst.src1;
    int_fast32_t                src2 = inst.src2;
}

