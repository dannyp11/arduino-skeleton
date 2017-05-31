# This makefile doesn't build anything, just for maintaining the skeleton 

TOP = ./
.PHONY: clean

# run all unit tests in all folders
check:
	$(TOP)/check.sh
        
# clean all projects for maintaining purpose
clean:
	$(TOP)/cleanup.sh

# show readme file
info help man:
	@view README.md

