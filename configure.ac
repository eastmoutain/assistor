#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT([assistor], [0.5], [wencheng.yang@cs2c.com])
AM_INIT_AUTOMAKE(assistor, 2.69)
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lcairo':
AC_CHECK_LIB([cairo], [main])
# FIXME: Replace `main' with a function in `-lcurl':
AC_CHECK_LIB([curl], [main])
# FIXME: Replace `main' with a function in `-lfontconfig':
AC_CHECK_LIB([fontconfig], [main])
# FIXME: Replace `main' with a function in `-lfreetype':
AC_CHECK_LIB([freetype], [main])
# FIXME: Replace `main' with a function in `-lrt':
AC_CHECK_LIB([rt], [main])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h sys/time.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([gettimeofday memset strndup strstr])

AC_OUTPUT(Makefile src/Makefile desktop/Makefile pixmaps/Makefile)
