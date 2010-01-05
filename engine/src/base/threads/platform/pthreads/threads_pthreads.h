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
/* POSIX threads.                                                                   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_POSIX_THREADS_H__
#define _SPCT_POSIX_THREADS_H__


/**
 * @file threads_pthreads.h
 * POSIX threads.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <pthread.h>  /* POSIX threads */
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

/* defines of the POSIX wrapper functions */

#define _S_MUTEX_INIT(M, __FILE__, __LINE__) s_pthread_mutex_init(M)


#define _S_MUTEX_DESTROY(M, __FILE__, __LINE__) s_pthread_mutex_destroy(M, __FILE__, __LINE__)


#define _S_MUTEX_LOCK(M, __FILE__, __LINE__) s_pthread_mutex_lock(M, __FILE__, __LINE__)


#define _S_MUTEX_UNLOCK(M, __FILE__, __LINE__) s_pthread_mutex_unlock(M, __FILE__, __LINE__)


#define _S_THREAD_ID() s_pthread_self()


/************************************************************************************/
/*                                                                                  */
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

typedef pthread_mutex_t s_mutex_t;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/* wrapper for pthread_mutex_init */
S_API void s_pthread_mutex_init(s_mutex_t *m);


/* wrapper for pthread_mutex_destroy */
S_API void s_pthread_mutex_destroy(s_mutex_t *m, const char *file_name, int line_number);


/* wrapper for pthread_mutex_lock */
S_API void s_pthread_mutex_lock(s_mutex_t *m, const char *file_name, int line_number);


/* wrapper for pthread_mutex_unlock */
S_API void s_pthread_mutex_unlock(s_mutex_t *m, const char *file_name, int line_number);


/* wrapper for pthread_self */
S_API unsigned long s_pthread_self(void);


/************************************************************************************/
/*                                                                                  */
/* End external c declaration                                                       */
/*                                                                                  */
/************************************************************************************/
S_END_C_DECLS


#endif /* _SPCT_POSIX_THREADS_H__ */

