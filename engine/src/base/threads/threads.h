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
/* Threads Abstraction.                                                             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_THREADS_H__
#define _SPCT_THREADS_H__


/**
 * @file threads.h
 * Threads Abstraction.
 */


/**
 * @ingroup SBase
 * @defgroup SThreads Threads Abstraction
 * Defines a set of macros to access multi-threaded functionality. The
 * Speect Engine itself is not multi-threaded, but it does need access
 * to mutex locks to be thread safe, and thread id's for logging
 * purposes. Different multi-threaded implementations are supported by
 * implementing the following macros:
 *
 * <table>
 *  <tr>
 *   <td> @code void _S_MUTEXT_INIT(s_mutex *M, __FILE__, __LINE__) @endcode </td>
 *   <td> Initialize a mutex (see @ref s_mutex_init) </td>
 *  </tr>
 *  <tr>
 *   <td> @code void _S_MUTEX_DESTROY(s_mutex *M, __FILE__, __LINE__) @endcode </td>
 *   <td> Destroy a mutex (see @ref s_mutex_destroy) </td>
 *  </tr>
 *  <tr>
 *   <td> @code void _S_MUTEX_LOCK(s_mutex *M, __FILE__, __LINE__) @endcode </td>
 *   <td> Lock a mutex (see @ref s_mutex_lock) </td>
 *  </tr>
 *  <tr>
 *    <td> @code void _S_MUTEX_UNLOCK(s_mutex *M, __FILE__, __LINE__) @endcode </td>
 *    <td> Unlock a mutex (see @ref s_mutex_unlock) </td>
 *  </tr>
 *  <tr>
 *    <td> @code unsigned long _S_THREAD_ID(void) @endcode </td>
 *    <td> Get the calling thread id (see @ref s_thread_id) </td>
 *  </tr>
 * </table>
 *
 * and defining the appropriate structure to #s_mutex. See the
 * threads_win32.h, threads_pthreads.h and threads_none.h for
 * examples. The mutex functions will print an error message
 * to @c stderr and abort if it cannot create, lock, unlock or destroy
 * a mutex.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

/*
 * Include the multi-threaded implementation.
 * threads_impl.h is automatically created by the build system
 * from user options
 */
#include "base/threads/platform/threads_impl.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

/**
 * Definition of a opaque mutex structure.
 */
typedef s_mutex_t s_mutex;


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

#ifdef SPCT_USE_THREADS

/**
 * Declare a @a static #s_mutex. Declare a @a static #s_mutex with the
 * given name, based on the threads implementation.
 * @hideinitializer
 *
 * @param NAME The name of the static #s_mutex.
 */
#  define S_DECLARE_MUTEX_STATIC(NAME) static s_mutex NAME

/**
 * Declare a #s_mutex. Declare a #s_mutex with the given name,
 * based on the threads implementation.
 * @hideinitializer
 *
 * @param NAME The name of the static #s_mutex.
 */
#  define S_DECLARE_MUTEX(NAME) s_mutex NAME

#else /* !SPCT_USE_THREADS */

#  define S_DECLARE_MUTEX_STATIC(NAME)
#  define S_DECLARE_MUTEX(NAME)

#endif /* SPCT_USE_THREADS */


/**
 * Initialize a mutex. Initializes the given #s_mutex mutex using the
 * default mutex attributes.
 * @hideinitializer
 *
 * @param mutex #s_mutex mutex pointer to initialize.
 */
#define s_mutex_init(mutex)							\
	do {											\
		_S_MUTEX_INIT(mutex, __FILE__, __LINE__);	\
	} while (0)


/**
 * Destroy a mutex. Destroys the given #s_mutex mutex.
 * @hideinitializer
 *
 * @param mutex #s_mutex mutex pointer to destroy.
 */
#define s_mutex_destroy(mutex)							\
	do {												\
		_S_MUTEX_DESTROY(mutex, __FILE__, __LINE__);	\
	} while (0)


/**
 * Lock a mutex. Locks the given #s_mutex mutex so that
 * it can only be accessed by one thread.
 * @hideinitializer
 *
 * @param mutex #s_mutex mutex pointer to lock.
 */
#define s_mutex_lock(mutex)							\
	do {											\
		_S_MUTEX_LOCK(mutex, __FILE__, __LINE__);	\
	} while (0)


/**
 * Unlock a mutex. Unlocks the given #s_mutex mutex that has
 * previously been locked by #s_mutex_lock, so that it is accessiable
 * to all threads.
 * @hideinitializer
 *
 * @param mutex #s_mutex mutex pointer to unlock.
 */
#define s_mutex_unlock(mutex)						\
	do {											\
		_S_MUTEX_UNLOCK(mutex, __FILE__, __LINE__);	\
	} while (0)


/**
 * Get the calling thread id.
 * @hideinitializer
 *
 * @return The calling thread id as an unsigned long.
 */
#define s_thread_id() _S_THREAD_ID()


/************************************************************************************/
/*                                                                                  */
/* End external c declaration                                                       */
/*                                                                                  */
/************************************************************************************/
S_END_C_DECLS


/**
 * @}
 * end documentation
 */

#endif /* _SPCT_THREADS_H__ */

