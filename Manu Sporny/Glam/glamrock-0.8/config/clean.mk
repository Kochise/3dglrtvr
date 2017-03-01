# Rules for cleaning directories
CLEANUP_FILES += core

clean:
	@echo Cleaning up in ${CURDIR}...
ifdef OBJECTS
	@rm -rf ${CLEANUP_FILES} ${OBJECTS}
endif
