#include <voima/num/fenv.h>

const fenv_t                    __fe_dfl_env
= {
    FENV_DEF_CTRLWORD,
    0x0000,
    0x0000,
    FENV_DEF_MXCSR,
    0xffffffff,
    {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff
    }
};

int
fesetexceptflag(const fexcept_t *fp, int exc)
{
    fenv_t                      env;
    uint32_t                    mxcsr;
}

