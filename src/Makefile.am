CLEANFILES = *~
MAINTAINERCLEANFILES = Makefile.in

INCLUDES = $(PACKAGE_CFLAGS)
AM_CFLAGS = $(LSYS_CFLAGS)

SUBDIRS = .

bin_PROGRAMS = lsys
lib_LIBRARIES = libstack.a

lsys_SOURCES = lsys.c  lsys.h  \
               draw.c  draw.h  \
               gui.c   gui.h   \
               main.c

lsys_LDFLAGS = 
lsys_LDADD = $(PACKAGE_LIBS) libstack.a

libstack_a_SOURCES = stack.c stack.h

# vim: set et:
