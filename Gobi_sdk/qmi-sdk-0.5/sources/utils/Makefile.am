AUTOMAKE_OPTIONS = foreign
 
lib_LTLIBRARIES = libqmiutils.la

#sources use xport_qmuxd. No ipc or udp
libqmiutils_la_SOURCES = src/common_v01.c \
						 src/qmi_cci_common.c \
						 src/qmi_cci_target.c \
						 src/qmi_cci_xport_qmuxd.c \
						 src/qmi_idl_accessor.c \
						 src/qmi_idl_lib.c \
						 src/smem_log.c

libqmiutils_la_LDFLAGS = -module
libqmiutils_la_LIBADD = -lpthread

libqmiutils_la_CPPFLAGS = -DQCCI_OVER_QMUX \
						  -I inc \
						  -I$(top_srcdir)/sources/include 
qmiutilsincludedir = $(includedir)/utils
qmiutilsinclude_HEADERS = $(top_srcdir)/sources/include/*.h

EXTRA_DIST = $(top_srcdir)/sources/utils/inc
