AUTOMAKE_OPTIONS = 1.4 foreign

MAINTAINERCLEANFILES = Makefile.in aclocal.m4 config.h.in configure depcomp install-sh missing


INCLUDES = -I$(top_srcdir)


bin_PROGRAMS = pathgen


pathgen_SOURCES = main.c i_defaults.h \
   pathgen_world.h pathgen_world.c \
   pathgen_node.h pathgen_node.c \
   pathgen_path.h pathgen_path.c \
   i_splash.h i_display.h i_sim.h i_world.h i_path.h \
   r_image r_image.c r_noise.h r_pixel.h \
   pg_data.h \
   pg_world.h pg_world.c \
   pg_display.h pg_display.c \
   pg_path.h pg_path.c \
   pg_path_finder.h pg_path_finder.c \
   pg_node.h pg_node.c \
   pg_node_rel.h pg_node_rel.c \
   pg_event.h 

pathgen_LDADD = @ELEMENTARY_LIBS@

pathgen_CFLAGS = @ELEMENTARY_CFLAGS@
