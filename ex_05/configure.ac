AC_INIT(program, 1.0)

dnl Switch to a C++ compiler, and check if it works.
AC_LANG(C++)
AC_PROG_CXX

AC_CHECK_HEADERS(zlib.h, [], [AC_ERROR([A working zlib is required])])
AC_SEARCH_LIBS(deflate, z, [], [AC_ERROR([A working zlib is required])])

dnl Process Makefile.in to create Makefile
AC_OUTPUT(Makefile)

