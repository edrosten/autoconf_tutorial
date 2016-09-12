#A brief introduction to Autoconf (2nd edition)

## Preface

A long time ago I  autoconf-ized the build system for 
[libCVD](http://www.edwardrosten.com/cvd/index.html).
It was one of the
best decisions I made with that library. It took me a while because there wasn't
a good “getting started” guide, so
[I wrote one](http://www.edwardrosten.com/code/autoconf/). It's been the
top link for “autoconf tutorial” on google for a while.

Don't go there.

The trouble is I wrote it after my first Autoconf project back in 2005 or so.
I've learned how to use it better since then and it's also moved on. It'll get
you started, but probably teach you bad habits. I've also since come to the
conclusion that while autoconf is entirely separate from make and can be used
without it, it makes much more sense to have examples with makefiles because
they are almost always used together.


## Introduction

Autoconf is a system for generating a script which will automatically determine
the presence of things which your project either depends upon, or can make use
of. Once these have been determined, autoconf can export this information in
various formats suitable for your project to use. Autoconf scripts themselves
are written in a combination of shell and M4 (a macro processing language
considerably more capable than the C preprocessor). Autoconf essentially runs
the preprocessor on your script to produce a portable shell script which will
perform all the requisite tests, produce handy log files, preprocess template
files, for example to generate `Makefile` from `Makefile.in` and and take a standard
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
* All public Autoconf M4 macros look like `A[CST]_??????`. There may be private versions starting with `_`, but it's generally a bad idea to use them.
* M4 arguments are quoted with `[` and `]`. There is NO WAY to escape these, however, you have several options if you wish to insert `[`'s or `]`'s:
  1. Use a ‘Quadrigaph’. `@<:@` gives you `[` and `@>:@` gives you `]`. Those look as nasty as they sound.
  2. Balance your quotes. M4 will turn `[[]]` in to `[]`. Beware of using this in arguments to macros. Sometimes, you need to double quote as well (`[[[]]]`).  This is not as nasty as it sounds, it's much, much nastier. M4 programming is quite fun in that you have such a huge of acievement when the quotes finally work correctly.
  3. Change the quoting using: `changequote(<<,>>)` to change the quoting to `<<` and `>>`. The autoconf documentation (rightly, in my opinion) warns against the (over) use of this, since it can lead to unexpected results. This is also as nasty... see the pattern here?
  4. Avoid `[` and `]` whereever possible.
  As a result, you can't easily use the shell command `[` to perform tests. You have to use test instead.
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
[It's now going to be autoconfized](ex_03). First, we rename `Makefile` to
`Makefile.in` and edit it look like this:
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

The AVR is a “non hosted” platform in that it has no OS. That means that you
can't expect obvious bits of the C++ standard library to be there. While this is
an obscure case, it does illustrate the next topic very nicely which is:

## Performing basic tests

So first we need some tests to perform. The iostream example is a little 
contrived, so I'll bring in a really common library, [zlib](http://zlib.com).
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
At this point you might wonder, “Why not save typing and just use `@LIBS@`
everywhere?”. It's a good question. The answer is that you can, but if you ever
end up debugging things and you want to make a quick, temporary change by
editing the generated Makefile, you will curse yourself because instead of changing things in
one place, you have to change it everywhere.

So, now, put the following lines in the `configure.ac` file:
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
Now try misspelling “deflate” and rerunning autoconf and configure. You'll see
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
absence of `-lz` in the library flags), but they are independent. You want to
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

I'm now going to make zlib an optional dependency.

### Compiling alternate files

The first thing to do is to [provide two alternatives](ex_06), one for zlib and
one without. Obviously in this case the example is a little contrived but we
want to compile [hello_libz.cc](ex_06/hello_libz.cc) if we find zlib and
[hello_no_libz.cc](ex_06/hello_no_libz.cc) if we don't. Either way, `main()` is
the same, and just calls the `hello()` function.

Now, alter [Makefile.in](ex_06/Makefile.in) so it can use another parameter that
we get from `configure`:
```make
objs=@objs@

program: program.o $(objs)
	$(LD) -o $@ $^ $(LDFLAGS) $(LIBS)
```
And alter [configure.ac](ex_06/configure.ac) so it populates @objs@ with either
one object file or another.
```autoconf
dnl List of object files as determined by autoconf
object_files=""

a=0
AC_CHECK_HEADERS(zlib.h, [], [a=1])
AC_SEARCH_LIBS(deflate, z, [], [a=1])

if test $a == 0
then
	object_files="$object_files hello_libz.o"
else
	object_files="$object_files hello_no_libz.o"
fi

dnl Make AC_OUTPUT substitute the contents of $object_files for @objs@
AC_SUBST(objs, ["$object_files"])
```

Now `configure` will choose between one of the two source files to compile and
link, depending on the result of the test. The complete program will work either
way.

### Conditional compilation

The [other main way](ex_07) of having autoconf affect the build is via conditional
compilation. You could export `-D` definitions to the CXXFLAGS or a custom
variable. That's a bad idea because your program will have a dependency on the
arguments, and make doesn't track or examine those dependencies. The standard
way is to generate a `config.h` file instead. The process is similar to, but not
identical to creating a makefile. The relevant part of
[configure.ac](ex_07/configure.ac) is:
```autoconf
a=0
AC_CHECK_HEADERS(zlib.h, [], [a=1])
AC_SEARCH_LIBS(deflate, z, [], [a=1])

if test $a == 0
then
	AC_DEFINE(HAVE_LIBZ)
fi

AC_CONFIG_HEADERS(config.h)
```
`AC_DEFINE` will cause a #define to be written in `config.h`. **IMPORTANT:**
`AC_CONFIG_HEADERS` must come before `AC_OUTPUT` otherwise it willbe silently
ignored. [config.h.in](ex_07/config.h.in) is a template of an include file, but
substitutions are done on `#undef` lines, it must contain:
```C
#undef HAVE_LIBZ
```
If `HAVE_LIBZ` is exported by `configure`, then that line is commented out. The
[C++ code](ex_07/program.c) can then `#include <config.h>` and use the macros.

#### :neckbeard: Ed's Soapbox :neckbeard:

Avoid `config.h` as much as possible. Disadvantages are:

1. Every file depending on `config.h` must be rebuilt if `config.h` changes even
if the change is irrelevant.
2. Conditional compilation is ugly and can lead to tangled messes of dead code.
3. It's generally good practice to avoid the preprocessor where possible.
4. If you're building a library and you've got a `config.h` included from the
public headers, then everything using the library has to be rebuilt if
`config.h` changes.
5. More source files means better parallel builds.

This isn't a rigid rule. Sometimes the clearest, simplest and most elegant
solution to some gnarly cross-platform portability problem involves the
preprocessor. In most cases though however, the cleanest portable designs will
rely on separating off platform specific parts into different modules and files.


### Nicer dependencies with --with and --enable

So if you've made it this far, you probably noticed that debugging and checking
the scripts was a bit of a pain, because you had to edit them to make one of the
tests fail then rerun autoconf, then put it back again. Autoconf provides two
methods of configuration. They are almost identical in function, but the
conventions are:
* `--enable-foo` enable feature foo. This is typically used to enable or disable
  an entire feature, such as `--enable-gui`. 
* `--with-bar` use dependency bar. This is typically used to switch on/off a
  dependency. That may have an effect on a whole feature. For example
  `--without-gtk` might end up disabling the GUI if no other toolkit is
  available.

If a script understands `--with-bar`, it will also understand `--without-bar`
which is exactly equivalent to `--with-bar=no`. That means you can also supply
arguments to `with`. `--enable-foo` works in the same way.  Both styles simply
set an environment variable. You can then act on that with the usual shell
scripting. 

The format is:
```autoconf
AC_ARG_WITH(zlib, [AS_HELP_STRING([--without-zlib], [do not use zlib])])
```
The first argument is the feature, and it will set `$with_zlib` to the value
specified (blank if the argument is not used). The second argument is the text
which appears if `./configure --help` is run. `AS_HELP_STRING` simply makes
everything line up and look pretty. Much like the other tests, you can also
specify actions for if the commandline argument is present or absent.


[This example](ex_08) is very similar to [Example 6](ex_06), except that the
relevant part of [configure.ac](ex_08/configure.ac) is now:
```configure
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

```
Now, if `--without-zlib` is specified then it doesn't even try to run the tests,
and carries on as if the tests have failed.

## Out of tree builds

Out of tree builds are one of those features that are rarely used, but probably
ought to be used more. They're kind of handy because with the same source tree,
you can build various versions (standard, debugging, cross compiled) from a
single source tree, without having to keep multiple copies in sync. It's also so
easy to do that you may as well do it.

Autoconf supports it by providing @srcdir@ which tells you the directory of the
configure script and of course by extension the source code. That's it as far as
autoconf goes, it's completely automatic.

You then need to use the virtual path (vpath) feature of make. Essentially, add
the line
```make
VPATH=@srcdir@
```
to Makefile.in, and now make will more or less overlay VPATH on the current
directory when it comes to file lookups.

That's all there is to is. Go into [example 9](ex_09) and run autoconf to create
the configure script. Now go into /tmp and type something like:
```shell
/path/to/autoconf_tutorial/ex_09/configure && make && ./program | zcat
```
It should work just as well as if you ran configure and make from within the
source directory.

## Moving on

At this point you now have a basic, but fully featured autoconf build system.
Most of what to do can probably be accomplished with what you've covered so far,
however you may come across things which aren't covered.

If you do get into writing your own tests, then it's a very good idea to build
off the autoconf macros if at all possible. I know from experience that it's an
especially bad idea to write tests that probe the machine you're running the
autoconf script from because then you won't be able to deploy the resulting
program on a different mahchine (for example if you're cross compiling).

Even without machine specific tests, there are other things you might want to
look for or other parts of the build system you might want to probe.

## Other tests

At this point I'm just going to list a selection of what's available. The usage
should be reasonably obvious from the names. Quite a few of the marcros have
`IF_ELSE` in the name, which means they just give you places to insert code for
success or failure. For quite a lot of common cases, there are more specialised
macros to help. Note again, this is a small selection to give a flavour. Since
this is a tutorial I can't cover them all, so it's best to browse 
[the manual](https://www.gnu.org/software/autoconf/manual/autoconf.html) at this
point.
* `AC_COMPILE_IFELSE`  --- useful for testing C++ header only libraries
* `AC_PREPROCESS_IFELSE`
* `AC_LINK_IFELSE`
* `AC_RUN_IFELSE` --- this one is tricky. If you're cross compiling, you
  probably can't run a program you've just compiled, so you need to provide
  the script with some way of making a decision in that case.
* `AC_PROG_AWK` --- find a working AWK. Versions exist for many of the common
  tools.
* `AC_CHECK_TYPES([long long])`, `AC_TYPE_UINT8_T` ---  check for various
  typedefs
* `AC_OPENMP`

## Other people's macros

Autoconf provides tools, but doesn't provide tests for everything, especially
outside of C.

## Writing your own tests



## What about automake and libtool?

Er... yes they exist.

I'm not going to cover them because I don't use them for a variety of reasons.
Back when I learned autoconf, automake always generated recursive Makefiles.
[Those are bad](aegis.sourceforge.net/auug97.pdf) for many reasons but
especially if you do parallel builds with `make -J 4`. I had a dual processor
machine back when multiple processors or cores were rare so it mattered to me
more than most people. I also like GNU Make particularly, and I'm entirely happy
with writing Makefiles to the point where I don't feel the need for extra
automation.

Libtool was (for me) a relatively frequent and hard to debug source of build
errors in other people's packages. In fairness, this may well have been due to
misuse of libtool, but either way I never learned it. Libtool was especially
important back in the olden days when systems were very diverse, and when static
libraries were measureably more efficient than dynamic ones. These days I tend
to compile my .a files with PIC (so I can blob them into a .so plugin easily),
and platforms with significant differences seem to have reduced to Linux/\*BSD,
OSX and Cygwin and MinGW though in the latter cases it will also work the same
way as Linux. So, in short I've not felt I'd gain all that much by learning it.









