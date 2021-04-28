#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([qmi-sdk], [0.1], [songchenglin@longsung.com])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE
AC_CONFIG_SRCDIR([sources/demo/main.c])
LT_INIT
#AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC
AC_PROG_RANLIB
AM_PROG_CC_C_O

# Checks for libraries.
#AC_CHECK_LIB([libpthread])

# Checks for header files.
AC_CHECK_HEADERS([fcntl.h netinet/in.h stdint.h stdlib.h string.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT8_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
#AC_FUNC_MALLOC
#AC_CHECK_FUNCS([bzero gettimeofday memset select socket])

AC_CONFIG_FILES([
                 Makefile
                 sources/Makefile
                 sources/utils/Makefile
                 sources/dms/Makefile
                 sources/nas/Makefile
                 sources/uim/Makefile
                 sources/vs/Makefile
                 sources/wds/Makefile
                 sources/demo/Makefile
                ])
AC_OUTPUT
