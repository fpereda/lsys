CLEANFILES = *~
MAINTAINERCLEANFILES = Makefile.in

AM_CFLAGS = @LSYS_CFLAGS@

INCLUDES = -I. -I$(top_srcdir)

SUBDIRS = .

TESTS_ENVIRONMENT = env
TESTS = stack-simple \
        xfuncs-malloc

check_PROGRAMS = stack-simple \
                 xfuncs-malloc

COMMONLDADD = $(top_builddir)/ctme/libctme.a \
              $(top_builddir)/lsysutil/liblsysutil.a

stack_simple_SOURCES = stack-simple.c
stack_simple_LDADD   = $(COMMONLDADD)

xfuncs_malloc_SOURCES = xfuncs-malloc.c
xfuncs_malloc_LDADD   = $(COMMONLDADD)

# vim: set et:
