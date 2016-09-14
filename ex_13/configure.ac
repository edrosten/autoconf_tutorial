AC_INIT(myconfig, version-0.1)
AC_LANG(C++)
AC_PROG_CXX


all_options="jpeg png tiff" 

dnl Force the compiler to run once and do all the basic checks
AC_CHECK_HEADERS(iostream)

echo ----------------------------------
echo    Checking for image libraries
echo ----------------------------------

a=0
AC_CHECK_HEADERS(jpeglib.h, [], [a=1])
AC_SEARCH_LIBS(jpeg_set_defaults, jpeg, [], [a=1])
if test $a == $0
then
	options="$options jpeg"
	dnl also AC_SUBST and anything else
fi

a=0
AC_CHECK_HEADERS(png.h, [], [a=1])
AC_SEARCH_LIBS(png_init, jpeg, [], [a=1])
if test $a == $0
then
	options="$options jpeg"
	dnl also AC_SUBST and anything else
fi
