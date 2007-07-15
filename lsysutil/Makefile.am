CLEANFILES = *~
MAINTAINERCLEANFILES = Makefile.in

INCLUDES = $(PACKAGE_CFLAGS)
AM_CFLAGS = $(LSYS_CFLAGS)

SUBDIRS = . test

lib_LIBRARIES = liblsysutil.a

liblsysutil_a_SOURCES = stack.c stack.h \
                        xfuncs.h

# vim: set et:
