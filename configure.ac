#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.67])
AC_INIT([fcolladaCE], [1.0], [])
AC_CONFIG_SRCDIR([FColladaPlugins/FArchiveXML/FAXGeometryExport.cpp])
AM_INIT_AUTOMAKE([-Wall -Werror foreign subdir-objects])
AM_PROG_AR
AC_PROG_LIBTOOL

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
PKG_CHECK_MODULES([DEPS], [libxml-2.0 >= 2.6.29])

# Checks for header files.
AC_CHECK_HEADERS([float.h malloc.h stdint.h stdlib.h string.h unistd.h wchar.h wctype.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_INT16_T
AC_TYPE_INT32_T
AC_TYPE_INT64_T
AC_TYPE_INT8_T
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_TYPE_UINT64_T
AC_TYPE_UINT8_T

# Checks for library functions.
AC_HEADER_MAJOR
AC_CHECK_FUNCS([floor memmove memset mkdir pow strstr])

AC_CONFIG_FILES([Makefile fcollada.pc])
AC_OUTPUT
