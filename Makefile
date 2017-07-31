# This makefile doesn't build anything, just for maintaining the skeleton 
TOP = ./

.PHONY: compile
# run all unit tests in all folders
check:
	$(TOP)/project_manager.sh -T
	$(TOP)/project_manager.sh -k test c-project cpp-project common
        
# comipile all projects
compile:        
	$(TOP)/project_manager.sh -T
	$(TOP)/project_manager.sh -fm test c-project cpp-project
	
# clean all projects for maintaining purpose
clean:
	$(TOP)/project_manager.sh -c test/
	$(TOP)/project_manager.sh -c c-project/
	$(TOP)/project_manager.sh -c cpp-project/

# show readme file
readme info:
	@view README.md

# show release note
log:
	@view ReleaseNote.txt

# show help
help:
	$(TOP)/project_manager.sh -h	
