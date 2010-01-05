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
/* Initialization, configuration and shutdown of the object system.                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_OBJSYSTEM_H__
#define _SPCT_OBJSYSTEM_H__


/**
 * @file objsystem.h
 * Generic object system.
 */


/**
 * @ingroup SBase
 * @defgroup SObjSystem Generic Object System
 * Provides an object-oriented approach for data types.
 * The object system provides basic object-oriented programming support (e.g., inheritance,
 * polymorphism, encapsulation, abstraction, decoupling etc.) to the Speect Engine by
 * using @c ISO @c C structures as objects and classes.
 *
 * <h3> Class </h3>
 * Classes are statically allocated and are used to create new instances of objects and to
 * define the interface of the object. The interface is defined with function pointers in
 * the class structure. Classes also contain information about the class hierarchy, the size
 * of the objects instantiated by the class, and version information.
 *
 * <h3> Object </h3>
 * Objects are dynamically allocated by the object's class. The first member of an object
 * structure definition is always an object of the object's superclass type. Objects keep
 * a reference count.
 *
 * @{
 * @todo headerfiles
 */


/************************************************************************************/
/*                                                                                  */
/*  Include files in objsystem                                                      */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/objsystem/object_macros.h"  /* Object/Class macros.                  */
#include "base/objsystem/object_def.h"     /* Object/Class definition.              */
#include "base/objsystem/object.h"         /* Object functions.                     */
#include "base/objsystem/class.h"          /* Class functions.                      */
#include "base/objsystem/primitives.h"     /* Primitive SObject types.              */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/errdbg/errdbg.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Initialize the object system.
 * Used internally by #speect_init.
 *
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_objsystem_init(s_erc *error);


/**
 * Quit the object system.
 * Used internally by #speect_quit.
 *
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_objsystem_quit(s_erc *error);


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

#endif /* _SPCT_OBJSYSTEM_H__ */

