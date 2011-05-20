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

#ifndef _SPCT_CLASS_H__
#define _SPCT_CLASS_H__


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
 * @file class.h
 * Class functions.
 */


/**
 * @ingroup SObjSystem
 * @defgroup SBaseClass Class Functions
 * Provides class type functions.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/errdbg/errdbg.h"
#include "base/objsystem/object_def.h"


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
 * @hideinitializer
 * Get the given class's base class.
 *
 * @param SELF The given class.
 *
 * @return Pointer to the #SObjectClass class of the given class.
 *
 * @note This casting is not safety checked.
 */
#define S_OBJECTCLASS(SELF) ((SObjectClass*)SELF)


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Add a class to the object system. The class gets added to the table
 * of classes available for object instantiation.
 *
 * @param cls Class object to add.
 * @param error Error Code.
 *
 * @note Class names must be unique.
 */
S_API void s_class_add(const SObjectClass *cls, s_erc *error);


/**
 * Initialize a class with the object system. The object system will
 * get the class's inheritance hierarchy ready for instantiating class
 * objects.
 *
 * @param cls Class object to initialize.
 * @param error Error Code.
 */
S_API void s_class_init(const SObjectClass *cls, s_erc *error);


/**
 * Register a class with the object system. This performs the function
 * of #s_class_add and #s_class_init and is the preferred method for
 * adding new classes through plug-ins.
 *
 * @param cls Class object to register.
 * @param error Error Code.
 */
S_API void s_class_reg(const SObjectClass *cls, s_erc *error);


/**
 * Free a class from the object system. After this function is called
 * on a class no objects of that class type can be instantiated
 * anymore. Plugins must call this function on exit for any classes
 * registered by the plug-in.
 *
 * @param cls Class object to free.
 * @param error Error Code.
 */
S_API void s_class_free(const SObjectClass *cls, s_erc *error);


/**
 * Get the name of the given class. The inheritance hierarchy is
 * removed from the class's registered name and only the actual class
 * name is returned.
 *
 * @param cls Class object to get name from.
 * @param error Error Code.
 *
 * @return The class name.
 */
S_API const char *s_class_name(const SObjectClass *cls, s_erc *error);


/**
 * Get the complete inheritance of the given class. The inheritance
 * will exclude the #SObject.
 *
 * @param cls Class object to get inheritance from.
 * @param error Error Code.
 *
 * @return A ":" separated string of the given classes inheritance
 * hierarchy, excluding #SObject.
 */
S_API const char *s_class_inheritance(const SObjectClass *cls, s_erc *error);


/**
 * Query if the given class name is registered.
 *
 * @param name The class name to query.
 * @param error Error code.
 *
 * @return @c TRUE if registered else @c FALSE.
 */
S_API s_bool s_class_is_reg(const char *name, s_erc *error);


/**
 * Get a class object by name.
 *
 * @param name The name of the desired class.
 * @param error Error Code.
 *
 * @return The class object.
 */
S_API const SObjectClass *s_class_find(const char *name, s_erc *error);


/**
 * @ingroup SBaseObject
 * Delete an object.
 * Free up any resources allocated to the object by the
 * #SObjectClass::init function.
 * @private @memberof SObject
 *
 * @param self Object to delete.
 * @param error Error Code.
 *
 * @note Do @b not use this function directly, use either
 * #S_DELETE or #S_FORCE_DELETE.
 */
S_API void SObjectDelete(SObject *self, s_erc *error);


/**
 * @ingroup SBaseObject
 * Allocates a new object instance of the named object type and
 * initializes its members.
 * @private @memberof SObject
 *
 * @param name The name of the object type from which to create a new
 * object.
 * @param error Error code.
 *
 * @return Pointer to the newly created object.
 *
 * @note Do not use this function, rather use one of the macros
 * #S_NEW.
 *
 * @todo check that everything is free'd on error.
 */
S_API SObject *SObjectNewFromName(const char *name, s_erc *error);


/**
 * @ingroup SBaseObject
 * Initialize an object by invoking the init() operation of
 * every class in the inheritance hierarchy of the given object.
 * @private @memberof SObject
 *
 * @param obj Object to initialize.
 * @param error Error Code.
 */
S_LOCAL void SObjectInit(SObject *obj, s_erc *error);


/**
 * Create the Speect class repository with the given size
 * as the initial size of the class table.  The initial size
 * of the table must be chosen with care as resizing a table
 * is an expensive operation. Used internally by #speect_init.
 * @private
 *
 * @param size The initial size of the class table.
 * @param error Error code.
 */
S_LOCAL void _s_classes_create(size_t size, s_erc *error);


/**
 * Initialize the Speect class repository.
 * Used internally by #speect_init.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_classes_init(s_erc *error);


/**
 * Iterate through class repository and free each one, the delete
 * the class repository.
 * Used internally by #speect_quit.
 *
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_classes_clear(s_erc *error);


#ifdef SPCT_DEBUGMODE
/**
 * Print stats and information about the Speect Engine class
 * repository to @c stdout.
 *
 *
 * @param error Error code.
 */
S_LOCAL void _s_classes_print(s_erc *error);
#endif /* SPCT_DEBUGMODE */


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

#endif /* _SPCT_CLASS_H__ */
