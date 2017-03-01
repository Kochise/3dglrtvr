link_headers:
ifdef LINK_HEADERS
	@${LPILINK} ${HEADER_LINK_DIR} ${LINK_HEADERS}
endif

link_sources:
ifdef LINK_SOURCES
	@${SOURCELINK} ${SOURCES_LINK_DIR} ${LINK_SOURCES}
endif

ifdef SOURCES
OBJECTS = ${SOURCES:.C=.o}
endif

%.o: %.C
	${CXX_COMPILE} $<

all: link_headers link_sources ${SUBDIRS} ${OBJECTS} ${LIBRARIES} ${EXECUTABLES}
