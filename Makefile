# This makefile doesn't build anything, just for maintaining the skeleton 

TOP = ./
.PHONY: clean

# run all unit tests in all folders
check:
	$(TOP)/project_manager.sh -k test/
	$(TOP)/project_manager.sh -k c-project/
	$(TOP)/project_manager.sh -k common/
        
# clean all projects for maintaining purpose
clean:
	$(TOP)/project_manager.sh -c test/
	$(TOP)/project_manager.sh -c c-project/

# show readme file
readme info:
	@view README.md

# show help
help:
	$(TOP)/project_manager.sh -h
