/************************************************************************************/
/* Copyright (c) 2009 The Department of Arts and Culture,                           */
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
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A halfphone database class implementation with the halfphone database entries in */
/* a SMap structure read from a EBML format file.                                   */
/* Inherits from SHalfphoneDB.                                                      */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_HALFPHONE_DB_EBML_H__
#define _SPCT_PLUGIN_HALFPHONE_DB_EBML_H__


/**
 * @file halfphone_db_ebml.h
 * A halfphone database class implementation with the halfphone
 * database entries in a SMap structure read from a EBML format file.
 */


/**
 * @ingroup SHalfphoneDB
 * @defgroup SHalfphoneDBEBML EBML Halfphone Database
 * A halfphone database class implementation with the halfphone
 * database entries in a SMap structure read from a EBML format file.
 * Inherits from #SHalfphoneDB.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "speect.h"
#include "halfphone_db.h"


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
 * Return the given #SHalfphoneDBEbml child/parent class object as a
 * #SHalfphoneDBEbml object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SHalfphoneDBEbml* type.
 * @note This casting is not safety checked.
 */
#define S_HALFPHONE_DB_EBML(SELF)    ((SHalfphoneDBEbml *)(SELF))


/************************************************************************************/
/*                                                                                  */
/* SHalfphoneDBEbml definition                                                      */
/*                                                                                  */
/************************************************************************************/

/**
 * The SHalfphoneDBEbml structure.
 * @extends SHalfphoneDB
 */
typedef struct
{
	/**
	 * @protected Inherit from #SHalfphoneDB.
	 */
	SHalfphoneDB  obj;

	/**
	 * @protected Halfphone unit catalogue.
	 */
	SMap         *catalogue;

	/**
	 * @protected A relation to keep the units in.
	 */
	SRelation    *units;
} SHalfphoneDBEbml;


/************************************************************************************/
/*                                                                                  */
/* SHalfphoneDBEbmlClass definition                                                 */
/*                                                                                  */
/************************************************************************************/

/**
 * Definition of the SHalfphoneDBEbml class. This class adds no class methods to the
 * #SHalfphoneDBClass and is therefore exactly the same.
 */
typedef SHalfphoneDBClass SHalfphoneDBEbmlClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SHalfphoneDBEbml plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_halfphone_db_ebml_class_reg(s_erc *error);

/**
 * Free the #SHalfphoneDBEbml plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_halfphone_db_ebml_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_HALFPHONE_DB_EBML_H__ */
