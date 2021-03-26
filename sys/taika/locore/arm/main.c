#if defined(__arm__)

void
kmain(void) {

    while (1) {
        m_waitint();
    }

    /* NOTREACHED */
}

#endif /* __arm__ */

