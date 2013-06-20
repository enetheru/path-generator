AUTOMAKE_OPTIONS = 1.4 foreign

MAINTAINERCLEANFILES = Makefile.in aclocal.m4 config.h.in configure depcomp install-sh missing


INCLUDES = -I$(top_srcdir)


bin_PROGRAMS = pathgen


pathgen_SOURCES = main.c world.c world.h node.h

pathgen_LDADD = @ELEMENTARY_LIBS@

pathgen_CFLAGS = @ELEMENTARY_CFLAGS@
