AC_INIT(program, 1.0)

dnl Switch to a C++ compiler, and check if it works.
AC_PROG_CXX


a=0
AC_CHECK_HEADERS(zlib.h, [], [a=1])
AC_SEARCH_LIBS(deflate, z, [], [a=1])

if test $a == 0
then
	AC_DEFINE(HAVE_LIBZ)
fi


AC_CONFIG_HEADERS(config.h)
AC_OUTPUT(Makefile)

