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
 * @ingroup SContainer
 * @defgroup SIterator Iterator
 * An abstract iterator for data containers. Any child class of #SContainer should
 * also derive an iterator from #SIterator.
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
 * An abstract iterator for container data types.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject     obj;
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
	SIterator     *(*next)   (SIterator *self, s_erc *error);

	/**
	 * @protected Key iterator function pointer.
	 * Return the key that is pointed to by the iterator.
	 * This is only useful for mapping type containers and does not
	 * need to be implemented by other types of containers.
	 *
	 * @param self The given iterator.
	 * @param error Error code.
	 *
	 * @return The key pointed to by the iterator.
	 */
	const char    *(*key)    (SIterator *self, s_erc *error);

	/**
	 * @protected Object iterator function pointer.
	 * Return the object that is pointed to by the iterator.
	 *
	 * @param self The given iterator.
	 * @param error Error code.
	 *
	 * @return The object pointed to by the iterator. This object can
	 * be anything and depends on the specific container iterator
	 * implementation.
	 */
	const SObject *(*object) (SIterator *self, s_erc *error);

	/**
	 * @protected Unlink function pointer.
	 * Unlink the object from the container, and return it.
	 *
	 * @param self The given iterator.
	 * @param error Error code.
	 *
	 * @return The unlinked object pointed to by the iterator. This
	 * object can be anything and depends on the specific container
	 * iterator implementation.
	 *
	 * @note The iterator is still valid, but does not point to
	 * any @a objects in the container.
	 * @note The caller is responsible for the memory of the returned
	 * object.
	 */
	SObject       *(*unlink) (SIterator *iterator, s_erc *error);
} SIteratorClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/


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
 * Return the key that is pointed to by the iterator.
 * This is only useful for mapping type containers (e.g. @ref SMap)
 * and will return @c NULL for any other type of container.
 *
 * @param self The given iterator.
 * @param error Error code.
 *
 * @return The key pointed to by the iterator.
 */
S_API const char *SIteratorKey(SIterator *self, s_erc *error);


/**
 * Return the object that is pointed to by the iterator.
 * @public @memberof SIterator
 *
 * @param self The given iterator.
 * @param error Error code.
 *
 * @return The object pointed to by the iterator. This object can be
 * anything and depends on the specific container iterator
 * implementation.
 */
S_API const SObject *SIteratorObject(SIterator *self, s_erc *error);


/**
 * Unlink the object from the container, and return it.
 *
 * @param self The given iterator.
 * @param error Error code.
 *
 * @return The unlinked object pointed to by the iterator. This
 * object can be anything and depends on the specific container
 * iterator implementation.
 *
 * @note The iterator is still valid, but does not point to
 * any objects in the container.
 * @note The caller is responsible for the memory of the returned
 * object.
 */
S_API SObject *SIteratorUnlink(SIterator *self, s_erc *error);


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
