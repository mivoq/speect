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


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/utils/types.h"
#include "base/errdbg/errdbg.h"
#include "base/objsystem/objsystem.h"


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
 * @ingroup SContainers
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
 * @ingroup SContainers
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
 * @ingroup SContainers
 * The SContainerClass structure.
 * The abstract SContainerClass defines no methods, child classes can define
 * methods as required.
 * @extends SObjectClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SObjectClass.
	 */
	SObjectClass  _inherit;

	/* No class methods, child classes supply methods. */
} SContainerClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

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
