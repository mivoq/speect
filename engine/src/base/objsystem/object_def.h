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
/* DATE    : 5 July 2008                                                            */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Implementation of the generic object system.                                     */
/*                                                                                  */
/* Adapted from Agar toolkit http://libagar.org/ (1.3.2)                            */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_OBJECT_DEF_H__
#define _SPCT_OBJECT_DEF_H__


/************************************************************************************/
/* libagar license                                                                  */
/*                                                                                  */
/* Copyright (c) 2002-2007 Hypertriton, Inc. <http://hypertriton.com/>              */
/* All rights reserved.                                                             */
/*                                                                                  */
/* Redistribution and use in source and binary forms, with or without               */
/* modification, are permitted provided that the following conditions               */
/* are met:                                                                         */
/* 1. Redistributions of source code must retain the above copyright                */
/*    notice, this list of conditions and the following disclaimer.                 */
/* 2. Redistributions in binary form must reproduce the above copyright             */
/*    notice, this list of conditions and the following disclaimer in the           */
/*    documentation and/or other materials provided with the distribution.          */
/*                                                                                  */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"      */
/* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE        */
/* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE       */
/* ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR       */
/* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL           */
/* DAMAGES (INCLUDING BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR        */
/* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER       */
/* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,    */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE        */
/* USE OF THIS SOFTWARE EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.          */
/*                                                                                  */
/************************************************************************************/


/**
 * @file object_def.h
 * Definition of the base object and class.
 */


/**
 * @ingroup SObjSystem
 * @defgroup SBaseObjectDef Base Object Definition
 * Provides the base object.
 * SObject provides an interface to a generic data container.
 * It is a simple object which can contain any other objects and
 * allows entities to store @a any type of data. Any data type
 * can inherit form the SObject class, and implement the basic
 * methods, or not. An object can also extend these as required.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/errdbg/errdbg.h"
#include "base/utils/vernum.h"
#include "base/utils/types.h"


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

/* Forward definitions of structured data format reader and writer. */

/**
 * @ingroup StructuredData
 * Definition of the <i> structured (formatted) data source reader </i>.
 */
typedef struct SDataReader SDataReader;


/**
 * @ingroup StructuredData
 * Definition of the <i> structured (formatted) data source writer </i>.
 */
typedef struct SDataWriter SDataWriter;


/**
 * @ingroup StructuredData
 * Definition of the <i>serialized object</i> data source.
 */
typedef struct SSerializedObject SSerializedObject;


/************************************************************************************/
/*                                                                                  */
/* SObject definition                                                               */
/*                                                                                  */
/************************************************************************************/

/* Forward definition of the base class */
typedef struct SObjectClass SObjectClass; /*!< Base class structure */


/**
 * The base object structure. The base object defines the object data
 * members and inherited data members of the object. All objects that
 * want to work with the @ref SObjSystem must inherit from this
 * object.
 */
typedef struct
{
	/**
	 * @protected Pointer to the class of this object.
	 */
	const SObjectClass *cls;

	/**
	 * @protected Reference count.
	 */
	uint32        ref;

	/**
	 * @protected Object mutex.
	 */
	S_DECLARE_MUTEX(object_mutex);
} SObject;


/************************************************************************************/
/*                                                                                  */
/* SObjectClass definition                                                          */
/*                                                                                  */
/************************************************************************************/

/**
 * The base class structure. The class defines a constructor and
 * destructor for the class objects, as well as class members, i.e.
 * members that belong to all object instances of a specific class
 * type. All classes inherit from this class structure.
 *
 * @todo better docs for class methods?
 * @todo destroy must check all object members before delete (for
 * NULL).
 */
struct SObjectClass
{
	/* Class members */
	/**
	 * @protected Class hierarchy name.
	 */
	const char *name;

	/**
	 * @protected he size of the objects of this class, in bytes (sizeoff).
	 */
	size_t      size;

	/**
	 * @protected The class version.
	 */
	s_version   ver;

	/* Class methods */
	/**
	 * @protected Constructor function pointer.
	 * Initialize the members of the objects of this class type.
	 * This function receives a chunck of allocated memory for the
	 * object (will @a always be valid memory), and
	 * initializes/allocated the members of the object of this class
	 * type.
	 *
	 * @param obj Memory for the newly created object.
	 * @param error Error code.
	 */
	void     (*init)    (void *obj, s_erc *error);

	/**
	 * @protected Destructor function pointer.
	 * Free up any resources allocated to the object by the
	 * #SObjectClass::init function. The object's memory itself must
	 * @b not be freed.
	 *
	 * @param obj The object to be destroyed.
	 * @param error Error code.
	 *
	 * @note See #S_DELETE and #S_FORCE_DELETE.
	 */
	void     (*destroy) (void *obj, s_erc *error);

	/**
	 * @protected Dispose function pointer.
	 * Decrement the object's reference counter if the object
	 * can be deleted by a call to #S_DELETE.
	 *
	 * @param obj The object to be disposed.
	 * @param error Error code.
	 *
	 * @note See #S_DELETE and #S_FORCE_DELETE.
	 */
	void     (*dispose) (void *obj, s_erc *error);

	/**
	 * @protected Comparison function pointer.
	 * Compare the two given objects with each other and return,
	 * either #TRUE or #FALSE, if the objects are equal or
	 * not. The given objects @a will always be the same type and
	 * valid. The two given objects reference must not be factored
	 * into the comparison.
	 *
	 * @param first Object to be compared with second.
	 * @param second Object to be compared with first.
	 * @param error Error code.
	 *
	 * @return #TRUE if the objects are the same, else #FALSE.
	 */
	s_bool   (*compare) (const SObject *first, const SObject *second, s_erc *error);

	/**
	 * @protected Print function pointer.
	 * Print the given object to a character string buffer.
	 *
	 * @param self The object to print.
	 * @param error Error code.
	 *
	 * @return Pointer to a character buffer containing a textual
	 * representation of the given object. Utf8 encoding is assumed.
	 */
	char    *(*print)   (const SObject *self, s_erc *error);

	/**
	 * @protected Copy function pointer.
	 * Copy (deep) the given object.
	 * Create a new object and deep copy all the given object's
	 * members to the new object. If the given object has other
	 * objects as data members they must in turn be copied with a call
	 * to #SObjectCopy. The new object's reference count must be 0.
	 *
	 * @param self The object to copy.
	 * @param error Error code.
	 *
	 * @return Pointer to new copy of the given object.
	 */
	SObject *(*copy)    (const SObject *self, s_erc *error);
};


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

#endif /* _SPCT_OBJECT_DEF_H__ */
