#  Copyright 2005 Adam Jackson.
#
#  Permission is hereby granted, free of charge, to any person obtaining a
#  copy of this software and associated documentation files (the "Software"),
#  to deal in the Software without restriction, including without limitation
#  on the rights to use, copy, modify, merge, publish, distribute, sub
#  license, and/or sell copies of the Software, and to permit persons to whom
#  the Software is furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice (including the next
#  paragraph) shall be included in all copies or substantial portions of the
#  Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT.  IN NO EVENT SHALL
#  ADAM JACKSON BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
#  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
#  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
# Process this file with autoconf to produce a configure script

# Initialize Autoconf
AC_PREREQ([2.60])
AC_INIT([drihybris],
        [0.1.0],
        [https://github.com/NotKit/drihybris],
        [drihybris])
AC_CONFIG_SRCDIR([Makefile.am])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_AUX_DIR(.)
AC_USE_SYSTEM_EXTENSIONS

# Initialize Automake
AM_INIT_AUTOMAKE([foreign dist-bzip2])

# Require xorg-macros: XORG_DEFAULT_OPTIONS
m4_ifndef([XORG_MACROS_VERSION],
          [m4_fatal([must install xorg-macros 1.3 or later before running autoconf/autogen])])
XORG_MACROS_VERSION(1.3)
XORG_DEFAULT_OPTIONS

# Initialize libtool
AC_DISABLE_STATIC
AC_PROG_LIBTOOL

AH_TOP([#include "xorg-server.h"])

# Define a configure option for an alternate module directory
AC_ARG_WITH(xorg-module-dir,
            AS_HELP_STRING([--with-xorg-module-dir=DIR],
                           [Default xorg module directory [[default=$libdir/xorg/modules]]]),
            [moduledir="$withval"],
            [moduledir="$libdir/xorg/modules"])

AC_SUBST(moduledir)

# X Server SDK location is required to install drihybris header file
sdkdir=`$PKG_CONFIG --variable=sdkdir xorg-server`
AC_SUBST([sdkdir])

LIBDRM="libdrm >= 2.4.23"
LIBGL="gl >= 7.1.0"
LIBPIXMAN="pixman-1 >= 0.21.8"
LIBEGL="egl"
LIBGLESV2="glesv2"
LIBGBM="gbm"
LIBGBMv9="gbm >= 9"

AC_MSG_CHECKING([whether to include GLAMOR_GLES2 support])
AC_ARG_ENABLE(glamor-gles2,         AS_HELP_STRING([--enable-glamor-gles2], [Build glamor based on gles2 (default: no)]), [GLAMOR_GLES2="$enableval"], [GLAMOR_GLES2=no])
AC_MSG_RESULT([$GLAMOR_GLES2])

AC_MSG_CHECKING([whether to include GLAMOR Xv support])
AC_ARG_ENABLE(xv,         AS_HELP_STRING([--disable-xv], [Disable glamor Xv (default: enabled)]), [GLAMOR_XV="$enableval"], [GLAMOR_XV=yes])
AC_MSG_RESULT([$GLAMOR_XV])
AM_CONDITIONAL([GLAMOR_XV], [test "x$GLAMOR_XV" != xno])

AC_MSG_CHECKING([whether to enable DEBUG])
AC_ARG_ENABLE(debug,         AS_HELP_STRING([--enable-debug], [Build debug version glamor (default: no)]), [DEBUG="$enableval"], [DEBUG=no])
AC_MSG_RESULT([$DEBUG])
if test "x$DEBUG" = xyes; then
   AC_DEFINE(DEBUG, 1, [Enable DEBUG])
fi

if test "x$GLAMOR_GLES2" = xyes; then
   AC_DEFINE(GLAMOR_GLES2,1,[Build glamor over GLES2])
   PKG_CHECK_MODULES(GLESV2, $LIBGLESV2)
   REQUIRED_LIBS="$REQUIRED_LIBS $LIBGLESV2"
   GLAMOR_GL_CFLAGS="$GLESV2_CFLAGS -DGLAMOR_GLES2"
else
   AC_DEFINE(GLAMOR_GL,1,[Build glamor over GL])
   PKG_CHECK_MODULES(GL, $LIBGL)
   REQUIRED_LIBS="$REQUIRED_LIBS $LIBGL"
   GLAMOR_GL_CFLAGS="$GL_CFLAGS -DGLAMOR_GL"
fi

AC_SUBST([GLAMOR_GL_CFLAGS])

AM_CONDITIONAL([GLAMOR_GLES2], [test "x$GLAMOR_GLES2" = xyes])
if test "x$GLAMOR_XV" = xyes; then
   AC_DEFINE(GLAMOR_XV,1,[Build Xv support])
fi

# Store the list of server defined optional extensions in REQUIRED_MODULES
XORG_DRIVER_CHECK_EXT(RANDR, randrproto)
XORG_DRIVER_CHECK_EXT(RENDER, renderproto)
XORG_DRIVER_CHECK_EXT(XV, videoproto)

# Obtain compiler/linker options for the driver dependencies
PKG_CHECK_MODULES(XORG, [xorg-server >= 1.4.99.901] xproto fontsproto $REQUIRED_MODULES)

# Checks for libraries.

AC_CONFIG_FILES([
                Makefile
                src/Makefile
])
AC_OUTPUT
