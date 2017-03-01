.PHONY: ${SUBDIRS}

${SUBDIRS}:
	@echo "Entering ${CURDIR}/$@..."
	@${MAKE} -C $@ ${RECURSIVE_TARGET}
	@echo "Leaving ${CURDIR}/$@..."

recursive: ${SUBDIRS}
