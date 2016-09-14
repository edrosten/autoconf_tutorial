AC_INIT(myconfig, version-0.1)
AC_LANG(C++)
AC_PROG_CXX

dnl list of all possible optional components
all_options="jpeg png tiff funkyimage v4l2 ffmpeg awesomevideo" 

dnl When a component is found, append it here
options=

dnl Force the compiler to run once and do all the basic checks
dnl if you don't, it will do the test on the first invocation 
dnl below and so your pretty section titles won't work as well
AC_CHECK_HEADERS(iostream)

dnl Small macro to print out a nice, pretty section title.
define(SECTION_TITLE,
[
	echo >& AS_MESSAGE_FD
	echo '   $1   ' | sed -e's/./-/g' >&AS_MESSAGE_FD
	echo '   $1' >& AS_MESSAGE_FD
	echo '   $1   ' | sed -e's/./-/g' >&AS_MESSAGE_FD
])


SECTION_TITLE([Checking for image libraries])

a=0
AC_CHECK_HEADERS(jpeglib.h, [], [a=1])
AC_SEARCH_LIBS(jpeg_set_defaults, jpeg, [], [a=1])
if test $a == 0
then
	options="$options jpeg"
fi

a=0
AC_CHECK_HEADERS(png.h, [], [a=1])
AC_SEARCH_LIBS(png_create_info_struct, png, [], [a=1])
if test $a == 0
then
	options="$options png"
fi

a=0
AC_CHECK_HEADERS(tiffio.h, [], [a=1])
AC_SEARCH_LIBS(TIFFOpen, tiff, [], [a=1])
if test $a == 0
then
	options="$options tiff"
fi

a=0
AC_CHECK_HEADERS(funkyimage.h, [], [a=1])
AC_SEARCH_LIBS(funkyopen, funkyimage, [], [a=1])
if test $a == 0
then
	options="$options tiff"
fi


SECTION_TITLE([Checking for video options])

a=0
AC_CHECK_HEADERS(linux/videodev2.h, [], [a=1])
if test $a == 0
then
	options="$options v4l2"
fi



dnl this one is quite big, so give some helpful extra
dnl messages
AC_MSG_CHECKING([for ffmpeg])
AC_MSG_RESULT([])

a=0
AC_CHECK_HEADERS([libavcodec/avcodec.h libavformat/avformat.h libswscale/swscale.h], [], [a=1])
AC_SEARCH_LIBS(main, avutil, [], [a=1])
AC_SEARCH_LIBS(av_read_frame, avformat, [], [a=1])
AC_SEARCH_LIBS(avcodec_open2, avcodec, [], [a=1])
AC_SEARCH_LIBS(sws_getContext, swscale, [], [a=1])
if test $a == 0
then
	options="$options ffmpeg"
else
	AC_MSG_NOTICE([ffmpeg not found])
fi

a=0
AC_CHECK_HEADERS(awesomevid.h, [], [a=1])
AC_SEARCH_LIBS(openawesome, awesomevid, [], [a=1])
if test $a == 0
then
	options="$options awesomevideo"
fi




dnl Now process the options strings. Essentially, we want two lists
dnl one for the options present (which we have) and one for the options
dnl missing (which we don't)

SECTION_TITLE([Configuration results])

echo "Options:" >& AS_MESSAGE_FD
echo "$options" >& AS_MESSAGE_FD
echo >& AS_MESSAGE_FD

echo "Missing options:" >& AS_MESSAGE_FD
echo "$options" "$all_options" | tr ' ' '\n' | sort | uniq -u | tr '\n' ' ' >& AS_MESSAGE_FD


echo -e "\n\n" >& AS_MESSAGE_FD
echo "CXXFLAGS=$CXXFLAGS" >& AS_MESSAGE_FD
echo "LDFLAGS=$LDFLAGS" >& AS_MESSAGE_FD
echo "LIBS=$LIBS" >& AS_MESSAGE_FD
