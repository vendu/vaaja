#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mt/mtx.h>
#if (SHUNTC)
#include <zero/shuntc.h>
#endif
#include <zero/shunt.h>

/*
 * Dijkstra's shunting yard algorithm
 * - turns infix-format expressions into RPN queues
 * - https://en.wikipedia.org/wiki/Shunting-yard_algorithm
 */
SHUNT_TOKEN *
shuntparse(SHUNT_TOKEN *tokqueue)
{
    SHUNT_TOKEN *tok;
    SHUNT_TOKEN *tok1 = tokqueue;
    SHUNT_TOKEN *tok2 = NULL;
    SHUNT_TOKEN *tok3 = NULL;
    SHUNT_TOKEN *queue = NULL;
    SHUNT_TOKEN *tail = NULL;
    SHUNT_TOKEN *stack = NULL;

    while (tok1) {
//        zpcprinttok(tok1);
        tok = malloc(sizeof(SHUNT_TOKEN));
        tok3 = tok1->next;
        memcpy(tok, tok1, sizeof(SHUNT_TOKEN));
        tok = tok1;
        if (shuntcisvalue(tok)) {
            shuntqueue(tok, &queue, &tail);
        } else if (shuntcisfunc(tok)) {
            shuntpush(tok, &stack);
        } else if (shuntcissep(tok)) {
            tok2 = stack;
            while ((tok2) && tok2->type != SHUNT_LEFTPAREN) {
                tok2 = shuntpop(&stack);
                shuntqueue(tok2, &queue, &tail);
                tok2 = shuntpop(&stack);
            }
            if ((tok2) && tok2->type == SHUNT_LEFTPAREN) {

                continue;
            } else {
                fprintf(stderr, "mismatched parentheses: %s\n", tok2->str);

                return NULL;
            }
        } else if (shuntcisop(tok)) {
            tok2 = stack;
            while (shuntcisop(tok2)) {
                if ((!shuntcisrtol(tok)
                     && shuntcopprec(tok) <= shuntcopprec(tok2))
                    || shuntcopprec(tok) < shuntcopprec(tok2)) {
                    tok2 = shuntpop(&stack);
                    shuntqueue(tok2, &queue, &tail);
                    tok2 = stack;
                } else {

                    break;
                }
            }
            shuntpush(tok, &stack);
        } else if (tok->type == SHUNT_LEFTPAREN) {
            shuntpush(tok, &stack);
        } else if (tok->type == SHUNT_RIGHTPAREN) {
            tok2 = stack;
            while ((tok2) && tok2->type != SHUNT_LEFTPAREN) {
                tok2 = shuntpop(&stack);
                shuntqueue(tok2, &queue, &tail);
                tok2 = stack;
            }
            if ((tok2) && tok2->type == SHUNT_LEFTPAREN) {
                tok2 = shuntpop(&stack);
            } else {
                if (tok2) {
                    fprintf(stderr, "mismatched parentheses: %s\n",
                            tok2->str);
                }

                return NULL;
            }
            if (shuntcisfunc(stack)) {
                tok2 = shuntpop(&stack);
                shuntqueue(tok2, &queue, &tail);
            }
        }
        tok1 = tok3;
    }
    do {
        tok1 = stack;
        if (shuntcisop(tok1)) {
            tok1 = shuntpop(&stack);
            shuntqueue(tok1, &queue, &tail);
        } else if ((tok1)
                   && (tok1->type == SHUNT_LEFTPAREN
                       || tok1->type == SHUNT_RIGHTPAREN)) {
            fprintf(stderr, "mismatched parentheses: %s\n", tok1->str);

            return NULL;
        }
    } while (stack);

    return queue;
}

SHUNT_TOKEN *
shunteval(SHUNT_TOKEN *tokqueue)
{
    SHUNT_TOKEN *tok = tokqueue;
    SHUNT_TOKEN *queue = NULL;
    SHUNT_TOKEN *tail = NULL;
    SHUNT_TOKEN *stack = NULL;
    SHUNT_TOKEN *tok1 = tok;
    SHUNT_TOKEN *tok2;
    SHUNT_TOKEN *arg1;
    SHUNT_TOKEN *arg2;
    SHUNT_INT    dest;
    SHUNT_OP    *func;
    long         radix;

    while (tok) {
        tok2 = tok->next;
        if (shuntcisvalue(tok)) {
            shuntpush(tok, &stack);
        } else if (shuntcisop(tok)) {
            if (!tok1) {
                fprintf(stderr, "missing argument 1\n");

                return NULL;
            }
            arg2 = NULL;
            if (SHUNT_NARGTAB[tok->type] == 2) {
                arg2 = shuntpop(&stack);
                if (!arg2) {
                    fprintf(stderr, "missing argument 2\n");

                    return NULL;
                }
            }
            arg1 = tok1;
#if (SHUNTC) && 0
            if (arg1) {
                shuntcprinttok(arg1);
            }
            if (arg2) {
                shuntcprinttok(arg2);
            }
#endif /* SHUNTC */
            switch (SHUNT_NARGTAB[tok->type]) {
                case 2:
                    if (!arg2) {
                        fprintf(stderr, "invalid argument 2\n");

                        return NULL;
                    }
                case 1:
                    if (!arg1) {
                        fprintf(stderr, "invalid argument 1\n");

                        return NULL;
                    }
                default:
                    fprintf(stderr, "invalid number of arguments\n");

                    return NULL;
            }
            func = SHUNT_EVALTAB[tok->type];
            if (func) {
                if (arg2) {
                    dest = func(arg2, arg1);
                    if (arg1->radix == SHUNT_HEX
                        || arg2->radix == SHUNT_HEX) {
                        tok1->radix = SHUNT_HEX;
                    } else if (arg1->radix == SHUNT_OCT
                               || arg2->radix == SHUNT_OCT) {
                        tok1->radix = SHUNT_OCT;
                    } else if (arg1->radix == SHUNT_BIN
                               || arg2->radix == SHUNT_BIN) {
                        tok1->radix = SHUNT_BIN;
                    } else {
                        tok1->radix = SHUNT_DEC;
                    }
#if defined(SHUNTSMARTTYPES)
                    tok1->type = arg1->type;
                    tok1->flg = arg1->flg;
                    tok1->sign = arg1->sign;
#endif
                } else if (arg1) {
                    dest = func(arg1, arg2);
                    tok1->radix = arg1->radix;
#if defined(SHUNTSMARTTYPES)
                    tok1->type = arg2->type;
                    tok1->flg = arg2->flg;
                    tok1->sign = arg2->sign;
#endif
                }
                tok1->data.i64 = dest;
                if (arg1->type == SHUNT_INT64 || arg1->type == SHUNT_UINT64) {
                    radix = tok1->radix;
                    if (!radix) {
                        radix = SHUNT_RADIX;
                    }
                    tok1->radix = radix;
#if (SHUNTC)
                    shuntprintstr(tok1, tok1->data.ui64, radix);
#endif
                }
            }
        }
        tok = tok2;
    }
    shuntqueue(tok1, &queue, &tail);

    return queue;
}

void
shuntprintbin(SHUNT_UINT val, char *str, long len)
{
    long       l;
#if (SHUNT_INTSIZE == 64)
    SHUNT_UINT mask = UINT64_C(1) << 63;
#elif (SHUNT_INTSIZE == 32)
    SHUNT_UINT mask = UINT32_C(1) << 31;
#endif

    if (len < SHUNT_INTSIZE + 4) {
        fprintf(stderr, "not enough room for %d bits\n", SHUNT_INTSIZE);

        return;
    }
    sprintf(str, "0b");
    for (l = 2 ; l < SHUNT_INTSIZE + 2 ; l++) {
        sprintf(&str[l], "%c", (val & mask) ? '1' : '0');
        mask >>= 1;
    }
    str[l] = '\0';

    return;
}

