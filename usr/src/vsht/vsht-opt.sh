#! /bin/sh

vsht_opt_debug_vsht=0
vsht_opt_analyze=0
vsht_opt_profile=0
vsht_opt_verbose=0
vsht_opt_log=0
vsht_opt_cc=""
vsht_opt_as=""
vsht_opt_ld=""

vsht_set_tools()
{
    CC="$vsht_opt_cc"
    AS="$vsht_opt_as"
    LD="$vsht_opt_ld"

    return
}

vsht_get_opts()
{
    for arg
    do
        case $arg in
            -D | --vsht-debug)
                vsht_opt_debug_vsht=1
                ;;
            -a | --analyze)
                vsht_opt_analyze=1
                ;;
            -h | --help)
                vsht_print_usage

                exit 0
                ;;
            -p | --profile)
                vsht_opt_profile=1
                ;;
            -v | --verbose)
                vsht_opt_verbose=1
                ;;
            -C)
                shift
                vsht_opt_cc=${arg}
            -A)
                shift
                vsht_opt_as=${arg}
            -L)
                shift
                vsht_opt_ld=${arg}
            --cc=*)
                vsht_opt_cc=`echo ${arg} | cut -f 2 -d\=`
                ;;
            --as=*)
                vsht_opt_as=`echo ${arg} | cut -f 2 -d\=`
                ;;
            --ld=*)
                vsht_opt_ld=`echo ${arg} | cut -f 2 -d\=`
                ;;
            --log)
                vsht_opt_log=1
                ;;
            *)
                vsht_print_usage

                exit 1
                ;;
        esac
    done
    #    vsht_export_opts

    return
}

vsht_print_opts()
{
    echo -e "build parameters"
    echo -e "----------------"
    echo -e "debug=$vsht_opt_debug"
    echo -e "analyze=$vsht_opt_analyze"
    echo -e "profile=$vsht_opt_profile"
    echo -e "verbose=$vsht_opt_verbose"
    echo -e "log=$vsht_opt_log"
    echo -e "CC=$CC"
    echo -e "AS=$AS"
    echo -e "LD=$LD"

    return
}

