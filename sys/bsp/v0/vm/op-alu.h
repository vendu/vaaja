#ifndef __SYS_V0_OP_H__
#define __SYS_V0_OP_H__

#define m_sex32(x, max)         (((x) ^ (max)) - (max))
#define m_sex8(b)               (((b) ^ 128) - 128)
#define m_sex16(b)              (((b) ^ 65536) - 128)

#endif /* __SYS_V0_OP_H__ */

