
# AM_LDFLAGS = -pthread -lgtk-x11-2.0 -lgdk-x11-2.0 -latk-1.0 -lgio-2.0 -lpangoft2-1.0 -lpangocairo-1.0 -lgdk_pixbuf-2.0 -lcairo -lpango-1.0 -lfreetype -lfontconfig -lgobject-2.0 -lgmodule-2.0 -lgthread-2.0 -lrt -lglib-2.0 -lcurl
# AM_CFLAGS = -I ../inc -pthread -I/usr/include/gtk-2.0 -I/usr/lib64/gtk-2.0/include -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib64/glib-2.0/include -I/usr/include/pixman-1 -I/usr/include/freetype2 -I/usr/include/libpng12
AM_CFLAGS = $(shell pkg-config --cflags gtk+-2.0)
AM_CFLAGS += -I. -I../inc
AM_LDFLAGS = $(shell pkg-config --libs gtk+-2.0)
AM_LDFLAGS += $(shell curl-config --libs)
bin_PROGRAMS = assistor
assistor_SOURCES = main.c window.c ccurl.c secretary.c
