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
/* AUTHOR  : Gerrit Botha, Aby Louw                                                 */
/* DATE    : 20 August 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* SObject EBML id's.                                                               */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_EBML_DEF_H__
#define _SPCT_EBML_DEF_H__


/**
 * @file ebml_def.h
 * #SObject EBML id's.
 */


/**
 * @ingroup SEbml
 * @defgroup SPCT_EBML_DEFS EBML SObject Definitions
 * EBML definitions for reading and writing #SObject objects.
 *
 * The ebml definition of an #SObject object is as follows:
 * @verbatim
sobject := container [ order:yes ]
{
    SObjectClassName := 0x82 string;
    SObjectData = 0x83 binary;
}
@endverbatim
 * therefore, an object should read/write it's own class name (<tt>id = 0x81</tt>), and
 * it's data in a container (<tt>id = 0x82</tt>). The class name and data id's are fixed
 * and should be used as defined above.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

/**
 * Object class name EBML id.
 */
#define S_EBML_ID_OBJECT_CLASSNAME      0x81


/**
 * Object data EBML id.
 */
#define S_EBML_ID_OBJECT_DATA           0x82


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

#endif /* _SPCT_EBML_DEF_H__ */

