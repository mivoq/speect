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
/* DATE    : 24 October 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A class for containing the items (arbitrary objects). Items are nodes in a       */
/* relation which is again linked to an utterance. Items contain a key-value        */
/* (string/object) set of features. An Item can have a list of daughter items as    */
/* well. Items can be shared between relations.                                     */
/*                                                                                  */
/* Loosely based on cst_item of Flite, http://cmuflite.org (1.3),                   */
/* EST_Item of Edinburgh Speech Tools,                                              */
/* http://www.cstr.ed.ac.uk/projects/speech_tools (1.2.96).                         */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned projects.                                                              */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_ITEM_H__
#define _SPCT_ITEM_H__


/************************************************************************************/
/*  Flite license, cst_item                                                         */
/*                                                                                  */
/*                       Language Technologies Institute                            */
/*                          Carnegie Mellon University                              */
/*                             Copyright (c) 1999                                   */
/*                             All Rights Reserved.                                 */
/*                                                                                  */
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
/*                                                                                  */
/*       CARNEGIE MELLON UNIVERSITY AND THE CONTRIBUTORS TO THIS WORK               */
/*       DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING            */
/*       ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT         */
/*       SHALL CARNEGIE MELLON UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE            */
/*       FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES          */
/*       WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN         */
/*       AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,                */
/*       ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF             */
/*       THIS SOFTWARE.                                                             */
/*                                                                                  */
/*       Author:  Alan W Black                                                      */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/* Edinburgh Speech Tools license,  EST_Item                                        */
/*                                                                                  */
/*                     Centre for Speech Technology Research                        */
/*                          University of Edinburgh, UK                             */
/*                              Copyright (c) 1998                                  */
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
/*       Author :  Alan W Black                                                     */
/*                                                                                  */
/************************************************************************************/


/**
 * @file item.h
 * A class for containing the items.
 */


/**
 * @ingroup SHrg
 * @defgroup SItem Item
 * A class for containing the items (arbitrary objects). Items are nodes in a
 * relation which is again linked to an utterance. Items contain a key-value
 * (string/object) set of features. An Item can have a list of daughter items as
 * well. Items can be shared between relations.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/errdbg/errdbg.h"
#include "hrg/item_content.h"


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
 * Return the given #SItem child class object as a #SItem object.
 * @param SELF The given object.
 * @return Given object as #SItem* type.
 * @note This casting is not safety checked.
 */
#define S_ITEM(SELF)    ((SItem *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SItem,
 * see full description #S_ITEM_CALL for usage.
 * @param SELF The given #SItem*.
 * @param FUNC The function method of the given object to call.
 * @note This casting is not safety checked.
 * @note Example usage: @code S_ITEM_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_ITEM_CALL(SELF, FUNC)					\
	((SItemClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SItem
 * can be called.
 * @param SELF The given #SItem*.
 * @param FUNC The function method of the given object to check.
 * @return #TRUE if function can be called, otherwise #FALSE.
 * @note This casting is not safety checked.
 */
#define S_ITEM_METH_VALID(SELF, FUNC)			\
	S_ITEM_CALL(SELF, FUNC) ? TRUE : FALSE


/**
 * @}
 */


/************************************************************************************/
/*                                                                                  */
/* SItem definition                                                                 */
/*                                                                                  */
/************************************************************************************/

/**
 * The SItem structure.
 * Items are nodes in a relation which is again linked to an utterance.
 * Items contain a key-value (string/object) set of features. An Item can have
 * a list of daughter items as well. Items can be shared between relations.
 * @extends SObject
 */
struct SItem
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject          obj;

	/**
	 * @protected Relation that this item is a node in.
	 */
	SRelation       *relation;

	/**
	 * @protected Shared content of this item.
	 */
	SItmContent     *content;

	/**
	 * @protected The parent of this item.
	 */
	SItem           *up;

	/**
	 * @protected The daughters of this item.
	 */
	SItem           *down;

	/**
	 * @protected Next item in this item's relation.
	 */
	SItem           *next;

	/**
	 * @protected Previous item in this item's relation.
	 */
	SItem           *prev;
};


/************************************************************************************/
/*                                                                                  */
/* SItemClass definition                                                            */
/*                                                                                  */
/************************************************************************************/

/**
 * The SItemClass structure.
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
	 * @protected ItemAs function pointer
	 * Find the item in the given relation that has the same shared
	 * content as the given item.
	 * @param self The item.
	 * @param relation The relation name.
	 * @param error Error code.
	 * @return The given item as it is represented in the named relation.
	 */
	SItem            *(*item_as)       (const SItem *self, const char *relation,
										s_erc *error);

	/**
	 * @protected ItemIn function pointer
	 * Query if the given item is in the named relation.
	 * @param self The item.
	 * @param relation The relation.
	 * @param error Error code.
	 * @return #TRUE if item is in relation else #FALSE.
	 */
	s_bool            (*item_in)       (const SItem *self, const char *relation,
										s_erc *error);

	/**
	 * @protected ItemNext function pointer
	 * Get the item next to the given one in the current relation.
	 * @param self The item.
	 * @param error Error code.
	 * @return Pointer to the next item.
	 */
	SItem            *(*item_next)     (const SItem *self, s_erc *error);

	/**
	 * @protected ItemPrev function pointer
	 * Get the item previous to the given one in the current relation.
	 * @param self The item.
	 * @param error Error code.
	 * @return Pointer to the prev item.
	 */
	SItem            *(*item_prev)     (const SItem *self, s_erc *error);

	/**
	 * @protected ItemAppend function pointer
	 * Create a new item and append it after the given item @c
	 * self. The new item will share it's content with the given item
	 * @c toShare.
	 * @param self The given item.
	 * @param toShare The item with which the newly created item will
	 * share it's content. If @c NULL then a new content will be
	 * created for the appended item.
	 * @param error Error code.
	 * @return Pointer to appended item.
	 */
	SItem            *(*item_append)   (SItem *self, const SItem *toShare,
										s_erc *error);

	/**
	 * @protected ItemPrepend function pointer
	 * Create a new item and prepend it before the given item @c
	 * self. The new item will share it's content with the given item
	 * @c toShare.
	 * @param self The given item.
	 * @param toShare The item with which the newly created item will
	 * share it's content. If @c NULL then a new content will be
	 * created for the prepended item.
	 * @param error Error code.
	 * @return Pointer to prepended item.
	 */
	SItem            *(*item_prepend)  (SItem *self, const SItem *toShare,
										s_erc *error);

	/**
	 * @protected ItemParent function pointer
	 * Returns the parent item of the given item.
	 * @param self The given item.
	 * @param error Error code.
	 * @return Pointer to the parent item of the given item.
	 */
	SItem            *(*item_parent)   (const SItem *self, s_erc *error);

	/**
	 * @protected ItemDaughter function pointer
	 * Retrieve the first daughter item of the given item.
	 * @param self The given item.
	 * @param error Error code.
	 * @return Pointer to the first daughter item of the given item.
	 */
	SItem            *(*item_daughter) (const SItem *self, s_erc *error);

	/**
	 * @protected ItemNth function pointer
	 * Retrieve the @c nth daughter item of the given item.
	 * @param self The given item.
	 * @param n The index of the daughter item to return.
	 * @note Indexing starts at 0.
	 * @param error Error code.
	 * @return Pointer to the @c nth daughter item of the given item.
	 */
	SItem            *(*nth_daughter)  (const SItem *self, uint32 n, s_erc *error);

	/**
	 * @protected LastDaughter function pointer
	 * Retrieve the last daughter item of the given item.
	 * @param self The given item.
	 * @param error Error code.
	 * @return Pointer to the last daughter item of the given item.
	 */
	SItem            *(*last_daughter) (const SItem *self, s_erc *error);

	/**
	 * @protected AddDaughter function pointer
	 * Create a new item and add it as a daughter of the given item @c
	 * self. The new item will share it's content with the given item
	 * @c toShare.
	 * @param self The given item.
	 * @param toShare The item with which the newly created item will
	 * share it's content. If @c NULL then a new content will be
	 * created for the daughter item.
	 * @param error Error code.
	 * @return Pointer to newly added daughter item.
	 */
	SItem            *(*add_daughter)  (SItem *self, const SItem *toShare,
										s_erc *error);

	/**
	 * @protected GetName function pointer
	 * Get the given item's name.
	 * @param self The given item.
	 * @param error Error code.
	 * @return Pointer to the given item's name.
	 */
	const char       *(*get_name)      (const SItem *self, s_erc *error);

	/**
	 * @protected SetName function pointer
	 * Set the given item's name.
	 * @param self The given item.
	 * @param name The name of the item.
	 * @param error Error code.
	 */
	void              (*set_name)      (SItem *self, const char *name,
										s_erc *error);

	/**
	 * @protected FeatPresent function pointer
	 * Query if named feature is present in the given item.
	 * @param self The given item.
	 * @param name The feature name.
	 * @param error Error code.
	 * @return #TRUE if present or #FALSE if not.
	 */
	s_bool            (*feat_present)  (const SItem *self, const char *name,
										s_erc *error);

	/**
	 * @protected FeatDelete function pointer
	 * Delete the named feature from the given item.
	 * @param self The given item.
	 * @param name The feature name.
	 * @param error Error code.
	 */
	void              (*feat_delete)   (SItem *self, const char *name,
										s_erc *error);

	/**
	 * @protected FeatKeys function pointer
	 * Get the keys of the features of the given item.
	 *
	 * @param self The given item.
	 * @param error Error code.
	 *
	 * @return Pointer to a #SList containing the keys of the features of
	 * the item. The list objects (keys) are string objects and are
	 * accessed with #SObjectGetString.
	 *
	 * @note Caller is responsible for the memory of the returned #SList
	 * object.
	 */
	SList            *(*feat_keys)     (const SItem *self, s_erc *error);

	/**
	 * @protected GetObject function pointer
	 * Get the #SObject object of the named feature of the given item.
	 * @param self The given item.
	 * @param name The feature name.
	 * @param error Error code.
	 * @return Pointer to the #SObject object.
	 */
	const SObject    *(*get_object)    (const SItem *self, const char *name,
										s_erc *error);

	/**
	 * @protected GetInt function pointer
	 * Get the integer value of the named feature of the given item.
	 * @param self The given item.
	 * @param name The feature name.
	 * @param error Error code.
	 * @return Integer value of named feature.
	 */
	sint32           (*get_int)        (const SItem *self, const char *name,
										s_erc *error);

	/**
	 * @protected GetFloat function pointer
	 * Get the float value of the named feature of the given item.
	 * @param self The given item.
	 * @param name The feature name.
	 * @param error Error code.
	 * @return Float value of named feature.
	 */
	float            (*get_float)      (const SItem *self, const char *name,
										s_erc *error);


	/**
	 * @protected GetString function pointer
	 * Get the string value of the named feature of the given item.
	 * @param self The given item.
	 * @param name The feature name.
	 * @param error Error code.
	 * @return Pointer to the string.
	 */
	const char      *(*get_string)     (const SItem *self, const char *name,
										s_erc *error);

	/**
	 * @protected SetObject function pointer
	 * Set the named feature to the given #SObject object in the given item.
	 * @param self The given item.
	 * @param name The feature name.
	 * @param obejct The #SObject.
	 * @param error Error code.
	 */
	void             (*set_object)     (SItem *self, const char *name,
										const SObject *object, s_erc *error);

	/**
	 * @protected SetInt function pointer
	 * Set the named feature to the given integer value.
	 * @param self The given item.
	 * @param name The feature name.
	 * @param i The integer value.
	 * @param error Error code.
	 */
	void             (*set_int)        (SItem *self, const char *name,
										sint32 i, s_erc *error);

	/**
	 * @protected SetFloat function pointer
	 * Set the named feature to the given float value.
	 * @param self The given item.
	 * @param name The feature name.
	 * @param f The float value.
	 * @param error Error code.
	 */
	void             (*set_float)      (SItem *self, const char *name,
										float f, s_erc *error);


	/**
	 * @protected SetString function pointer
	 * Set the named feature to the given string value.
	 * @param self The given item.
	 * @param name The feature name
	 * @param string The string value.
	 * @param error Error code.
	 */
	void             (*set_string)     (SItem *self, const char *name,
										const char *string, s_erc *error);

	/**
	 * @protected Relation function pointer
	 * Get the relation of the given item.
	 * @param self The given item.
	 * @param error Error code.
	 * @return Pointer to the given item's relation.
	 */
	const SRelation  *(*relation)      (const SItem *self, s_erc *error);

	/**
	 * @protected Utterance function pointer
	 * Get the utterance of the given item.
	 * @param self The given item.
	 * @param error Error code.
	 * @return Pointer to the given item's utterance.
	 */
	const SUtterance *(*utterance)     (const SItem *self, s_erc *error);

	/**
	 * @protected Voice function pointer
	 * Get the voice of the given item.
	 * @param self The given item.
	 * @param error Error code.
	 * @return Pointer to the given item's voice.
	 */
	const SVoice     *(*voice)         (const SItem *self, s_erc *error);
} SItemClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * @name Initialization
 * @{
 */


/**
 * Initialize a newly created item. Set it's relation and the content it must
 * share with another item. If the given shared item is @c NULL, a new content
 * is created.
 * @public @memberof SItem
 *
 * @param self The newly created item to initialize.
 * @param rel The relation of the new item.
 * @param toShare Item with which to share content. If @c NULL then a new content is
 * created.
 * @param error Error code.
 *
 * @note If this function fails the item will be deleted and the @c
 * self pointer will be set to @c NULL.
 */
S_API void SItemInit(SItem **self, const SRelation *rel, const SItem *toShare,
					 s_erc *error);


/**
 * Initialize a newly created item. Set it's relation and the content it must
 * share with another item. If the given shared item is @c NULL, a new content
 * is created. This is almost the same as #SItemInit except that no mutex locking
 * is done. If we don't do this then a relation cannot create items.
 * This *must* only be called in SRelation and SItem functions
 * @private @memberof SItem
 *
 * @param self The newly created item to initialize.
 * @param rel The relation of the new item.
 * @param toShare Item with which to share content. If @c NULL then a new content is
 * created.
 * @param error Error code.
 *
 * @note If this function fails the item will be deleted and the @c
 * self pointer will be set to @c NULL.
 */
S_LOCAL void _SItemInit_no_lock(SItem **self, const SRelation *rel,
								const SItem *toShare, s_erc *error);


/**
 * @}
 */


/**
 * @name As/In Relation
 * @{
 */


/**
 * Find the item in the given relation that has the same shared
 * content as the given item.
 * @public @memberof SItem
 * @param self The given item.
 * @param relation The relation name.
 * @param error Error code.
 * @return The given item as it is in the named relation.
 */
S_API SItem *SItemAs(const SItem *self, const char *relation, s_erc *error);


/**
 * Query if the given item is in the named relation.
 * @public @memberof SItem
 * @param self The given item.
 * @param relation The relation.
 * @param error Error code.
 * @return #TRUE if item is in relation else #FALSE.
 */
S_API s_bool SItemIn(const SItem *self, const char *relation, s_erc *error);


/**
 * @}
 */


/**
 * @name List Traversal
 * @{
 */


/**
 * Get the item next to the given one in the current relation.
 * @public @memberof SItem
 * @param self The given item.
 * @param error Error code.
 * @return Pointer to the next item.
 */
S_API SItem *SItemNext(const SItem *self, s_erc *error);


/**
 * Get the item previous to the given one in the current relation.
 * @public @memberof SItem
 * @param self The given item.
 * @param error Error code.
 * @return Pointer to the prev item.
 */
S_API SItem *SItemPrev(const SItem *self, s_erc *error);


/**
 * Create a new item and append it after the given item @c
 * self. The new item will share it's content with the given item
 * @c toShare.
 * @public @memberof SItem
 *
 * @param self The given item.
 * @param toShare The item with which the newly created item will
 * share it's content. If @c NULL then a new content will be
 * created for the appended item.
 * @param error Error code.
 *
 * @return Pointer to appended item.
 */
S_API SItem *SItemAppend(SItem *self, const SItem *toShare, s_erc *error);


/**
 * Create a new item and prepend it before the given item @c
 * self. The new item will share it's content with the given item
 * @c toShare.
 * @public @memberof SItem
 *
 * @param self The given item.
 * @param toShare The item with which the newly created item will
 * share it's content. If @c NULL then a new content will be
 * created for the prepended item.
 * @param error Error code.
 *
 * @return Pointer to prepended item.
 */
S_API SItem *SItemPrepend(SItem *self, const SItem *toShare, s_erc *error);


/**
 * @}
 */


/**
 * @name Tree Traversal
 * @{
 */


/**
 * Returns the parent item of the given item.
 * @public @memberof SItem
 * @param self The given item.
 * @param error Error code.
 * @return Pointer to the parent item of the given item.
 */
S_API SItem *SItemParent(const SItem *self, s_erc *error);


/**
 * Retrieve the first daughter item of the given item.
 * @public @memberof SItem
 * @param self The given item.
 * @param error Error code.
 * @return Pointer to the first daughter item of the given item.
 */
S_API SItem *SItemDaughter(const SItem *self, s_erc *error);


/**
 * Retrieve the @c nth daughter item of the given item.
 * @public @memberof SItem
 * @param self The given item.
 * @param n The index of the daughter to return.
 * @note Indexing starts at 0.
 * @param error Error code.
 * @return Pointer to the @c nth daughter item of the given item.
 */
S_API SItem *SItemNthDaughter(const SItem *self, uint32 n, s_erc *error);


/**
 * Retrieve the last daughter item of the given item.
 * @public @memberof SItem
 * @param self The given item.
 * @param error Error code.
 * @return Pointer to the last daughter item of the given item.
 */
S_API SItem *SItemLastDaughter(const SItem *self, s_erc *error);


/**
 * Create a new item and add it as a daughter of the given item @c
 * self. The new item will share it's content with the given item
 * @c toShare.
 * @public @memberof SItem
 *
 * @param self The given item.
 * @param toShare The item with which the newly created item will
 * share it's content. If @c NULL then a new content will be
 * created for the daughter item.
 * @param error Error code.
 *
 * @return Pointer to newly added daughter item.
 */
S_API SItem *SItemAddDaughter(SItem *self, const SItem *toShare, s_erc *error);


/**
 * @}
 */


/**
 * @name Feature Functions
 * @{
 */


/**
 * Get the given item's name.
 * @public @memberof SItem
 * @param self The given item.
 * @param error Error code.
 * @return Pointer to the given item's name.
 */
S_API const char *SItemGetName(const SItem *self, s_erc *error);


/**
 * Set the given item's name.
 * @public @memberof SItem
 * @param self The given item.
 * @param name The name of the item.
 * @param error Error code.
 */
S_API void SItemSetName(SItem *self, const char *name, s_erc *error);


/**
 * Query if named feature is present in the given item.
 * @public @memberof SItem
 * @param self The given item.
 * @param name The feature name.
 * @param error Error code.
 * @return #TRUE if present or #FALSE if not.
 */
S_API s_bool SItemFeatureIsPresent(const SItem *self, const char *name, s_erc *error);


/**
 * Delete the named feature from the given item.
 * @public @memberof SItem
 * @param self The given item.
 * @param name The feature name.
 * @param error Error code.
 */
S_API void SItemDelFeature(SItem *self, const char *name, s_erc *error);


/**
 * Get the keys of the features of the given item.
 * @public @memberof SItem
 *
 * @param self The given item.
 * @param error Error code.
 *
 * @return Pointer to a #SList containing the keys of the features of
 * the item. The list objects (keys) are string objects and are
 * accessed with #SObjectGetString.
 *
 * @note Caller is responsible for the memory of the returned #SList
 * object.
 */
S_API SList *SItemFeatKeys(const SItem *self, s_erc *error);


/**
 * @}
 */


/**
 * @name Get key-value feature pairs
 * @{
 */


/**
 * Get the #SObject of the named feature of the given item.
 * @public @memberof SItem
 * @param self The given item.
 * @param name The feature name.
 * @param error Error code.
 * @return Pointer to the SObject object.
 * @note If feature does not exist an @c S_ARGERROR will be raised,
 * use #SItemFeatureIsPresent to test if feature is present.
 */
S_API const SObject *SItemGetObject(const SItem *self, const char *name, s_erc *error);


/**
 * Get the integer value of the named feature of the given item.
 * @public @memberof SItem
 * @param self The given item.
 * @param name The feature name.
 * @param error Error code.
 * @return Integer value of named feature.
 * @note If feature does not exist an @c S_ARGERROR will be raised,
 * use #SItemFeatureIsPresent to test if feature is present.
 */
S_API sint32 SItemGetInt(const SItem *self, const char *name, s_erc *error);


/**
 * Get the float value of the named feature of the given item.
 * @public @memberof SItem
 * @param self The given item.
 * @param name The feature name.
 * @param error Error code.
 * @return Float value of named feature.
 * @note If feature does not exist an @c S_ARGERROR will be raised,
 * use #SItemFeatureIsPresent to test if feature is present.
 */
S_API float SItemGetFloat(const SItem *self, const char *name, s_erc *error);


/**
 * Get the string value of the named feature of the given item.
 * @public @memberof SItem
 * @param self The given item.
 * @param name The feature name.
 * @param error Error code.
 * @return Pointer to the string.
 * @note If feature does not exist an @c S_ARGERROR will be raised,
 * use #SItemFeatureIsPresent to test if feature is present.
 */
S_API const char *SItemGetString(const SItem *self, const char *name, s_erc *error);


/**
 * @}
 */


/**
 * @name Set key-value feature pairs
 * @{
 */


/**
 * Set the named feature to the given #SObject object of the given item.
 * @public @memberof SItem
 * @param self The given item.
 * @param name The feature name.
 * @param object The SObject object.
 * @param error Error code.
 */
S_API void SItemSetObject(SItem *self, const char *name,
						  const SObject *object, s_erc *error);


/**
 * Set the named feature to the given integer value.
 * @public @memberof SItem
 * @param self The given item.
 * @param name The feature name.
 * @param i The integer value.
 * @param error Error code.
 */
S_API void SItemSetInt(SItem *self, const char *name, sint32 i, s_erc *error);


/**
 * Set the named feature to the given float value.
 * @public @memberof SItem
 * @param self The given item.
 * @param name The feature name.
 * @param f The float value.
 * @param error Error code.
 */
S_API void SItemSetFloat(SItem *self, const char *name, float f, s_erc *error);


/**
 * Set the named feature to the given string value.
 * @public @memberof SItem
 * @param self The given item.
 * @param name The feature name
 * @param string The string value.
 * @param error Error code.
 */
S_API void SItemSetString(SItem *self, const char *name, const char *string, s_erc *error);


/**
 * @}
 */


/**
 * @name Miscellaneous
 * @{
 */


/**
 * Get the relation of the given item.
 * @public @memberof SItem
 * @param self The given item.
 * @param error Error code.
 * @return Pointer to the given item's relation.
 */
S_API const SRelation *SItemRelation(const SItem *self, s_erc *error);


/**
 * Get the utterance of the given item.
 * @public @memberof SItem
 * @param self The given item.
 * @param error Error code.
 * @return Pointer to the given item's utterance.
 */
S_API const SUtterance *SItemUtterance(const SItem *self, s_erc *error);


/**
 * Get the voice of the given item.
 * @public @memberof SItem
 * @param self The given item.
 * @param error Error code.
 * @return Pointer to the given item's voice.
 */
S_API const SVoice *SItemVoice(const SItem *self, s_erc *error);


/**
 * Query if the two given items are equal. Equal meaning if they
 * share the same contents.
 * @public @memberof SItem
 * @param a The item to test.
 * @param b The item to test against.
 * @param error Error code.
 * @return #TRUE if they are equal, otherwise #FALSE.
 */
S_API s_bool SItemEqual(const SItem *a, const SItem *b, s_erc *error);


/**
 * @}
 */


/**
 * Add the SItem class to the object system.
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_item_class_add(s_erc *error);


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

#endif /* _SPCT_ITEM_H__ */
