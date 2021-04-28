AUTOMAKE_OPTIONS = foreign

lib_LTLIBRARIES = libqmivs.la
libqmivs_la_SOURCES = voice_service_v02.c
libqmivs_la_LDFLAGS = -module
libqmivs_la_CPPFLAGS = -I. \
						-I$(top_builddir)/sources/include
qmivsincludedir = $(includedir)/vs
qmivsinclude_HEADERS = *.h
