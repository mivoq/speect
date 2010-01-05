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
/* DATE    : 10 August 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* An abstract _structured_ data format reader class.                               */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_DATA_READER_H__
#define _SPCT_DATA_READER_H__


/**
 * @file data_reader.h
 * Abstract structured (formatted) data reader.
 */


/**
 * @ingroup SDatasource
 * @defgroup StructuredData Structured Data Format
 * Structured data format readers and writers.
 * @todo explain
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <stdio.h>
#include "include/common.h"
#include "base/objsystem/objsystem.h"
#include "base/errdbg/errdbg.h"
#include "datasources/data_source.h"


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
 * Return the given #SDataReader child class object as a #SDataReader object.
 * @param SELF The given object.
 * @return Given object as #SDataReader* type.
 * @note This casting is not safety checked.
 */
#define S_DATAREADER(SELF)    ((SDataReader *)(SELF))


/**
 * @}
 */


/************************************************************************************/
/*                                                                                  */
/* SDataReader definition                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * The abstract data reader structure. Provides an abstract interface to a @a structured
 * data format reader. All classes that want to read an #SObject type to file must
 * inherit from this object's class.
 * @extends SObject
 */
struct SDataReader  /* typedef is in object_def.h */
{
	/**
	 * @protected Inherit from SObject.
	 */
	SObject        obj;

	/**
	 * @protected The data source to read from.
	 */
	SDatasource   *ds;
};


/************************************************************************************/
/*                                                                                  */
/* SDataReaderClass definition                                                      */
/*                                                                                  */
/************************************************************************************/

/**
 * The data reader class structure.
 * The abstract SDataReaderClass defines no methods, child classes can define
 * methods as required.
 * @extends SObjectClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from SObjectClass. */
	SObjectClass  _inherit;

	/* No class methods, child classes supply methods. */
} SDataReaderClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Add the SDataReader class to the object system.
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_data_reader_class_add(s_erc *error);


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

#endif /* _SPCT_DATA_READER_H__ */

