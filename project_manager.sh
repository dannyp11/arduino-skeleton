#!/bin/bash
#
#       maintainer script
#       essential for travis builder
#       

# path to this script file
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
SCRIPT_DIR_RELATIVE=$0

# global vars
FORCE_CONTINUE=0
FULL_REPORT=1

RETVAL_FILE=.retVal.tmp # file contains return value of script, useful for travis
rm -f $RETVAL_FILE # rm retval file first

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

updateRetval()
{
    # add $1 to retVal file
    local retVal=0
    local incVal=$1
    
    if [ -f $RETVAL_FILE ]; then
        # get content of retvalfile to retVal
        retVal=$(cat $RETVAL_FILE)
    fi
        
    ((retVal = retVal + incVal))
    echo $retVal > $RETVAL_FILE
}

# run all make check in directory recursively
function checkFolder()
{
	find "$1" -name Makefile | while read line; do		
        local TEST_DIR=$(dirname $line)/test/
        if [ -d "$TEST_DIR" ] ; then
        	print_title "Checking in $(dirname ${line})"
            make -C $(dirname $line) check BRIEF=1 -j4       
            updateRetval $?
        fi		
	done
}

# run checkFolder on multiple folders
function checkMultipleFolders()
{
    # $* - list of folders
    dirList=("$*")
    
    for D in $dirList; do
        if [ -d $D ]; then
            checkFolder $D
        else
            print_warning "$D doesn't exist"
            updateRetval 1
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
	    updateRetval $MK_RESULT

	    if (($MK_RESULT!=0)) ; then
		    print_warning "Error code $MK_RESULT on compiling $(dirname $line)"
		    if (($FORCE_CONTINUE == 0)) ; then
			    exit $MK_RESULT
		    fi
	    fi
	done
}

# run makeFolder on multiple folders
function makeMultipleFolders()
{
    # $* - list of folders
    dirList=("$*")
    
    for D in $dirList; do
        if [ -d $D ]; then        
            makeFolder $D
        else
            print_warning "$D doesn't exist"
            updateRetval 1
        fi
    done
}

function checkDependency()
{    
    # check submodules
    # for each dir in submodules
    local TOP=$(dirname $(readlink -f $0))
    for D in `find $TOP/submodules/ -maxdepth 1 -mindepth 1 -type d`
    do
        if [ -z "$(ls -A $D)" ]; then
            ((retVal=retVal+1))
            print_warning "Missing submodule $D, perhaps you forgot to run git submodule update --init --recursive"
        fi
    done
    
    # check arduino ide
    if [ -z "$(which arduino 2>/dev/null)" ]; then  
        ((retVal=retVal+1))
        print_warning "Missing arduino ide, which is required for Arduino-makefile submodule, please install arduino"
    fi
    
    # check avr
    if [ -z "$(which avr-gcc 2>/dev/null)" ] || [ -z "$(which avrdude 2>/dev/null)" ]; then  
        ((retVal=retVal+1))
        print_warning "Missing avr package, please install gcc-avr binutils-avr gdb-avr avr-libc avrdude"
    fi
}

function showHelp()
{
cat << EOF
        usage: $SCRIPT_DIR_RELATIVE [option] {value} DIR1 DIR2 ...
        
        OPTION  DECRIPTION
        -c dir  search in dir recursively and run make clean
        -k      search in DIR1 DIR2 ... recursively and run make check
        -m      search in DIR1 DIR2 ... recursively and run make all
        
        -T      check dependency
        -f      force continue on make error, default: $FORCE_CONTINUE
        -l      set FULL_REPORT=0, default: $FULL_REPORT
        -h      this menu
EOF
}

function detectBuildFailure()
{
    # check if there is .hex and .elf file generated from make
    # if not, return number of failures
    echo ""
}

# main ##############################################################
CHECK_DIR=0
COMPILE_DIR=0

# getopt
while getopts ":c:kmt:flT" o; do
    case "${o}" in
        c)
            cleanFolder ${OPTARG}            
            exit $?            
            ;;
        k)
            CHECK_DIR=1
            ;;
        m)
            COMPILE_DIR=1
            ;;
        t)
            print_title ${OPTARG}
            ;;
        f)
            FORCE_CONTINUE=1
            ;;
        l)
            FULL_REPORT=0            
            ;;
        T)
            checkDependency
            ;;
        *)
            showHelp
            ;;
    esac
done
shift $((OPTIND-1))

# make check on a list of dirs recursively
if [ "$CHECK_DIR" -ne "0" ]; then
    checkMultipleFolders "$*"
fi

# make all on a list of dirs recursively
if [ "$COMPILE_DIR" -ne "0" ]; then
    makeMultipleFolders "$*"
fi

# make all on a list of dirs recursively

# find return value
if [ -f $RETVAL_FILE ]; then
    retVal=$(cat $RETVAL_FILE)
    rm -f $RETVAL_FILE
    exit $retVal
else
    exit -1
fi
