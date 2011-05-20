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
/* DATE    : 5 October 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* An abstract container class.                                                     */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_CONTAINER_H__
#define _SPCT_CONTAINER_H__


/**
 * @file container.h
 * An abstract container class.
 */


/**
 * @ingroup Speect
 * @defgroup SContainer Containers
 * Container abstract data type, stores a collections of other objects.
 * @todo explain concurrency
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/utils/types.h"
#include "base/errdbg/errdbg.h"
#include "base/objsystem/objsystem.h"
#include "containers/iterator.h"


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

/**
 * @name Utility Macros
 * @{
 */


/**
 *
 * @hideinitializer
 * Return the given #SContainer child class object as a container
 * object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SContainer* type.
 *
 * @note This casting is not safety checked.
 */
#define S_CONTAINER(SELF)    ((SContainer *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SContainer,
 * see full description #S_CONTAINER_CALL for usage.
 *
 * @param SELF The given #SContainer*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_CONTAINER_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters
 * passed to the object function @c func.
 */
#define S_CONTAINER_CALL(SELF, FUNC)					\
	((SContainerClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SContainer
 * can be called.
 *
 * @param SELF The given #SContainer*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_CONTAINER_METH_VALID(SELF, FUNC)		\
	S_CONTAINER_CALL(SELF, FUNC) ? TRUE : FALSE


/**
 * @hideinitializer
 * Get an iterator to the given #SContainer object. If
 * @c SPCT_DO_SAFE_CAST is defined then the given object, will be safely
 * cast to #SContainer and, if successful, #SContainerGetIterator
 * called. Otherwise this is just a wrapper with an unsafe cast to
 * #SContainerGetIterator.
 *
 * @param SELF The given #SContainer* or child object.
 * @param ERROR Pointer to error code.
 *
 * @return #SIterator to @c SELF.
 *
 * @note Of course @c SELF must be a child type of #SObject, or else
 * the behaviour will be undefined.
 */
#ifdef SPCT_DO_SAFE_CAST
#   define S_ITERATOR_GET(SELF, ERROR) _s_container_get_iterator_check(SELF, ERROR)
#else /* !SPCT_DO_SAFE_CAST */
#   define S_ITERATOR_GET(SELF, ERROR) SContainerGetIterator(S_CONTAINER(SELF), ERROR)
#endif /* SPCT_DO_SAFE_CAST */


/**
 * @}
 */


/*
 * Used internally by container implementations to serialize
 * access to container data.
 */
#define S_LOCK_CONTAINER												\
	s_mutex_lock(&(S_CONTAINER(self)->container_mutex))


#define S_UNLOCK_CONTAINER												\
	s_mutex_unlock(&(S_CONTAINER(self)->container_mutex))


/************************************************************************************/
/*                                                                                  */
/* SContainer definition                                                            */
/*                                                                                  */
/************************************************************************************/

/**
 * The SContainer structure.
 * An abstract data type which is a collections of other objects.
 * @extends SObject
 */
typedef struct SContainer
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject    obj;

	/**
	 * @protected Locking mutex.
	 */
	S_DECLARE_MUTEX(container_mutex);
} SContainer;


/************************************************************************************/
/*                                                                                  */
/* SContainerClass definition                                                       */
/*                                                                                  */
/************************************************************************************/

/**
 * The SContainerClass structure.
 * @extends SObjectClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SObjectClass.
	 */
	SObjectClass  _inherit;

	/* Class methods */
	/**
	 * @protected Get iterator function pointer.
	 * Return an iterator to the first object of the container.
	 *
	 * @param self The given container.
	 * @param error Error code.
	 *
	 * @return Iterator to the first object of the container. If there
	 * are no objects in the container, then @c NULL must be
	 * returned.
	 */
	SIterator  *(*get_iterator)(const SContainer *self, s_erc *error);
} SContainerClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Get an iterator that points to the first object of the
 * container. If the container is empty then @c NULL is returned.
 * @public @memberof SContainer
 *
 * @param self The container for which an iterator is requested.
 * @param error Error code.
 *
 * @return Iterator that points to first object in container or @c
 * NULL if the container is empty.
 *
 * @sa #S_ITERATOR_GET
 */
S_API SIterator *SContainerGetIterator(const SContainer *self, s_erc *error);


/**
 * Utility function for macro #S_ITERATOR_GET.
 * If @c SPCT_DO_SAFE_CAST is defined then this function is called and
 * it tries to cast the @c self variable to #SContainer. If it fails
 * it sets an error an returns @c NULL.
 * @private
 *
 * @param self The SContainer type variable to get an iterator from.
 * @param error Error code.
 *
 * @return Iterator to container is cast was successful, else @c NULL.
 */
S_API SIterator *_s_container_get_iterator_check(const void *self, s_erc *error);


/**
 * @ingroup SContainers
 * Add the SContainer class to the object system.
 * @private @memberof SContainer
 *
 * @param error Error code.
 */
S_LOCAL void _s_container_class_add(s_erc *error);


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

#endif /* _SPCT_CONTAINER_H__ */
