#A brief introduction to Autoconf (2nd edition)

## Peface

A long time ago I  autoconf-ized the build system for 
[libCVD](http://www.edwardrosten.com/cvd/index.html).
It was one of the
best decisions I made with that library. It took me a while because there wasn't
a good "getting started" guide, so
[I wrote one](http://www.edwardrosten.com/code/autoconf/). It's been the
top link for "autoconf tutorial" on google for a while.

Don't go there.

The trouble is I wrote it after my first Autoconf project back in 2005 or so.
I've learned how to use it better since then and it's also moved on. It'll get
you started, but probably teach you bad habits.


## Introduction

Autoconf is a system for generating a script which will automatically determine
the presence of things which your project either depends upon, or can make use
of. Once these have been determined, autoconf can export this information in
various formats suitable for your project to use. Autoconf scripts themselves
are written in a combination of shell and M4 (a macro processing language
considerably more capable than the C preprocessor). Autoconf essentially runs
the preprocessor on your script to produce a portable shell script which will
perform all the requisite tests, produce handy log files, preprocess template
files, for example to generate Makefile from Makefile.in and and take a standard
set of command line arguments.

There are several tools in the suite, including automake (for automatically
generating makefiles) and autoscan (for scanning source trees to make configure
scripts). I'm not going to cover them.  However, while autoconf has nothing to
do with Make, by far the most common use of it is to generate a Makefile. I only
use GNU Make, and so the examples may well use GNU Make only features.

In order to read this tutorial, a knowledge of shell scripting is assumed, but
not essential for the basic examples. No knowledge of M4 is needed, but it worth
knowing especially if you want to write your own custom tests which you wish to
reuse.

##Hello, World.

Grab the code in [example 1](ex_01), or put this in configure.ac:

```autoconf
AC_INIT(myconfig, version-0.1)
AC_MSG_NOTICE([Hello, world.])

```
Now compile it and run it
```bash
autoconf
./configure
```
And you get:
```text
ex_01 $./configure 
configure: Hello, world.
ex_01 $
```
Alriiight! :)

Now that that's out of the way, there's a few basic things.
* All public Autoconf M4 macros look like A[CST]_??????. There may be private versions starting with _, but it's generally a bad idea to use them.
* M4 arguments are quoted with [ and ]. There is NO WAY to escape these, however, you have several options if you wish to insert ['s or ]'s:
  1. Use a `Quadrigaph'. @<:@ gives you [ and @>:@ gives you ]. Those look as nasty as they sound.
  2. Balance your quotes. M4 will turn [[]] in to []. Beware of using this in arguments to macros. Sometimes, you need to double quote as well ([[[]]]).  This is also as nasty as it sounds.
  3. Change the quoting using: changequote(<<,>>) to change the quoting to << and >>. The autoconf documentation (rightly, in my opinion) warns against the (over) use of this, since it can lead to unexpected results. This is also as nasty... see the pattern here?
  4. Avoid [ and ] whereever possible.
  As a result, you can't easily use the shell command [ to perform tests. You have to use test instead.
* If you make bad shell code, the errors probably won't appear until you run the configure script.
* Configure scripts are almost always called configure.ac
* Bear in mind that this is a tutorial. Many macros provide optional arguments which allow you to change the way of doing things. There may be many more ways of solving a problem than I've covered here.

## A basic project

Since the purpose of autoconf is compiling projects, it makes most sense to
start with [a simple project](ex_02). The project as it stands just has a
Makefile. Go in to [ex_02](ex_02) and do:
```bash
make
./program
```
If all goes well and you have a C++ compiler installed, then you should get:
```text
Hello, I am a program
```
[It's now going to be autoconfized](ex_03). First, we rename Makefile to
Makefile.in and make it look like this:
```make
CXX=@CXX@
LD=@CXX@
CXXFLAGS=@CXXFLAGS@

program: program.o
	$(LD) -o $@ $^

.PHONY: clean
clean:
	rm -f program *.o

```
It's more or less the same, but instead of hard-wiring the compiler names and
flags and so on, we put in placeholders that the configure script will
substitute. All the placeholders are of the form <tt>@foo@</tt>. I've also added
a "make clean" rule.

We now need a matching configure script:
```autoconf
AC_INIT(program, 1.0)

dnl Switch to a C++ compiler, and check if it works.
AC_PROG_CXX

dnl Process Makefile.in to create Makefile
AC_OUTPUT(Makefile)
```
By the way, <tt>dnl</tt> in autoconf is a comment. That's because of M4 and M4
is... best not to go there right now.

Now, compile the configure script by typing "autoconf" and you're ready to
perform the standard build incantation:
```bash
./configure &&  make
```
I get the output:
```text
checking for g++... g++
checking whether the C++ compiler works... yes
checking for C++ compiler default output file name... a.out
checking for suffix of executables... 
checking whether we are cross compiling... no
checking for suffix of object files... o
checking whether we are using the GNU C++ compiler... yes
checking whether g++ accepts -g... yes
configure: creating ./config.status
config.status: creating Makefile
g++ -g -O2   -c -o program.o program.cc
g++ -o program program.o

```
That's basically it. You now have a fully armed and operational configure
script. Because autoconf is a decent system, it already comes with all the
different features. You can, for example change the compiler flags:
```text
make clean
./configure CXXFLAGS="-Wall -Wextra" && make
```
or, you can change the compiler if you like:
```bash
make clean
./configure CXX=g++-5 && make 

```
Naturally that will fail if you don't have that compiler. You can even cross
compile! First, install the AVR toolchain if you can, then run:
```bash
./configure --host avr && make
```
You should get the output:
```text
checking for avr-g++... avr-g++
checking whether the C++ compiler works... yes
checking for C++ compiler default output file name... a.out
checking for suffix of executables... 
checking whether we are cross compiling... yes
checking for suffix of object files... o
checking whether we are using the GNU C++ compiler... yes
checking whether avr-g++ accepts -g... yes
configure: creating ./config.status
config.status: creating Makefile
avr-g++ -g -O2   -c -o program.o program.cc
program.cc:1:20: fatal error: iostream: No such file or directory
 #include <iostream>
                    ^
compilation terminated.
make: *** [program.o] Error 1
```
Wait, what?

The AVR is a "non hosted" platform in that it has no OS. That means that you
can't expect obvious bits of the C++ standard library to be there. While this is
an obscure case, it does illustrate the next topic very nicely which is:

## Performing basic tests

So first we need some tests to perform. The iostream example is a little 
contrived, so I'll bring in a really common library, [http://zlib.com](zlib).
The new program [example 5](ex_05) now outputs its data gzip compressed.

First, we need to modify the [Makefile.in](ex_05/Makefile.in) to use the library
flags. First, we add the template substitutions:
```Make
LDFLAGS=@LDFLAGS@
LIBS=@LIBS@
```
then we change the build line to use them:
```Make
program: program.o
        $(LD) -o $@ $^ $(LDFLAGS) $(LIBS)
```
At this point you might wonder, "Why not save typing and just use @LIBS@
everywhere?". It's a good question. The answer is that you can, but if you ever
end up debugging things and you want to make a quick, temporary change by
editing the Makefile, you will curse yourself because instead of changing it in
one place, you have to change it everywhere.

So, now, put the following lines in the configure.ac file:
```autoconf
AC_CHECK_HEADERS(zlib.h)
AC_SEARCH_LIBS(deflate, z)
```
The first function compiles a minimal program against each of the specified
headers (you can give more than one), and the second checks for the function
"deflate" in any one of the listed libraries.
If you were to run the configure script, you'd get something like this in the
output:
```text
checking zlib.h usability... yes
checking zlib.h presence... yes
checking for zlib.h... yes
checking for library containing deflate... -lz
```
That's pretty good, and you can now build the program, however, if the test
fails then the configure script will continue and give you a makefile which
would fail to build your program.

So now what?

At this point you need to do something with the results of the test. What you
want to do, and how you want to do it depend strongly on what you're trying to
achieve. The simplest case is a hard dependency, where failure to find the
library prevents the configure script from completing.

Both the functions above set various variables on success, in this case:
```shell
ac_cv_header_zlib_h=yes
ac_cv_search_deflate=-lz
```
You can look in config.log, that has a list of everything which is defined. You
can perform tests with shell logic on those if you like. However, those
functions also provide two optional arguments: action on success and action on
failure.

Probably the easiest solution is to do this:

```autoconf
AC_CHECK_HEADERS(zlib.h, [], [AC_ERROR([A working zlib is required])])
AC_SEARCH_LIBS(deflate, z, [], [AC_ERROR([A working zlib is required])])
```
Now try misspelling "deflate" and rerunning autoconf and configure. You'll see
it fail like this:
```text
checking zlib.h usability... yes
checking zlib.h presence... yes
checking for zlib.h... yes
checking for library containing deflte... no
configure: error: A working zlib is required
```

## More advanced test results

Sometimes, dependencies are optional. There are of course an infinite number of
ways of dealing with those. The two common ones are via conditional compilation
(i.e. setting macros) or via choosing between different source files. I strongly
prefer the latter if possible.

Now, the tests so far will set things which can be used (like the presence or
absence of -lz in the library flags), but they are independent. You want to
enable the dependency if and only if all relavent tests pass. I find the easiest
way of doing that is using a construct like this:

```autoconf
a=0
AC_CHECK_HEADERS(zlib.h, [], [a=1])
AC_SEARCH_LIBS(deflate, z, [], [a=1])

if test $a == 0
then
	dnl Thing to do if the library is present
	;
else
	dnl Thing to do if the library is present
	;
fi

```



