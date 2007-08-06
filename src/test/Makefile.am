CLEANFILES = *~
MAINTAINERCLEANFILES = Makefile.in

AM_CFLAGS = @LSYS_CFLAGS@

INCLUDES = -I. -I$(top_srcdir)

SUBDIRS = .

TESTS_ENVIRONMENT = env
TESTS = \
        list-examples \
        version

check_SCRIPTS = \
                list-examples \
                version

EXTRA_DIST = $(check_SCRIPTS)

# vim: set et:
