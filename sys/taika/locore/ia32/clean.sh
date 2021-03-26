#! /bin/sh

cleanfiles="*.o *.sym *~ zero *.s *.i *.bc *.log"
cleandirs="zeroiso build grub"

optfiles=""
optdir=""
optforce=""
optrecursive=""

wipe()
{
    rmopts="-rf"

    rm $rmopts $cleanfiles $cleandirs

    return 0
}

getopt()
{
    for opt in
    do
	case $opt in
	    -C)
		$optforce=1
                $optrecursive=1
		;;
	    -F)
		$optforce=1
		;;
	    -R)
                $optrecursive=1
		;;
	    *)
		if [ test -e "$opt" ] && [ test -nz "$optdir" ]; then
		    echo "argument $opt follows directory $optdir"

		    exit 1
		fi
		if [ test -f "$opt" ]; then
		    if [ test -z "$optfiles" ]; then
			optfiles="$opt"
		    else
			optfiles="$optfiles $opt"
		    fi
		elif [ test -d "$opt" ]; then
		    optdir="$opt"
		fi
	esac
    done
}

getopt $@
wipe

