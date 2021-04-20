bin_PROGRAMS = dial_demo

 dial_demo_SOURCES = qmi_volte_demo.c
#dial_demo_SOURCES = multi_wds_control.c
#dial_demo_SOURCES = main.c
#dial_demo_SOURCES = wds_only.c
dial_demo_LDADD = $(top_srcdir)/sources/utils/libqmiutils.la \
				  $(top_srcdir)/sources/wds/libqmiwds.la \
				  $(top_srcdir)/sources/dms/libqmidms.la \
				  $(top_srcdir)/sources/nas/libqminas.la \
				  $(top_srcdir)/sources/uim/libqmiuim.la \
				  $(top_srcdir)/sources/vs/libqmivs.la
				  
dial_demo_CPPFLAGS = -I. \
					 -I$(top_srcdir)/sources/include \
				     -I$(top_srcdir)/sources/wds \
				     -I$(top_srcdir)/sources/dms \
				     -I$(top_srcdir)/sources/nas \
				     -I$(top_srcdir)/sources/uim \
					 -I$(top_srcdir)/sources/vs
