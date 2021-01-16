#! /bin/sh

OBJECTS=""
CSOURCES=""

vsht_set_files()
{
    srcext=".c"
    objext=".o"

    for prog in $TARGETS
    do
	    for srcname in $SOURCES
	    do
	        srcfile="$srcname$srcext"
	        objfile="$srcname$objext"
	        if [ -z $OBJECTS ]; then
		        OBJECTS="$objfile"
	        else
		        OBJECTS="$OBJECTS $objfile"
	        fi
	        if [ -z $CSOURCES ]; then
		        CSOURCES="$srcfile"
	        else
		        CSOURCES="$CSOURCES $srcfile"
	        fi
	    done
    done

    return;
}

vsht_build()
{
    if [ "vsht_opt_debug_vsht" -ne 0 ]; then
	    echo -e "CSOURCES\t$CSOURCES"
	    echo -e "OBJECTS\t$OBJECTS"
    fi
    vshtfile="${prog}.vsht"
    if [ -f ./$vshtfile ]; then
	    . ./$vshtfile
    fi
    vsht_set_files $@
    for prog in $TARGETS
    do
	    for src in $CSOURCES
	    do
	        $CC $CFLAGS $CCWARNFLAGS $CCOPTFLAGS $INCLUDES -c "$src"
	    done
	    $LD $LDFLAGS -o "$prog" $OBJECTS
    done

    return
}

set -x
echo "CMD"
. vsht.conf
. vsht-opt.sh
vsht_get_opts $@
. vsht-env.sh $@
. vsht-ctools.sh
vsht_set_files $@
vsht_build $@

