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
/* DATE    : 22 October 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A class for containing the shared contents of items, that is the individual      */
/* linguistic features and references to relations.                                 */
/*                                                                                  */
/* Loosely based on cst_item_contents of Flite, http://cmuflite.org (1.3),          */
/* and ItemContents of FreeTTS, http://sourceforge.net/projects/freetts (1.2)       */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned projects.                                                              */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_ITEM_CONTENTS_H__
#define _SPCT_ITEM_CONTENTS_H__


/************************************************************************************/
/*  Flite license, cst_item_contents                                                */
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
/*                                                                                  */
/* FreeTTS license, ItemContents                                                    */
/*                                                                                  */
/*      Portions Copyright 2001-2004 Sun Microsystems, Inc.                         */
/*      Portions Copyright 1999-2001 Language Technologies Institute,               */
/*      Carnegie Mellon University.                                                 */
/*      All Rights Reserved.  Use is subject to license terms.                      */
/*                                                                                  */
/*      Permission is hereby granted, free of charge, to use and distribute         */
/*      this software and its documentation without restriction, including          */
/*      without limitation the rights to use, copy, modify, merge, publish,         */
/*      distribute, sublicense, and/or sell copies of this work, and to             */
/*      permit persons to whom this work is furnished to do so, subject to          */
/*      the following conditions:                                                   */
/*                                                                                  */
/*       1. The code must retain the above copyright notice, this list of           */
/*          conditions and the following disclaimer.                                */
/*       2. Any modifications must be clearly marked as such.                       */
/*       3. Original authors' names are not deleted.                                */
/*       4. The authors' names are not used to endorse or promote products          */
/*          derived from this software without specific prior written               */
/*          permission.                                                             */
/*                                                                                  */
/*      SUN MICROSYSTEMS, INC., CARNEGIE MELLON UNIVERSITY AND THE                  */
/*      CONTRIBUTORS TO THIS WORK DISCLAIM ALL WARRANTIES WITH REGARD TO THIS       */
/*      SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND           */
/*      FITNESS, IN NO EVENT SHALL SUN MICROSYSTEMS, INC., CARNEGIE MELLON          */
/*      UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR      */
/*      CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF      */
/*      USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR       */
/*      OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR      */
/*      PERFORMANCE OF THIS SOFTWARE.                                               */
/*                                                                                  */
/*      Authors: Willie Walker, Paul Lamere, Philip Kwok                            */
/*                                                                                  */
/************************************************************************************/


/**
 * @file item_content.h
 * Shared contents of #SItem.
 */


/**
 * @ingroup SHrg
 * @defgroup SItemContents Item Contents
 * A class for containing the shared contents of items, that is
 * the individual linguistic features and references to relations.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/errdbg/errdbg.h"
#include "containers/containers.h"
#include "hrg/hrg_defs.h"


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
 * Return the given #SItmContent child class object as a #SItmContent object.
 * @param SELF The given object.
 * @return Given object as #SItmContent* type.
 * @note This casting is not safety checked.
 */
#define S_ITMCONTENT(SELF)    ((SItmContent *)(SELF))


/**
 * @}
 */


/************************************************************************************/
/*                                                                                  */
/* SItemContents definition                                                         */
/*                                                                                  */
/************************************************************************************/

/**
 * The item content object structure.
 * A class for containing the shared contents of items, that is
 * the individual linguistic features and references to relations.
 * @extends SObject
 */
struct SItmContent
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject      obj;

	/**
	 * @protected Features for the item contents.
	 */
	SMap        *features;

	/**
	 * @protected Relations that this item content is in.
	 */
	SMap        *relations;

	/**
	 * @protected Locking mutex.
	 */
	S_DECLARE_MUTEX(content_mutex);
};


/************************************************************************************/
/*                                                                                  */
/* SItemContentsClass definition                                                    */
/*                                                                                  */
/************************************************************************************/

/**
 * The item content class structure.
 * A class for containing the shared contents of items, that is
 * the individual linguistic features and references to relations.
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
	 * @protected Add function pointer.
	 * Adds the given item from the given relation into the relations
	 * key-value container. Whenever an item is added to a relation,
	 * it should add the name and the Item reference to this set of
	 * relation/item mappings. This allows an item to find out the set
	 * of all relations that it is contained in.
	 *
	 * @param self The item content.
	 * @param relation The name of the relation of the given item.
	 * @param item The item reference in the relation.
	 * @param error Error code.
	 */
	void         (*add)           (SItmContent *self, const char *relation,
								   const SItem *item, s_erc *error);

	/**
	 * @protected Get function pointer.
	 * Given the name of a relation, return the item that shares the
	 * same item content.
	 *
	 * @param self The item content.
	 * @param relation The name of the relation of interest.
	 * @param error Error code.
	 *
	 * @return The item associated with this item content in the named
	 * relation.
	 */
	const SItem *(*get)           (const SItmContent *self, const char *relation,
								   s_erc *error);

	/**
	 * @protected Remove function pointer.
	 * Removes the relation/item (key-value) mapping from the given
	 * item content.
	 *
	 * @param self The item content.
	 * @param relation The name of the relation/item (key-value) to remove.
	 * @param error Error code.
	 */
	void         (*remove)        (SItmContent *self, const char *relation,
								   s_erc *error);


	/**
	 * @protected InRelation function pointer.
	 * Query if the named relation is in the given content's @a
	 * relations key-value container.
	 *
	 * @param self The item content.
	 * @param relation The name of the relation.
	 * @param error Error code.
	 *
	 * @return #TRUE if relation is in content else #FALSE.
	 */
	s_bool       (*in_relation)   (const SItmContent *self, const char *relation,
								   s_erc *error);

	/**
	 * @protected Features function pointer.
	 * Returns the features for this item content.
	 *
	 * @param self The item content.
	 * @param error Error code.
	 *
	 * @return A key-value map of the features for the given content.
	 */
	const SMap  *(*features)      (const SItmContent *self, s_erc *error);

	/**
	 * @protected NumRelations function pointer.
	 * Get the number of relations that the owner item of the given item
	 * content is in.
	 *
	 * @param self The item content.
	 * @param error Error code.
	 *
	 * @return The number of relations in which the item content owner
	 * item is in.
	 */
	size_t       (*num_relations) (const SItmContent *self, s_erc *error);
} SItmContentClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Adds the given item from the given relation into the relations
 * key-value container. Whenever an item is added to a relation, it
 * should add the name and the Item reference to this set of
 * relation/item mappings. This allows an item to find out the set of
 * all relations that it is contained in.
 * @public @memberof SItmContent
 *
 * @param self The item content.
 * @param relation The name of the relation of the given item.
 * @param item The item reference in the relation.
 * @param error Error code.
 */
S_API void SItmContentAdd(SItmContent *self, const char *relation,
						  const SItem *item, s_erc *error);


/**
 * Query if the named relation is in the given content's @a relations
 * key-value container.
 * @public @memberof SItmContent
 *
 * @param self The item content.
 * @param relation The name of the relation.
 * @param error Error code.
 *
 * @return #TRUE if relation is in content else #FALSE.
 */
S_API s_bool SItmContentInRelation(const SItmContent *self, const char *relation,
								   s_erc *error);


/**
 * Removes the relation/item (key-value) mapping from this item
 * content.
 * @public @memberof SItmContent
 *
 * @param self The item content.
 * @param relation The name of the relation/item (key-value) to remove.
 * @param error Error code.
 */
S_API void SItmContentRemove(SItmContent *self, const char *relation,
							 s_erc *error);


/**
 * Given the name of a relation, return the item that shares the
 * same item content.
 * @public @memberof SItmContent
 *
 * @param self The item content.
 * @param relation The name of the relation of interest.
 * @param error Error code.
 *
 * @return The item associated with this item content in the named
 * relation.
 */
S_API const SItem *SItmContentGet(const SItmContent *self, const char *relation,
								  s_erc *error);


/**
 * Returns the features for this item content.
 * @public @memberof SItmContent
 *
 * @param self The item content.
 * @param error Error code.
 *
 * @return A key-value map of the features for the given content.
 */
S_API const SMap *SItmContentFeatures(const SItmContent *self, s_erc *error);


/**
 * Get the number of relations that the owner item of the given item
 * content is in.
 * @public @memberof SItmContent
 *
 * @param self The item content.
 * @param error Error code.
 *
 * @return The number of relations in which the item content owner
 * item is in.
 */
S_API size_t SItmContentNumRelations(const SItmContent *self, s_erc *error);


/**
 * Add the SItmContent class to the object system.
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_itm_content_class_add(s_erc *error);


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

#endif /* _SPCT_ITEM_CONTENT_H__ */
