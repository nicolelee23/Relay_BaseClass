noinst_LTLIBRARIES=libtask.la
libtask_la_SOURCES=LogOffTask.cpp \
				   LogOffWorkItem.cpp \
				   LogInTask.cpp \
				   LogInWorkItem.cpp
libtask_la_CXXFLAGS=-I$(top_srcdir)/include/common/ \
					-I$(top_srcdir)/include/ \
					-I$(top_srcdir)/src
libtask_la_LIBADD=$(top_srcdir)/lib/libcommon.la \
				  ../../CToSMapManager/libctosmapmanager.la
