AC_INIT(program, 1.0)

dnl Switch to a C++ compiler, and check if it works.
AC_LANG(C++)
AC_PROG_CXX

dnl List of object files as determined by autoconf
object_files=""


AC_ARG_WITH(zlib, [AS_HELP_STRING([--without-zlib], [do not use zlib])])

zlib_objs=hello_no_libz.o
if test "$with_zlib" != no
then
	a=0
	AC_CHECK_HEADERS(zlib.h, [], [a=1])
	AC_SEARCH_LIBS(deflate, z, [], [a=1])

	if test $a == 0
	then
		zlib_objs=hello_libz.o
	fi
fi

object_files="$object_files $zlib_objs"



dnl Make AC_OUTPUT substitute the contents of $object_files for @objs@
AC_SUBST(objs, ["$object_files"])

dnl Process Makefile.in to create Makefile
AC_OUTPUT(Makefile)

