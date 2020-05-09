#! /bin/sh

# script name
PROG_NAME="progtool.sh"
# set locale if not set
if [[ -z "$LANG" ]]; then
    LANG="en_US.ISO8859-1"
fi

# error codes
EOKAY=0
EMSG=0
EINFO=0
EDEBUG=1
EFAULT=2
EABORT=3
EERROR=4

# configurable variables
PROGTOOL_REPO_URIS="https://github.com/gittup/tup.git https://github.com/libusb/libusb.git"
PROGTOOL_DEST_DIRS="bin sbin lib libexec man etc com var include share info locale man"

#initialised variables
PROGTOOL_BUILD_ROOT=`pwd`

# runtime variables
progtool_root_path="$PROGTOOL_BUILD_ROOT"
progtool_build_path=""
progtool_repo_uris="https://github.com/vendu/OS-Zero.git"
progtool_repo_list=""
progtool_option_flags=""
progtool_build_option=0
progtool_clean_option=0
progtool_default_option=0
progtool_verbose_option=0
progtool_force_option=0
progtool_dry_run_option=0
progtool_quiet_option=0
progtool_help_option=0
progtool_info_option=0
progtool_exec_prefix=""
progtool_cross_build_option=0

# print help message, exit if -h or --help not used
progtool_print_usage()
{
    if [[ $progtool_help_option -eq 0 ]]; then
 	echo "invalid command: $@"
    fi
    echo ""
    echo "usage:	$PROG_NAME [options] [paths]"
    echo ""
    echo "options"
    echo "-------"
    echo "-B, --build		build programs"
    echo "-C, --clean		remove empty build directories"
    echo "-D, --default		build default packages"
    echo "-v, --verbose		verbose mode"
    echo "-f, --force		force operations"
    echo "-n, --dry-run		list what would be done without executing commands"
    echo "-q, --quiet		quiet, no output messages"
    echo "-h, --help		this help screen"
    echo "-U, --uri		repository to build"
    echo "-I, --info		build information screen"
    echo "-E, --exec-prefix	exec prefix, e.g. for -X"
    echo "-X, --cross-build	cross-build, i.e. cross-compilation"
    echo ""

    exit 0
}

# print debug message, execute cmd arg, and exit or return error code
# progtool <message> <level> <cmd>
progtool_debug_event()
{
    msg="$1"
    lvl="$2"
    cmd="$3"
    arg="$4"
    val=""

    if [[ -n "$cmd" ]]; then
	if [[ -n "$arg" ]]; then
	    "'$cmd'" $arg
	else
	    "'$cmd'"
	fi
	eval val="'$?'"
    fi
    if [[ -n "$val" ]] && [[ "$val" == "$lvl" ]]; then
        echo "$msg"
    else
	case $val in
	    0)
		echo "$msg"
		val=$EOKAY
		;;
  	    msg)	    
		echo "$msg"
		val=$EOKAY
		;;
	    info)
		echo "INFO	$msg"
		val=$EINFO
		;;
	    warn)
		echo "WARN	$msg"
		val=$EMSG
		;;
	    debug)
		echo "DEBUG	$msg"
		val=$EDEBUG
		;;	
	    fault)
		echo "FAULT	$msg"
		val=$EFAULT
		;;
	    abort)
		echo "ABORT	$msg"
		exit $EABORT
		;;
	    error)
		echo "ERROR	$msg"
		exit $EERROR
		;;
	esac
    fi
    eval ret="'$val'"

    return $ret
}

# print [info] message
progtool_print_msg()
{
    ret=0
    shift
    str=$@

    if [[ $progtool_dry_run_option -ne 0 ]]; then
	echo "$str"
    elif [[ $progtool_verbose_option -ne 0 ]]; then
	echo "$str"
    elif [[ $progtool_quiet_option -ne 0 ]]; then
	echo "$str" >> progtool.log
    fi
    eval ret="'$?'"
    
    return $ret
}

# handle command-line options
progtool_get_opts()
{
    while [ $# -gt 0 ]
    do
	case $1 in
	    -B | --build)
		progtool_build_option=1
		progtool_option_flags="'$progtool_option_flags'B"
		shift
		;;
	    -C | --clean)
		progtool_clean_option=1
		progtool_option_flags="'$progtool_option_flags'C"
		shift
		;;
	    -D | --default)
		progtool_default_option=1
		progtool_option_flags="'$progtool_option_flags'D"
		shift
		;;
	    -v | --verbose)
		progtool_verbose_option=1
		progtool_option_flags="'$progtool_option_flags'V"
		shift
		;;
	    -f | --force)
		progtool_force_option=1
		progtool_option_flags="'$progtool_option_flags'f"
		shift
		;;
	    -n | --dry-run)
		progtool_dry_run_option=1
		progtool_option_flags="'$progtool_option_flags'n"
		shift
		;;
	    -q | --quiet)
		progtool_quiet_option=1
		progtool_option_flags="'$progtool_option_flags'Q"
		shift
		;;
	    -h | --help)
		progtool_help_option=1
		progtool_print_usage $@
		shift
		;;
	    -U | --uri | --url)
		shift
		progtool_uri_options="$progtool_uri_options $1"
		progtool_option_flags="'$progtool_option_flags'U"
		shift
		;;
	    -I | --info)
		progtool_info_option=1
		progtool_option_flags="'$progtool_option_flags'I"
		shift
		;;
	    -E | --exec-prefix)
		shift
		progtool_exec_prefix="$1"
		progtool_option_flags="'$progtool_option_flags'E"
		shift
		;;
	    -X | --cross-build)
		progtool_cross_build_option=1
		progtool_option_flags="'$progtool_option_flags'X"
		shift
		;;
	    *)
		if [[ -d "$1" ]]; then
		    if [[ "$progtool_root_path" == "$PROGTOOL_BUILD_ROOT" ]];
		    then
			progtool_root_path="$1"
			progtool_option_flags="'$progtool_option_flags'r"
		    fi
		else
		    progtool_uri_options="$progtool_uri_options $1"
		    progtool_option_flags="'$progtool_option_flags'wu"
		fi
		shift
		;;
	esac
    done
     
    return
}

# execute command
progtool_run_cmd()
{
    cmd="$1"
    ret=0

    if [[ $progtool_dry_run_option -ne 0 ]]; then
	echo '$cmd'
    elif [[ $progtool_quiet_option -ne 0 ]]; then
	ret=`$cmd 2> /dev/null`
    else
	if [[ $progtool_verbose_option -ne 0 ]]; then
	    echo "$cmd"
	fi
	ret=`$cmd`
    fi

    return $ret
}

# construct list of repositories to update/build
progtool_create_repo_list()
{
    root="$progtool_root_path"
    uris=$@

    cd "$root"
    echo "ROOT: $root, URIS: $uris"
    if [[ -n "$uris" ]]; then
	for uri in $uris
	do
	    path=`basename $uri`
	    dir=`echo $path | sed -e 's/'"\.git"'\$//'`
	    progtool_repo_list="$progtool_repo_list $uri"
	    progtool_repo_dirs="$progtool_repo_dirs $dir"
	done
    fi
    cd "$root"

    return
}

# remove directories and their files recursively
progtool_clean_dirs()
{
    root="$progtool_root_path"
    dirs=$@

    echo "CLEAN: $dirs" 
    cd "$root"
    if [[ -n "dirs" ]]; then
	for dir in $dirs
	do
	    progtool_run_cmd "rm -rf $dir"
	done
    fi
    cd "$root"

    return
}

# remove empty directories
progtool_remove_dirs()
{
    root="$progtool_root_path"
    dirs=$@

    echo "CLEAN: $dirs"
    cd "$root"
    if [[ -n "dirs" ]]; then
	for dir in $dirs
	do
	    progtool_run_cmd `eval "rmdir $dir 2> /dev/null"`
	done
    fi
    cd "$root"

    return
}

# build programs in argument paths
progtool_build_progs()
{
    root="$progtool_root_path"
    dirs=$@
    ret=0

    cd "$root"
    if [[ -n $dirs ]]; then
	for dir in $dirs
	do
	    progtool_print_msg "error building $progtool_root_path/$dir"
	    progtool_build_auto "$dir"
	    eval ret="'$?'"
	done
    fi
    cd "$root"

    return $ret
}

# update git reposity
git_update_repo()
{
    uri="$1"
    dir="$2"
    ret=0

    progtool_print_msg "cloning $uri into $progtool_root_path/$dir"
    progtool_run_cmd `eval 'git clone "$uri"'`
    eval ret="'$?'"
    if [[ $ret -ne 0 ]] && [[ -d "$dir" ]]; then
	progtool_print_msg "updating $progtool_root_path/$dir"
	progtool_run_cmd `eval 'cd "$dir"'`
	progtool_run_cmd "git pull"
	eval ret="'$?'"
	if [[ $ret -ne 0 ]]; then
	    progtool_print_msg "failed to update $uri in $progtool_root_path/$dir"
	fi
    fi
    
    return $ret
}

# update mercurial repository
hg_update_repo()
{
    root="$progtool_root_path"
    uri="$1"
    dir="$2"
    ret=0
    
    progtool_print_msg "cloning $uri in $progtool_root_dir/$path"
    progtool_run_cmd `eval hg clone "$uri"`
    eval ret=="'$?'"
    if [[ $ret -ne 0 ]] && [[ -d "$dir" ]]; then
	progtool_run_cmd `eval cd "$dir"`
	progtool_print_msg "updating $progtool_root_path/$dir"
	progtool_run_cmd "hg pull"
	eval ret="'$?'"
	if [[ $ret -ne 0 ]]; then
	    progtool_print_msg "failed to update $uri in $progtool_root_path/$dir"
	fi
    fi
 
    return $ret
}

# fetch or update argument repositories
progtool_update_repos()
{
    root="$progtool_root_path"
    uris=$@
    ret=0

    for uri in $uris
    do
	path=`basename $uri`
	dir=`echo $path | sed -e 's/'"\.git"'\$//p'`
	cd "$root"
	git_update_repo "$uri" "$dir"
	eval ret="'$?'"
	cd "$root"
	if [[ $ret -ne 0 ]] && [[ -d "$dir" ]]; then
	    hg_update_repo "$uri" "$dir"
	    eval ret="'$?'"
	    cd "$root"
	    if [[ $ret -ne 0 ]]; then
		progtool_print_msg "failed to update $uri in $progtool_root_path/$dir"
	    fi
	fi
    done
    cd "$root"
    
    return $ret;
}

# build program using gnu auto-tools
progtool_build_auto()
{
    root="$progtool_root_path"
    dirs=$@
    docdir="$progtool_root_path/doc/$pkg"
    ret=0

    cd "$root"
    if [[ -d "$dir" ]]; then
	cd "$dir"
    else
	eval ret="'$?'"

	return $ret
    fi
    if [[ $progtool_quiet_option -ne 0 ]]; then
	configure_options="-q"
    fi
    if [[ $progtool_dry_run_option -ne 0 ]]; then
	configure_options="-n"
    fi
    ret=1
    if [[ -x "bootstrap.sh" ]]; then
	cmd="./bootstrap.sh"
	progtool_run_cmd `eval "$cmd"`
	eval ret="'$?'"
    fi
    if [[ $ret -ne 0 ]]; then
	if [[ -x "autogen.sh" ]]; then
	    cmd=",/autogen.sh"
	    progtool_run_cmd `eval "$cmd"`
	    eval ret="'$?'"
	fi
    fi
    if [[ $ret -ne 0 ]]; then
	cmd="autoreconf -i"
	progtool_run_cmd `eval "$cmd"`
	eval ret="'$?'"
    fi
    if [[ $ret -ne 0 ]]; then
	progtool_print_msg "$cmd failed"
    else
	progtool_run_cmd `eval ./configure $configure_options --prefix="$progtool_root_path" --exec-prefix="$progtool_root_path" --program-prefix="$pkg" --includedir="$progtool_root_path/include" --oldincludedir="$progtool_root_path/usr/include" --datarootdir="$progtool_root_path/share" --datadir="$progtool_root_path/share" --infodir="$progtool_root_path/info" --localedir="$progtool_root_path/share" --mandir="$PROGTOOL_ROOT/man" --docdir="$docdir" --htmldir="$docdir" --dvidir="$docdir" --pdfdir="$docdir" --psdir="$docdir"`
	eval ret="'$?'"
	if [[ $ret -ne 0 ]]; then
	    progtool_print_msg "configure failed"
	else
	    cmd="make"
	    progtool_run_cmd `eval "$cmd"`
	    eval ret="'$?'"
	    if [[ $ret -ne 0 ]]; then
		progtool_print_msg "make failed"
	    else
		cmd="make install"
		progtool_run_cmd `eval "$cmd"`
		eval ret="'$?'"
		if [[ $ret -ne 0 ]]; then
		    progtool_print_msg "make install failed"
		fi
	    fi
	fi
    fi
    cd "$root"

    return $ret
}

# print configuration information
progtool_print_info()
{
    uris="$progtool_repo_list"
    echo "package repositories"
    echo "--------------------"
    for uri in $uris
    do
	echo "	$uri"
    done
    dirs="$progtool_repo_dirs"
    echo "package directories"
    echo "-------------------"
    for dir in $dirs
    do
	echo "	$dir"
    done

    return
}

# main routine
progtool_main()
{
    ret=0
    
    progtool_get_opts $@
    progtool_create_repo_list "$PROGTOOL_REPO_URIS"
    progtool_create_repo_list "$progtool_repo_uris"
    if [[ $progtool_build_option -ne 0 ]]; then
	progtool_update_repos "$progtool_repo_list"
	progtool_build_progs "$progtool_repo_dirs"
	eval ret="'$?'"
    fi
    if [[ $progtool_clean_option -ne 0 ]]; then
	progtool_clean_dirs "$progtool_repo_dirs"
	if [[ $progtool_force_option -ne 0 ]]; then
	    progtool_clean_dirs "$PROGTOOL_DEST_DIRS"
	else
	    progtool_remove_dirs "$PROGTOOL_DEST_DIRS"
	fi
	eval ret="'$?'"
    fi
    if [[ $progtool_info_option -ne 0 ]]; then
	progtool_print_info "$progtool_repo_list"
	progtool_print_info "$progtool_repo_dirs"
    fi
    
    exit $ret
}

progtool_main $@

