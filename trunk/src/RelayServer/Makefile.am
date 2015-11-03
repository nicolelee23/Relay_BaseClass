SUBDIRS =Task
INCLUDES = -I$(top_srcdir)/include/ \
		   -I$(top_srcdir)/include/common \
		   -I$(top_srcdir)/src/

bin_PROGRAMS = $(top_srcdir)/bin/testserver
__top_srcdir__bin_testserver_SOURCES = main.cpp ClientAgent.cpp 
__top_srcdir__bin_testserver_LDADD = $(top_srcdir)/lib/libcommon.la \
									 $(top_srcdir)/build/src/CToSMapManager/libctosmapmanager.la \
									 $(top_srcdir)/build/src/RelayServer/Task/libtask.la


