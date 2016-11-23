/************************************************************************************/
/* Copyright (c) 2008-2011 The Department of Arts and Culture,                      */
/* The Government of the Republic of South Africa.                                  */
/*                                                                                  */
/* Contributors:  Meraka Institute, CSIR, South Africa.                             */
/*                                                                                  */
/* Permission is hereby granted, free of charge, to any person obtaining a copy     */
/* of this software and associated documentation files (the "Software"), to deal    */
/* in the Software without restriction, including without limitation the rights     */
/* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell        */
/* copies of the Software, and to permit persons to whom the Software is            */
/* furnished to do so, subject to the following conditions:                         */
/* The above copyright notice and this permission notice shall be included in       */
/* all copies or substantial portions of the Software.                              */
/*                                                                                  */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR       */
/* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,         */
/* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE      */
/* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER           */
/* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,    */
/* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN        */
/* THE SOFTWARE.                                                                    */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* AUTHOR  : Aby Louw                                                               */
/* DATE    : 25 March 2008                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* System specific libraries.                                                       */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_SYSLIBS_H__
#define _SPCT_SYSLIBS_H__


/**
 * @file syslibs.h
 * Includes system specific libraries based on CMAKE configuration.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/spctconfig.h"


/************************************************************************************/
/*                                                                                  */
/* System specific includes (depends on cmake configuration in spctconfig.h)        */
/*                                                                                  */
/************************************************************************************/

/*
 * Include <windows.h> if build on _WIN32
 */
#ifdef SPCT_WIN32
#  include <windows.h>
#endif

/*
 * Include <BaseTsd.h> and define ssize_t if build on MSVC
 */
#ifdef SPCT_MSVC
#  include <BaseTsd.h>
typedef SSIZE_T ssize_t;
#endif

/*
 * Include <sys/types.h> if it is available.
 */
#ifdef SPCT_HAVE_SYS_TYPES_H
#  include <sys/types.h>
#endif /* SPCT_HAVE_SYS_TYPES_H */


/*
 * Include <sys/stat.h> if it is available.
 */
#ifdef SPCT_HAVE_SYS_STAT_H
#  include <sys/stat.h>
#endif /* SPCT_HAVE_SYS_STAT_H */


/*
 * Include <unistd.h> if it is available.
 */
#ifdef SPCT_HAVE_UNISTD_H
#  include <unistd.h>
#endif /* SPCT_HAVE_UNISTD_H */


/*
 * Include <limits.h> if it is available.
 */
#ifdef SPCT_HAVE_LIMITS_H
#  include <limits.h>
#endif /* SPCT_HAVE_LIMITS_H */


/*
 * Include <fcntl.h> if it is available.
 */
#ifdef SPCT_HAVE_FCNTL_H
#  include <fcntl.h>
#endif /* SPCT_HAVE_FCNTL_H */


/*
 * Include <sys/mman.h> if it is available.
 */
#ifdef SPCT_HAVE_SYS_MMAN_H
#  include <sys/mman.h>
#endif /* SPCT_HAVE_SYS_MMAN_H */


#endif /* _SPCT_SYSLIBS_H__ */
