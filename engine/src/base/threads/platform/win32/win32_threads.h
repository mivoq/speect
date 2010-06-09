/************************************************************************************/
/* Copyright (c) 2008-2009 The Department of Arts and Culture,                      */
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
/* WIN32 threads.                                                                   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_WIN32_THREADS_H__
#define _SPCT_WIN32_THREADS_H__


/**
 * @file win32_threads.h
 * WIN32 threads.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <windows.h>  /* Windows threads */
#include "include/common.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/* defines of the WIN32 thread wrapper functions */

#define _S_MUTEX_INIT(M, __FILE__, __LINE__) s_win32_mutex_init(M, __FILE__, __LINE__)


#define _S_MUTEX_DESTROY(M, __FILE__, __LINE__) s_win32_mutex_destroy(M, __FILE__, __LINE__)


#define _S_MUTEX_LOCK(M, __FILE__, __LINE__) s_win32_mutex_lock(M, __FILE__, __LINE__)


#define _S_MUTEX_UNLOCK(M, __FILE__, __LINE__) s_win32_mutex_unlock(M, __FILE__, __LINE__)


#define _S_THREAD_ID() s_win32_thread_self()


/************************************************************************************/
/*                                                                                  */
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

/*
 * s_mutex structure for win32 threads.
 */
typedef struct win32_api_mutex_s s_mutex_t;


/*
 * Structure of the win32 mutex. We replicate the API's of the pthread library,
 * therefore the need for this structure.
 */
struct win32_api_mutex_s
{
	HANDLE mutex;         /* An opague handle to a mutex */
	int    destroyed;     /* Destroyed flag              */
	int    init;          /* Initialized flag            */
	int    locked;        /* Locked flag                 */
};


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/* wrapper for win32 mutex_init */
S_API void s_win32_mutex_init(s_mutex_t *m, const char *file_name, int line_number);


/* wrapper for win32 mutex_destroy */
S_API void s_win32_mutex_destroy(s_mutex_t *m, const char *file_name, int line_number);


/* wrapper for win32 mutex_lock */
S_API void s_win32_mutex_lock(s_mutex_t *m, const char *file_name, int line_number);


/* wrapper for win32 mutex_unlock */
S_API void s_win32_mutex_unlock(s_mutex_t *m, const char *file_name, int line_number);


/* wrapper for win32 thread id */
S_API unsigned long s_win32_thread_self(void);


/************************************************************************************/
/*                                                                                  */
/* End external c declaration                                                       */
/*                                                                                  */
/************************************************************************************/
S_END_C_DECLS


#endif /* _SPCT_WIN32_THREADS_H__ */

