# This makefile doesn't build anything, just for maintaining the skeleton 

TOP = ./
.PHONY: clean

# run all unit tests in all folders
check:
	$(TOP)/project_manager.sh -k test/
	$(TOP)/project_manager.sh -k c-project/
	$(TOP)/project_manager.sh -k cpp-project/
	$(TOP)/project_manager.sh -k common/
        
# comipile all projects
compile:        
	$(TOP)/project_manager.sh -m test/
	$(TOP)/project_manager.sh -f -m c-project/ 
	$(TOP)/project_manager.sh -f -m cpp-project/ 
	
# clean all projects for maintaining purpose
clean:
	$(TOP)/project_manager.sh -c test/
	$(TOP)/project_manager.sh -c c-project/
	$(TOP)/project_manager.sh -c cpp-project/

# show readme file
readme info:
	@view README.md

# show help
help:
	$(TOP)/project_manager.sh -h
