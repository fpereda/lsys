CLEANFILES = *~
MAINTAINERCLEANFILES = Makefile.in

INCLUDES = -I$(top_srcdir)
AM_CFLAGS = @LSYS_CFLAGS@ -std=c99 -D_XOPEN_SOURCE -D_XOPEN_SOURCE_EXTENDED

SUBDIRS = .

lib_LIBRARIES = liblsys.a

liblsys_a_SOURCES = lsys.c lsys.h \
                    examples.c examples.h
liblsys_a_LIBADD = $(top_builddir)/lsysutil/liblsysutil.a

# vim: set et:
