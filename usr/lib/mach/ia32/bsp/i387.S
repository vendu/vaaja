/*
 * implementation of sqrt() for the i387 as written by R on stackoverflow.com :)
 * - added some speedup hacks and comments // vendu
 */

 .globl	__fpusqrt

/* FIXME: make sure the stack adjustments etc. are correct */
 __fpusqrt:
    fldl 4(%esp)        // load 64-bit argument to top of FPU stack
    fsqrt               // compute square root
    fstsw %ax           // store FPU status-word in %ax
    subl $12,%esp       // sp -= 12;
    fldl (%esp)         // load result high word into %st(0)
    fstpt (%esp)        // push square root to argument memory
    movl (%esp),%ecx    // move high word of result to %ecx
    movl (%esp), %edx   // move high word of result to %edx
    andl $0x7ff,%ecx    // reshi &= 0x7ff;
    cmpl $0x400,%ecx    // (reshi == 0x400)
    jnz 1f              //  ? goto 1
    andl $0x200,%eax    //  : fpusw &= 0x200
    subl $0x100,%eax    //    fpusw -= 0x100
    subl %eax, %edx     //    reshi -= fpusw
    fstp %st(0)         //    push %st(0)
    fldt (%esp)         //    load res into %st(0)
1:
    fstpl 16(%esp)      // push %st(0) to result
    fldl 16(%esp)       // reload %st(0) (return value?)
    addl $12, %esp      // sp += 12;
    ret

