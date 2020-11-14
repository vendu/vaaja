void
v0aluop(struct v0vm * C_RESTRICT vm, struct v0inst * C_RESTRICT inst)
{
    ;
}

void
v0procinst(struct v0vm * C_RESTRICT vm)
{
    int32_t                     pc = vm->intregs[V0_PC_REG];
    struct v0inst              *inst = (struct v0inst *)&vm->mem[pc];

}
