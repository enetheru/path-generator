AUTOMAKE_OPTIONS = 1.4 foreign

MAINTAINERCLEANFILES = Makefile.in aclocal.m4 config.h.in configure depcomp install-sh missing


INCLUDES = -I$(top_srcdir)


bin_PROGRAMS = pathgen


pathgen_SOURCES = main.c world.c world.h pathgen.h \
   pathgen_map.h pathgen_map.c \
   pathgen_node.h pathgen_node.c \
   pathgen_path.h pathgen_path.c

pathgen_LDADD = @ELEMENTARY_LIBS@

pathgen_CFLAGS = @ELEMENTARY_CFLAGS@
