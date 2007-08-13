CLEANFILES = *~
MAINTAINERCLEANFILES = Makefile.in

INCLUDES = -I$(top_srcdir)
AM_CFLAGS = @LSYS_CFLAGS@ $(gtk_CFLAGS) $(gdk_CFLAGS) $(cairo_CFLAGS) $(copme_CFLAGS)

SUBDIRS = . test

bin_PROGRAMS = lsys

lsys_SOURCES = draw.c  draw.h  \
               gui.c   gui.h   \
               about.c about.h \
               examples.c examples.h \
               main.c

lsys_LDADD = \
             $(top_builddir)/lsys/liblsys.a \
             $(top_builddir)/lsysutil/liblsysutil.a \
             $(gtk_LIBS) $(gdk_LIBS) $(cairo_LIBS) $(copme_LIBS)

# vim: set et:
