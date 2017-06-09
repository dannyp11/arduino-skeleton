#!/bin/bash
#
#       maintainer script
#       

# path to this script file
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SCRIPT_DIR_RELATIVE=$0

# global vars
FORCE_CONTINUE=0
FULL_REPORT=0

# print colored title
print_title()
{
	local RED='\033[0;31m'
	local GREEN='\033[0;32m'
	local NC='\033[0m' # No Color

	local txt=$@
	echo -e "\n${GREEN}|=============================================="
	echo "| $txt"
	echo -e "|==============================================${NC}"
}

# print colored warning
print_warning()
{
	local RED='\033[0;31m'
	local GREEN='\033[0;32m'
	local NC='\033[0m' # No Color

	local txt=$@
	echo -e "\n${RED}|=============================================="
	echo "| $txt"
	echo -e "|==============================================${NC}"
}

# run all make check in directory recursively
function checkFolder()
{
	find "$1" -name Makefile | while read line; do		
                local TEST_DIR=$(dirname $line)/test/
                if [ -d "$TEST_DIR" ] ; then
                	print_title "Checking in $(dirname ${line})" ;	
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
	        print_title "Compile in $(dirname ${line})..." ;
	        
	        if (($FULL_REPORT == 0)) ; then
                  make -C $(dirname $line) all -j4 > /dev/null
	        else
	          make -C $(dirname $line) all -j4
	        fi	
		
		local MK_RESULT=$?

		if (($MK_RESULT!=0)) ; then
			print_warning "Error code $MK_RESULT on compiling $(dirname $line)"
			if (($FORCE_CONTINUE == 0)) ; then
				exit $MK_RESULT
			fi
		fi
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
        
        -f      force continue on make error, default: $FORCE_CONTINUE
        -l      full report, default: $FULL_REPORT
EOF
}

# main ##############################################################
# getopt
while getopts ":c:k:m:t:fl" o; do
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
        t)
            print_title ${OPTARG}
            ;;
        f)
            FORCE_CONTINUE=1
            ;;
        l)
            FULL_REPORT=1
            ;;
        *)
            showHelp
            ;;
    esac
done
shift $((OPTIND-1))

