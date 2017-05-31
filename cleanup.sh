#!/bin/bash

function cleanFolder()
{
	find "$1" -name Makefile | while read line; do		
		make -C $(dirname $line) clean
	done		
}

cleanFolder test/
cleanFolder c-project/without-os/

