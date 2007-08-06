CLEANFILES = *~ examples.c
MAINTAINERCLEANFILES = Makefile.in

INCLUDES = -I$(top_srcdir)
AM_CFLAGS = @LSYS_CFLAGS@ -std=c99 -D_XOPEN_SOURCE=600

SUBDIRS = .

lib_LIBRARIES = liblsys.a

liblsys_a_SOURCES = lsys.c lsys.h \
                    examples.c examples.h
liblsys_a_LIBADD = $(top_builddir)/lsysutil/liblsysutil.a

examples.c: $(top_srcdir)/misc/gen_examples.bash
	$(top_srcdir)/misc/gen_examples.bash \
		$(top_srcdir)/misc/generated-file.txt \
		$(top_srcdir)/lsys/examples \
		$(top_srcdir)/lsys/examples.c.in

# vim: set et:
