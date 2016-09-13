AC_INIT(myconfig, version-0.1)
AC_LANG(C++)
AC_PROG_CXX


dnl TEST_AND_SET_CXXFLAG(flag, [program])
dnl
dnl This attempts to compile a program with a certain compiler flag.
dnl If no program is given, then the minimal C++ program is compiled, and 
dnl this tests just the validity of the compiler flag. 
dnl
define([TEST_AND_SET_CXXFLAG],[
	AC_MSG_CHECKING([if compiler flag $1 works])	
	
	dnl Store the current CXXFLAGS
	save_CXXFLAGS="$CXXFLAGS"

	dnl Append the flag of interest
	CXXFLAGS="$CXXFLAGS $1"
	
	dnl Create an M4 macro, "prog", which expands to a C++ program.
	dnl This should either be a default one or the one specified.
	dnl Note that macros are not local, but there is a stack so push
	dnl the definition on to the stack to prevent clobbering a definition
	dnl that might already exist.
	m4_if([$2],[],[pushdef(prog, [int main(){}])], [pushdef(prog, [$2])])
	
	flag_test=0
	
	dnl See if the compiler runs
	AC_COMPILE_IFELSE([AC_LANG_SOURCE([prog])], [flag_test=1],[flag_test=0])
	
	dnl De-clobber the "prog" macro
	popdef([prog])

	if test $flag_test = 1
	then
		AC_MSG_RESULT(yes)
	else
		AC_MSG_RESULT(no)
		dnl The flag doesn't work, so restore the old CXXFLAGS
		CXXFLAGS="$save_CXXFLAGS"
	fi
])


dnl Add flags, but only if the flags weren't overridden already
if test "$CXXFLAGS" == "-g -O2"
then
	
	TEST_AND_SET_CXXFLAG(-Wall)
	TEST_AND_SET_CXXFLAG(-Wextra)
	TEST_AND_SET_CXXFLAG(-W)
	TEST_AND_SET_CXXFLAG(-O3)
	TEST_AND_SET_CXXFLAG(-ggdb)
	TEST_AND_SET_CXXFLAG(-fnot-a-valid-flag)
fi

