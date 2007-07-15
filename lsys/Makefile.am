CLEANFILES = *~
MAINTAINERCLEANFILES = Makefile.in

INCLUDES = $(PACKAGE_CFLAGS) -I$(top_srcdir)
AM_CFLAGS = $(LSYS_CFLAGS)

SUBDIRS = .

lib_LIBRARIES = liblsys.a

liblsys_a_SOURCES = lsys.c lsys.h
liblsys_a_LIBADD = $(top_builddir)/lsysutil/liblsysutil.a

# vim: set et:
