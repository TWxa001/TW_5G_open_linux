AUTOMAKE_OPTIONS = foreign

lib_LTLIBRARIES = libqminas.la
libqminas_la_SOURCES = network_access_service_v01.c
libqminas_la_LDFLAGS = -module
libqminas_la_CPPFLAGS = -I. \
						-I$(top_srcdir)/sources/include
qminasincludedir = $(includedir)/nas
qminasinclude_HEADERS = *.h
