AUTOMAKE_OPTIONS = 1.4 foreign

MAINTAINERCLEANFILES = Makefile.in aclocal.m4 config.h.in configure depcomp install-sh missing


INCLUDES = -I$(top_srcdir)


bin_PROGRAMS = pathgen


pathgen_SOURCES = main.c i_defaults.h image.h\
   pathgen_world.h pathgen_world.c \
   pathgen_node.h pathgen_node.c \
   pathgen_path.h pathgen_path.c \
   i_splash.h i_display.h i_sim.h i_world.h i_path.h

pathgen_LDADD = @ELEMENTARY_LIBS@

pathgen_CFLAGS = @ELEMENTARY_CFLAGS@
