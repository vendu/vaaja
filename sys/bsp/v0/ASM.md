# Zen Kernel Machine Assembly Interface

## int32_t m_atomread(m_atomic_t *ap);
- read atomic variable
## void m_atomwrite(m_atomic_t *ap, int32_t i);
- write atomic variable
## void m_atomadd(m_atomic_t *ap, int32_t i);
- add integer to atomic variable
## void m_atomsub(m_atomic_t *ap, int32_t i);
- subtract integer from atomic variable
## void m_atominc(m_atomic_t *ap);
- increment atomic variable
## void m_atomdec(m_atomic_t *ap);
- decrement atomic variable
## long m_atomchkaddneg(m_atomic_t *ap, int32_t i);
- atomically add i to *ap and return TRUE if result is negative, FALSE if >= 0
## long m_atomchksub(m_atomic_t *ap, int32_t i);
- atomically add i to *ap and return TRUE if result is zero, FALSE otherwise
## long m_atomchkinc(m_atomic_t *ap);
## long m_atomchkdec(m_atomic_t *ap);
## long m_atomaddret(m_atomic_t *ap, int32_t i);
- atomically add i to *ap and return result
## long m_atomsubret(m_atomic_t *ap, int32_t i);
- atomically subtract i from *ap and return result
## long m_atomtryadd(m_atomic_t *ap, int i, int v);
- atomically add a to *ap iff *ap != v. return old *ap.

