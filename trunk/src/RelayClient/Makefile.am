AM_CPPFLAGS = -I$(top_srcdir)/include/ \
			  -I$(top_srcdir)/src

bin_PROGRAMS = $(top_srcdir)/bin/testclient
__top_srcdir__bin_testclient_SOURCES = main.cpp ServerAgent.cpp
__top_srcdir__bin_testclient_LDADD = $(top_srcdir)/lib/libcommon.la 

LIBS += -g
