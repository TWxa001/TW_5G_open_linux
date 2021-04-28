AUTOMAKE_OPTIONS = foreign

lib_LTLIBRARIES = libqmiwds.la
libqmiwds_la_SOURCES = wireless_data_service_v01.c \
                       wireless_data_service_common_v01.c \
		       data_common_v01.c
libqmiwds_la_LDFLAGS = -module
libqmiwds_la_CPPFLAGS = -I. \
						-I$(top_srcdir)/sources/include
qmiwdsincludedir = $(includedir)/wds
qmiwdsinclude_HEADERS = *.h
