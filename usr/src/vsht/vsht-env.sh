#! /bin/sh

vsht_print_usage()
{
    echo -e "$0 usage"
    echo -e "----"
    echo -e "\t-a, --analyze\tenable static analysis"
    echo -e "\t-p, --profile\tenable profiling"-
    echo -e "\t-v, --verbose\tverbose messages"
    echo -e "\t-C CMD, --cc=CMD\tuse CMD as the compiler"
    echo -e "\t-A CMD, --as=CMD\tuse CMD as the assembler"
    echo -e "\t-L CMD, --ld=CMD\tuse CMD as the linker"
}

vsht_print_env()
{
    if [ "$vsht_opt_verbose" -ne 0 ]; then
	    vsht_print_opts
	    vsht_print_conf
    fi
}

set -x
vsht_print_env

