CLEANFILES = *~
MAINTAINERCLEANFILES = Makefile.in

INCLUDES = $(PACKAGE_CFLAGS) -I$(top_srcdir)
AM_CFLAGS = $(LSYS_CFLAGS)

SUBDIRS = .

bin_PROGRAMS = lsys

lsys_SOURCES = draw.c  draw.h  \
               gui.c   gui.h   \
               about.c about.h \
               examples.c examples.h \
               main.c

lsys_LDADD = $(PACKAGE_LIBS) \
             $(top_builddir)/lsys/liblsys.a \
             $(top_builddir)/lsysutil/liblsysutil.a \
             $(top_builddir)/copme/libcopme.a

# vim: set et:
