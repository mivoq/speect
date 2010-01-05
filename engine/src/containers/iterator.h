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
/* DATE    : 5 October 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* An abstract iterator for data containers.                                        */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_ITERATOR_H__
#define _SPCT_ITERATOR_H__


/**
 * @file iterator.h
 * Abstract iterator for data containers.
 */


/**
 * @ingroup SContainers
 * @defgroup SIterator Iterator
 * An abstract iterator for data containers. Any child class of #SContainer can
 * also derived an iterator from #SIterator.
 * @example list_iteration_example.c
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/utils/types.h"
#include "base/objsystem/objsystem.h"
#include "base/errdbg/errdbg.h"
#include "containers/container.h"


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
 * @hideinitializer
 * Return the given #SIterator child class object as an iterator
 * object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SIterator* type.
 *
 * @note This casting is not safety checked.
 */
#define S_ITERATOR(SELF)    ((SIterator *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SIterator,
 * see full description #S_ITERATOR_CALL for usage.
 *
 * @param SELF The given #SIterator*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_ITERATOR_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters
 * passed to the object function @c func.
 */
#define S_ITERATOR_CALL(SELF, FUNC)			\
	((SIteratorClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SIterator
 * can be called.
 *
 * @param SELF The given #SIterator*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_ITERATOR_METH_VALID(SELF, FUNC)			\
	S_ITERATOR_CALL(SELF, FUNC) ? TRUE : FALSE


/**
 * @}
 */


/************************************************************************************/
/*                                                                                  */
/* SIterator definition                                                             */
/*                                                                                  */
/************************************************************************************/

/**
 * @ingroup SIterator
 * The SIterator structure.
 * An iterator for container data types.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject     obj;

	/**
	 * @protected Container object of iterator instance.
	 */
	SContainer *myContainer;
} SIterator;


/************************************************************************************/
/*                                                                                  */
/* SIteratorClass definition                                                        */
/*                                                                                  */
/************************************************************************************/

/**
 * The SIteratorClass structure.
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
	 * @protected First iterator function pointer.
	 * Return an iterator to the first object of the container
	 * associated with the given iterator.
	 *
	 * @param self The given iterator.
	 * @param error Error code.
	 *
	 * @return Iterator to the first object of the container. If there
	 * are no objects in the container, then @c NULL must be
	 * returned so that #SIteratorFirst can delete the iterator.
	 *
	 * @note If an error occurs then @c error must be set so that
	 * #SIteratorFirst can catch the error and delete the
	 * iterator.
	 */
	SIterator  *(*first)    (SIterator *self, s_erc *error);

	/**
	 * @protected Last iterator function pointer.
	 * Return an iterator to the last object of the container
	 * associated with the given iterator.
	 *
	 * @param self The given iterator.
	 * @param error Error code.
	 *
	 * @return Iterator to the last object of the container. If there
	 * are no objects in the container, then @c NULL must be
	 * returned so that #SIteratorLast can delete the iterator.
	 *
	 * @note If an error occurs then @c error must be set so that
	 * #SIteratorLast can catch the error and delete the
	 * iterator.
	 */
	SIterator  *(*last)     (SIterator *self, s_erc *error);

	/**
	 * @protected Next iterator function pointer.
	 * Return an iterator to the next object of the container
	 * associated with the given iterator.
	 *
	 * @param self The given iterator.
	 * @param error Error code.
	 *
	 * @return Iterator to the next object of the container. If there
	 * are no more objects in the container, then @c NULL must be
	 * returned so that #SIteratorNext can delete the iterator.
	 *
	 * @note If an error occurs then @c error must be set so that
	 * #SIteratorNext can catch the error and delete the
	 * iterator.
	 */
	SIterator  *(*next)     (SIterator *self, s_erc *error);

	/**
	 * @protected Prev iterator function pointer.
	 * Return an iterator to the previous object of the container
	 * associated with the given iterator.
	 *
	 * @param self The given iterator.
	 * @param error Error code.
	 * @return Iterator to the previous object of the container. If
	 * there are no more objects in the container, then @c NULL must
	 * be returned so that #SIteratorPrev can delete the iterator.
	 *
	 * @note If an error occurs then @c error must be set so that
	 * #SIteratorPrev can catch the error and delete the
	 * iterator.
	 */
	SIterator  *(*prev)     (SIterator *self, s_erc *error);
} SIteratorClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/


/**
 * @name Forward Iteration
 * @{
 */


/**
 * Return an iterator to the first object of the container associated
 * with the given iterator.
 * @public @memberof SIterator
 *
 * @param self The given iterator.
 *
 * @return Iterator to the first object of the container. If there are
 * no objects in the container, the iterator will be deleted and
 * @c NULL will be returned.
 *
 * @note If an error occurred, then the iterator @c self will be
 * deleted, the error will be logged and @c NULL will be returned.
 */
S_API SIterator *SIteratorFirst(SIterator *self);


/**
 * Return an iterator to the next object of the container associated
 * with the given iterator.
 * @public @memberof SIterator
 *
 * @param self The given iterator.
 *
 * @return Iterator to the next object of the container. If there are
 * no more objects in the container, the iterator will be deleted and
 * @c NULL will be returned.
 *
 * @note If an error occurred, then the iterator @c self will be
 * deleted, the error will be logged and @c NULL will be returned.
 */
S_API SIterator *SIteratorNext(SIterator *self);


/**
 * @}
 */


/**
 * @name Reverse Iteration
 * @{
 */


/**
 * Return an iterator to the last object of the container associated
 * with the given iterator.
 * @public @memberof SIterator
 *
 * @param self The given iterator.
 *
 * @return Iterator to the last object of the container. If there are
 * no objects in the container, the iterator will be deleted and
 * @c NULL will be returned.
 *
 * @note If an error occurred, then the iterator @c self will be
 * deleted, the error will be logged and @c NULL will be returned.
 */
S_API SIterator *SIteratorLast(SIterator *self);


/**
 * Return an iterator to the previous object of the container
 * associated with the given iterator.
 * @public @memberof SIterator
 *
 * @param self The given iterator.
 *
 * @return Iterator to the previous object of the container. If there
 * are no more objects in the container, the iterator will be deleted
 * and @c NULL will be returned.
 *
 * @note If an error occurred, then the iterator @c self will be
 * deleted, the error will be logged and @c NULL will be returned.
 */
S_API SIterator *SIteratorPrev(SIterator *self);


/**
 * @}
 */


/**
 * Add the SIterator class to the object system.
 * @private @memberof SIterator
 * @param error Error code.
 */
S_LOCAL void _s_iterator_class_add(s_erc *error);


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

#endif /* _SPCT_ITERATOR_H__ */
