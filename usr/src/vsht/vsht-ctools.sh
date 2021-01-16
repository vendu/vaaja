#! /bin/sh

CFLAGS+="-g"
LDFLAGS+="-r"
CCOPTFLAGS="-O"
CCWARNFLAGS="-D_FORTIFY_SOURCE=2 -Wall -Wundef -Wpointer-arith -Wuninitialized -Wlong-long -Winit-self -Wparentheses -Wswitch-default -Wswitch-bool -Wunused-parameter -Wstrict-aliasing -Wstrict-overflow -Wfloat-equal -Wshadow -Wbad-function-cast -Wcast-align -Wwrite-strings -Wdate-time -Waggregate-return -Wstrict-prototypes -Wpacked -Wpadded -Wredundant-decls -Wnested-externs -Winline -Wvla -Wstack-protector -Woverlength-strings -Wdeclaration-after-statement -Wformat-security -Wmissing-format-attribute"
GNUWARNFLAGS="-Wtrampolines -Wstack-usage=16384 -Wunsafe-loop-optimizations -Wc99-c11-compat -Wjump-misses-init -Wlogical-op -Wnormalized -Wno-designated-init -Wsuggest-attribute=const -Wsuggest-attribute=pure -Wsuggest-attribute=noreturn -Wsuggest-attribute=format"
GNULDFLAGS=""
#CLANGCFLAGS="--dynamicbase --forceinteg --no-trampoline -fansi-escape-codes -fcolor-diagnostics -fdebug-macro"
CLANGCFLAGS="--dynamicbase --no-trampoline -fansi-escape-codes -fcolor-diagnostics -fdebug-macro"
CLANGOPTFLAGS="-fvectorize -fzvector -fopenmp -fshort-enums -freciprocal-math -fsave-optimization-record -fstack-size-section -mno-stack-arg-probe "
CLANGANALYZEFLAGS="-fcoverage-mapping --analyze --analyzer-output text"
CLANGPROFILEFLAGS="-fcs-profile-generate=.clang -fdebug-info-for-profiling"

vsht_set_ctools()
{
    if [ -n "$vsht_opt_cc" ]; then
	    if [ -x "$vsht_opt_cc" ]; then
	        CC="$vsht_opt_cc"
	    else
	        echo "compiler $vsht_opt_cc not executable"

	        exit 1
	    fi
    fi
    if [ -n "$vsht_opt_as" ]; then
	    if [ -x "$vsht_opt_as" ]; then
	        AS="$vsht_opt_as"
	    else
	        echo "assembler $vsht_opt_as not executable"

	        exit 1
	    fi
    fi
    if [ -n "$vsht_opt_ld" ]; then
	    if [ -x "$vsht_opt_ld" ]; then
	        LD="$vsht_opt_ld"
	    else
	        echo "linker $vsht_opt_ld not executable"

	        exit 1
	    fi
    fi

    return
}

# try to pick c compiler, assembler, and linker
vsht_check_ctools()
{
    if [ -z "$CC" ]; then
	    CC=cc
	    AS=cc
	    LD=cc
    fi
    gccver=`$CC --version | grep -i gcc`
    clangver=`$CC --version | grep -i clang`
    if [ -n "$gccver" ]; then
	    CC=gcc
    elif [ -n "$clangver" ]; then
	    CC="clang"
    fi
    gasver=`$AS --version | grep -i gnu`
    clangas=`$AS --version | grep -i clang`
    gnuld=`$LD --version | grep -i gnu`
    if [ -n "$gnuld" ]; then
	    LDFLAGS="$GNULDFLAGS"
    fi
    if [ -n "$gasver" ]; then
	    AS="gas"
	    LD="ld"
    elif [ -n "clangas" ]; then
	    AS="clang -integrated-as"
	    LD="clang"
    elif [ $CC = "clang" ]; then
	    CC="clang -no-integrated-as"
	    AS="$CC"
	    LD="«CC"
    fi

    return
}

vsht_conf_ctools()
{
    if [ $CC = "gcc" ]; then
	    CCWARNFLAGS="$CCWARNFLAGS $GNUWARNFLAGS"
	    LDFLAGS="$GNULDFLAGS"
    elif [ $CC = "clang" ]; then
	    CFLAGS="$CFLAGS $CLANGCFLAGS"
	    CCWARNFLAGS=$CCWARNFLAGS $CLANGWARNFLAGS
	    CCOPTFLAGS="$CCOPTFLAGS $CLANGOPTFLAGS"
	    CCDEBUGFLAGS="$CLANGDEBUGFLAGS"
	    if [ "$vsht_opt_analyze" -ne 0 ]; then
	        CCANALYZEFLAGS="$CLANGANALYZEFLAGS"
	    fi
	    if [ "$vsht_opt_profile" -ne 0 ]; then
	        CCPROFILEFLAGS="$CLANGPROFILEFLAGS"
	    fi
    fi

    return
}

vsht_print_ctools()
{
    echo -e "tool options"
    echo -e "------------"
    echo -e "general flags"
    echo -e "\t$CFLAGS $CCOPTFLAGS"
    echo -e "warning flags"
    echo -e "\t$CCWARNFLAGS"
    echo -e "analysis flags"
    echo -e "\t$CCANALYZEFLAGS"
    echo -e "profiling flags"
    echo -e "\t$CCPROFILEFLAGS"

    return
}

set -x
vsht_set_ctools $@
vsht_check_ctools $@
vsht_conf_ctools $@
vsht_print_ctools

