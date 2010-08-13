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
/* DATE    : 12 October 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A list class, an abstract data structure that implements an ordered collection   */
/* of values, where the same value may occur more than once.                        */
/*                                                                                  */
/* Loosely based on EST_TList of Edinburgh Speech Tools,                            */
/* http://www.cstr.ed.ac.uk/projects/speech_tools (1.2.96)                          */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_LIST_H__
#define _SPCT_LIST_H__


/************************************************************************************/
/* Edinburgh Speech Tools license,  EST_TList                                       */
/*                                                                                  */
/*                     Centre for Speech Technology Research                        */
/*                          University of Edinburgh, UK                             */
/*                           Copyright (c) 1995,1996                                */
/*                             All Rights Reserved.                                 */
/*       Permission is hereby granted, free of charge, to use and distribute        */
/*       this software and its documentation without restriction, including         */
/*       without limitation the rights to use, copy, modify, merge, publish,        */
/*       distribute, sublicense, and/or sell copies of this work, and to            */
/*       permit persons to whom this work is furnished to do so, subject to         */
/*       the following conditions:                                                  */
/*        1. The code must retain the above copyright notice, this list of          */
/*           conditions and the following disclaimer.                               */
/*        2. Any modifications must be clearly marked as such.                      */
/*        3. Original authors' names are not deleted.                               */
/*        4. The authors' names are not used to endorse or promote products         */
/*           derived from this software without specific prior written              */
/*           permission.                                                            */
/*       THE UNIVERSITY OF EDINBURGH AND THE CONTRIBUTORS TO THIS WORK              */
/*       DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING            */
/*       ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT         */
/*       SHALL THE UNIVERSITY OF EDINBURGH NOR THE CONTRIBUTORS BE LIABLE           */
/*       FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES          */
/*       WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN         */
/*       AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,                */
/*       ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF             */
/*       THIS SOFTWARE.                                                             */
/*                                                                                  */
/*       Author :  Paul Taylor                                                      */
/*                                                                                  */
/************************************************************************************/


/**
 * @file src/containers/list/list.h
 * Abstract list data container.
 */


/**
 * @ingroup SContainer
 * @defgroup SList List
 * A list class, an abstract data structure that implements an ordered
 * collection of values (#SObject), where the same value may occur
 * more than once.
 *
 * The iterator (@ref SIterator) implementation of #SList objects
 * returns the elements (as #SObject) of the list for the
 * #SIteratorObject, and #SIteratorUnlink calls. The #SIteratorKey
 * method is not implemented and will set an error and return @c NULL
 * if called for SList type iterators.
 *
 * @{
 */


/**
 * @example list_iteration_example.c
 * An example of adding objects to an #SList and also iterating over
 * the list.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/errdbg/errdbg.h"
#include "containers/container.h"
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
 * @hideinitializer
 * Return the given #SList child class object as a list object.
 * @param SELF The given object.
 * @return Given object as #SList* type.
 * @note This casting is not safety checked.
 */
#define S_LIST(SELF)    ((SList *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SList,
 * see full description #S_LIST_CALL for usage.
 * @param SELF The given #SList*.
 * @param FUNC The function method of the given object to call.
 * @note This casting is not safety checked.
 * @note Example usage: @code S_LIST_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_LIST_CALL(SELF, FUNC)					\
	((SListClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SList
 * can be called.
 * @param SELF The given #SList*.
 * @param FUNC The function method of the given object to check.
 * @return #TRUE if function can be called, otherwise #FALSE.
 * @note This casting is not safety checked.
 */
#define S_LIST_METH_VALID(SELF, FUNC)			\
	S_LIST_CALL(SELF, FUNC) ? TRUE : FALSE


/**
 * @}
 */


/************************************************************************************/
/*                                                                                  */
/* SList definition                                                                 */
/*                                                                                  */
/************************************************************************************/

/**
 * The SList structure.
 * @extends SContainer
 */
typedef struct
{
	/**
	 * @protected Inherit from #SContainer.
	 */
	SContainer  obj;
} SList;


/************************************************************************************/
/*                                                                                  */
/* SListClass definition                                                            */
/*                                                                                  */
/************************************************************************************/

/**
 * The SListClass structure. It inherits from #SContainer so that it supports
 * different list implementations.
 * @extends SContainerClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SContainerClass.
	 */
	SContainerClass  _inherit;

	/* Class methods */
	/**
	 * @protected IsEmpty function pointer.
	 * Test to see if the given list is empty.
	 *
	 * @param self The list container.
	 * @param error Error code.
	 *
	 * @return #TRUE if empty else, #FALSE.
	 */
	s_bool         (*is_empty)     (const SList *self, s_erc *error);

	/**
	 * @protected Size function pointer.
	 * Return the number of #SObject in the list.
	 *
	 * @param self The list container.
	 * @param error Error code.
	 *
	 * @return The number of #SObject objects in the list.
	 */
	size_t         (*size)         (const SList *self, s_erc *error);

	/**
	 * @protected Append function pointer.
	 * Append an #SObject to end of list.
	 *
	 * @param self The list container.
	 * @param object #SObject to append to end of list.
	 * @param error Error code.
	 */
	void           (*append)       (SList *self, const SObject *object, s_erc *error);

	/**
	 * @protected Prepend function pointer.
	 * Prepend #SObject to beginning of list.
	 *
	 * @param self The list container.
	 * @param object #SObject to prepend to beginning of list.
	 * @param error Error code.
	 */
	void           (*prepend)      (SList *self, const SObject *object, s_erc *error);

	/**
	 * @protected InsertBefore function pointer.
	 * Insert an #SObject before the one currently pointed to by
	 * the iterator.
	 *
	 * @param self The list container.
	 * @param itr Iterator to current list object.
	 * @param object #SObject to insert.
	 * @param error Error code.
	 */
	void           (*insert_before)(SList *self, SIterator *itr, const SObject *object,
									s_erc *error);

	/**
	 * @protected InsertAfter function pointer.
	 * Insert an #SObject after the one currently pointed to by
	 * the iterator.
	 *
	 * @param self The list container.
	 * @param itr Iterator to current list object.
	 * @param object #SObject to insert.
	 * @param error Error code.
	 */
	void           (*insert_after) (SList *self, SIterator *itr, const SObject *object,
									s_erc *error);

	/**
	 * @protected Merge function pointer.
	 * Merge two lists. Append all the #SObject of one list onto another
	 * list. For example:
	 * @verbatim
	 self = (12, "a", 0.9644)
	 with = (5.31, "a")

	 SListMerge(self, with, error);

	 self = (12, "a", 0.9644, 5.31, "a")
	 with = (5.31, "a")
	 @endverbatim
	 *
	 * @param self The list container that @c with is merged with.
	 * @param with The list to merge with @c self.
	 * @param error Error code.
	 */
	void           (*merge)        (SList *self, const SList *with, s_erc *error);

	/**
	 * @protected Copy (shallow) function pointer.
	 * Copy the list objects from @a src to @a dst. If @a dst
	 * does not exist a new one will be created.
	 *
	 * @param dst Pointer to destination list. If @c NULL then a
	 * new list will be created.
	 * @param src Pointer to source list.
	 * @param error Error code.
	 *
	 * @return Pointer to destination list.
	 */
	SList         *(*copy)         (SList *dst, const SList *src, s_erc *error);

	/**
	 * @protected Push function pointer.
	 * Push #SObject into end of list.
	 *
	 * @param self The list container.
	 * @param object #SObject to push into end of list.
	 * @param error Error code.
	 */
	void           (*push)         (SList *self, const SObject *object, s_erc *error);

	/**
	 * @protected Pop function pointer.
	 * Pop #SObject from end of SList.
	 *
	 * @param self The list container.
	 * @param error Error code.
	 *
	 * @return #SObject popped from end of list.
	 *
	 * @note The #SObject is unlinked from the list and it's memory
	 * is the responsibility of the caller.
	 */
	SObject       *(*pop)          (SList *self, s_erc *error);

	/**
	 * @protected Reverse function pointer.
	 * Reverse the order of the elements in the list in place.
	 *
	 * @param self The list container to reverse.
	 * @param error Error code.
	 */
	void           (*reverse)      (SList *self, s_erc *error);

	/**
	 * @protected Nth function pointer.
	 * Return a pointer to the nth #SObject in the list.
	 *
	 * @param self The list container.
	 * @param n Index of the #SObject.
	 * @param error Error code.
	 *
	 * @return #SObject at index @c n.
	 *
	 * @note Indexing starts at 0.
	 */
	const SObject *(*nth)          (const SList *self, uint32 n, s_erc *error);

	/**
	 * @protected ValPresent function pointer.
	 * Test if the given #SObject in the list.
	 *
	 * @param self The list container.
	 * @param object The #SObject to test for.
	 * @param error Error code.
	 *
	 * @return #TRUE or #FALSE.
	 *
	 * @note The #SObjectClass::compare function must be implemented for the given
	 * object type (class).
	 */
	s_bool         (*val_present)  (const SList *self, const SObject *object,
									s_erc *error);
} SListClass;


/************************************************************************************/
/*                                                                                  */
/*  Function prototypes                                                             */
/*                                                                                  */
/************************************************************************************/

/**
 * @name Query
 * @{
 */


/**
 * Test to see if the given list is empty.
 * @public @memberof SList
 *
 * @param self The list container.
 * @param error Error code.
 *
 * @return #TRUE if empty else, #FALSE.
 */
S_API s_bool SListIsEmpty(const SList *self, s_erc *error);


/**
 * Return the number of #SObject in the list.
 * @public @memberof SList
 *
 * @param self The list container.
 * @param error Error code.
 *
 * @return The number of #SObject objects in the list.
 */
S_API size_t SListSize(const SList *self, s_erc *error);


/**
 * Test if the given #SObject in the list.
 * @public @memberof SList
 *
 * @param self The list container.
 * @param object The #SObject to test for.
 * @param error Error code.
 *
 * @return #TRUE or #FALSE.
 *
 * @note The #SObjectClass::compare function must be implemented for
 * the given object type.
 */
S_API s_bool SListValPresent(const SList *self, const SObject *object, s_erc *error);


/**
 * @}
 */


/**
 * @name Insertion
 * @{
 */


/**
 * Append an #SObject to end of list.
 * @public @memberof SList
 *
 * @param self The list container.
 * @param object #SObject to append to end of list.
 * @param error Error code.
 */
S_API void SListAppend(SList *self, const SObject *object, s_erc *error);


/**
 * Prepend #SObject to beginning of list.
 * @public @memberof SList
 *
 * @param self The list container.
 * @param object #SObject to prepend to beginning of list.
 * @param error Error code.
 */
S_API void SListPrepend(SList *self, const SObject *object, s_erc *error);


/**
 * Insert an #SObject before the one currently pointed to by
 * the iterator.
 * @public @memberof SList
 *
 * @param self The list container.
 * @param itr Iterator to current list object.
 * @param object #SObject to insert.
 * @param error Error code.
 */
S_API void SListInsertBefore(SList *self, SIterator *itr, const SObject *object,
							 s_erc *error);


/**
 * Insert #SObject after the one currently pointed to by
 * the iterator.
 * @public @memberof SList
 *
 * @param self The list container.
 * @param itr Iterator to current list object.
 * @param object #SObject to insert.
 * @param error Error code.
 */
S_API void SListInsertAfter(SList *self, SIterator *itr, const SObject *object,
							s_erc *error);


/**
 * @}
 */


/**
 * @name Merge/Copy
 * @{
 */


/**
 * Merge two lists. Append all the #SObject of one list onto another
 * list. For example:
 * @verbatim
 self = (12, "a", 0.9644)
 with = (5.31, "a")

 SListMerge(self, with, error);

 self = (12, "a", 0.9644, 5.31, "a")
 with = (5.31, "a")
 @endverbatim
 *
 * @public @memberof SList
 *
 * @param self The list container that @c with is merged with.
 * @param with The list to merge with @c self.
 * @param error Error code.
 */
S_API void SListMerge(SList *self, const SList *with, s_erc *error);


/**
 * Copy (shallow) the list objects from @a src to @a dst. If @a dst
 * does not exist a new one will be created.
 * @public @memberof SList
 *
 * @param dst Pointer to destination list. If @c NULL then a
 * new list will be created.
 * @param src Pointer to source list.
 * @param error Error code.
 *
 * @return Pointer to destination list.
 */
S_API SList *SListCopy(SList *dst, const SList *src, s_erc *error);


/**
 * @}
 */


/**
 * @name Queue
 * @{
 */


/**
 * Push #SObject into end of list.
 * @public @memberof SList
 *
 * @param self The list container.
 * @param object #SObject to push into end of list.
 * @param error Error code.
 */
S_API void SListPush(SList *self, const SObject *object, s_erc *error);


/**
 * Pop #SObject from end of SList.
 * @public @memberof SList
 *
 * @param self The list container.
 * @param error Error code.
 * @return #SObject popped from end of list.
 *
 * @note The #SObject is unlinked from the list and it's memory
 * is the responsibility of the caller.
 */
S_API SObject *SListPop(SList *self, s_erc *error);


/**
 * Reverse the order of the elements in the list in place.
 * @public @memberof SList
 *
 * @param self The list container to reverse.
 * @param error Error code.
 */
S_API void SListReverse(SList *self, s_erc *error);


/**
 * @}
 */


/**
 * @name Accessing
 * @{
 */


/**
 * Return a pointer to the nth #SObject in the list.
 * @public @memberof SList
 * @param self The list container.
 * @param n Index of the #SObject.
 * @note Indexing starts at 0.
 * @param error Error code.
 * @return #SObject object at index @a n.
 */
S_API const SObject *SListNth(const SList *self, uint32 n, s_erc *error);


/**
 * See also #SIterator.
 */

/**
 * @}
 */


/**
 * @name Remove
 * @{
 */


/**
 * See #SIterator.
 */


/**
 * @}
 */


/**
 * Add the SList class to the object system.
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_list_class_add(s_erc *error);


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

#endif /* _SPCT_LIST_H__ */
