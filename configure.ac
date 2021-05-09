AC_INIT([randbus], [0.0.1], [henrik@ullman.com])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([src/config.h])
AM_INIT_AUTOMAKE([foreign subdir-objects])
AC_PROG_CC
AC_PROG_LIBTOOL

	
AC_SEARCH_LIBS([prom_gauge_new], [prom], [], [AC_MSG_ERROR([required library libprom not found])])
AC_SEARCH_LIBS([promhttp_start_daemon], [promhttp], [], [AC_MSG_ERROR([required library libpromhttp not found])])
AC_CHECK_LIB([z], [crc32], [], [AC_MSG_ERROR([required library zlib not found])])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
