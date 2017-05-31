#!/bin/bash
#
# run all make check in directory recursively

function checkFolder()
{
	find "$1" -name Makefile | while read line; do		
                local TEST_DIR=$(dirname $line)/test/
                if [ -d "$TEST_DIR" ] ; then
                        make -C $(dirname $line) check
                fi		
	done		
}

checkFolder test/
checkFolder c-project/without-os/

