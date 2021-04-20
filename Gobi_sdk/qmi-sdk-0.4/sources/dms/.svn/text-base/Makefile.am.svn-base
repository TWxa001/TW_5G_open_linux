AUTOMAKE_OPTIONS = foreign
 
lib_LTLIBRARIES = libqmidms.la
libqmidms_la_SOURCES = device_management_service_v01.c
libqmidms_la_LDFLAGS = -module
libqmidms_la_CPPFLAGS = -I. \
						-I$(top_builddir)/sources/include
qmidmsincludedir = $(includedir)/dms
qmidmsinclude_HEADERS = *.h
