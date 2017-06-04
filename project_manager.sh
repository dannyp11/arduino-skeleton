#!/bin/bash
#
#       maintainer script
#       

# path to this script file
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SCRIPT_DIR_RELATIVE=$0

# run all make check in directory recursively
function checkFolder()
{
	find "$1" -name Makefile | while read line; do		
                local TEST_DIR=$(dirname $line)/test/
                if [ -d "$TEST_DIR" ] ; then
                        make -C $(dirname $line) check BRIEF=1
                fi		
	done		
}

# run make clean in dir recursively
function cleanFolder()
{
	find "$1" -name Makefile | while read line; do		
		make -C $(dirname $line) clean
	done		
}

function showHelp()
{
cat << EOF
        usage: $SCRIPT_DIR_RELATIVE option [value]
        
        OPTION  DECRIPTION
        -c dir  search in dir recursively and run make clean
        -k dir  search in dir recursively and run make check
EOF
}

# main ##############################################################
# getopt
while getopts ":c:k:" o; do
    case "${o}" in
        c)
            cleanFolder ${OPTARG}            
            ;;
        k)
            checkFolder ${OPTARG}
            ;;
        *)
            showHelp
            ;;
    esac
done
shift $((OPTIND-1))

