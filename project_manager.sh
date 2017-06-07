#!/bin/bash
#
#       maintainer script
#       

# path to this script file
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SCRIPT_DIR_RELATIVE=$0

# print colored title
print_title()
{
	local RED='\033[0;31m'
	local GREEN='\033[0;32m'
	local NC='\033[0m' # No Color

	local txt=$1
	echo -e "${GREEN} =========================================="
	echo " $txt"
	echo -e " ========================================== ${NC}"
}

# run all make check in directory recursively
function checkFolder()
{
	find "$1" -name Makefile | while read line; do		
                local TEST_DIR=$(dirname $line)/test/
                if [ -d "$TEST_DIR" ] ; then
                        make -C $(dirname $line) check BRIEF=1 -j4
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

# run make all in dir recursively
function makeFolder()
{
	find "$1" -name Makefile | while read line; do	
	        print_title "Compiling" ;	
		make -C $(dirname $line) all -j4
	done		
}


function showHelp()
{
cat << EOF
        usage: $SCRIPT_DIR_RELATIVE option [value]
        
        OPTION  DECRIPTION
        -c dir  search in dir recursively and run make clean
        -k dir  search in dir recursively and run make check
        -m dir	search in dir recursively and run make all
EOF
}

# main ##############################################################
# getopt
while getopts ":c:k:m:" o; do
    case "${o}" in
        c)
            cleanFolder ${OPTARG}            
            ;;
        k)
            checkFolder ${OPTARG}
            ;;
        m)
            makeFolder ${OPTARG}
            ;;
        *)
            showHelp
            ;;
    esac
done
shift $((OPTIND-1))

