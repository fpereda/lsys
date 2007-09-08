CLEANFILES = *~ examples.c
MAINTAINERCLEANFILES = Makefile.in

INCLUDES = -I$(top_srcdir)
AM_CFLAGS = @LSYS_CFLAGS@ -std=c99 -D_XOPEN_SOURCE=600

SUBDIRS = .
SUBDIRS_DIST = examples

lib_LIBRARIES = liblsys.a

liblsys_a_SOURCES = lsys.c lsys.h \
                    examples.c examples.h
liblsys_a_LIBADD = $(top_builddir)/lsysutil/liblsysutil.a

examples = \
           $(top_srcdir)/lsys/examples/akoch-island \
           $(top_srcdir)/lsys/examples/dragon \
           $(top_srcdir)/lsys/examples/hilbert \
           $(top_srcdir)/lsys/examples/hilbert2 \
           $(top_srcdir)/lsys/examples/koch-curve \
           $(top_srcdir)/lsys/examples/koch-island \
           $(top_srcdir)/lsys/examples/koch-quad \
           $(top_srcdir)/lsys/examples/peano \
           $(top_srcdir)/lsys/examples/plant1 \
           $(top_srcdir)/lsys/examples/plant2 \
           $(top_srcdir)/lsys/examples/plant3 \
           $(top_srcdir)/lsys/examples/plant4 \
           $(top_srcdir)/lsys/examples/plant5 \
           $(top_srcdir)/lsys/examples/sier-arrow \
           $(top_srcdir)/lsys/examples/sier-carpet \
           $(top_srcdir)/lsys/examples/sier-sieve \
           $(top_srcdir)/lsys/examples/tree1 \
           $(top_srcdir)/lsys/examples/tree2

examples.c: $(top_srcdir)/misc/gen_examples.bash $(examples)
	$(top_srcdir)/misc/gen_examples.bash \
		$(top_srcdir)/misc/generated-file.txt \
		$(top_srcdir)/lsys/examples \
		$(top_srcdir)/lsys/examples.c.in

# vim: set et:
