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
/* HalfphoneDB EBML element id's.                                                   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_HALFPHONE_DB_EBML_ELEMENT_ID__
#define _SPCT_PLUGIN_HALFPHONE_DB_EBML_ELEMENT_ID__


/**
 * @file element_id.h
 * Halfphone database EBML element id's.
 */


/**
 * @inggroup SHalfphoneDBEBML
 * Halfphone database EBML element id's
 * @{
 */


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
 * Speect Halfphone database EBML id
 */
#define S_HALFPHONE_DB_EBML_ID 0x1371111c


/**
 * Halfphone database definition
 */
#define S_HALFPHONE_DB_EBML_DEF               0x1111111a
#define S_HALFPHONE_DB_EBML_DEF_NAME          0x1111111b
#define S_HALFPHONE_DB_EBML_DEF_LANGUAGE      0x1111111c
#define S_HALFPHONE_DB_EBML_DEF_LANG_CODE     0x1111111d
#define S_HALFPHONE_DB_EBML_DEF_DESCRIPTION   0x1111111e
#define S_HALFPHONE_DB_EBML_DEF_VERSION       0x1111111f
#define S_HALFPHONE_DB_EBML_DEF_VERSION_MAJOR 0x11111120
#define S_HALFPHONE_DB_EBML_DEF_VERSION_MINOR 0x11111121


/**
 * Halfphone database features
 */
#define S_HALFPHONE_DB_EBML_FEATURES     0x11111122
#define S_HALFPHONE_DB_EBML_FEAT_NAME    0x83
#define S_HALFPHONE_DB_EBML_FEAT_OBJECT  0x84


/**
 * Halfphone database units
 */
#define S_HALFPHONE_DB_EBML_UNITS            0x11111123
#define S_HALFPHONE_DB_EBML_UNIT_FEATURES    0x85
#define S_HALFPHONE_DB_EBML_UNIT_FEAT_NAME   0x86
#define S_HALFPHONE_DB_EBML_UNIT_FEAT_OBJECT 0x87


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

#endif /* _SPCT_PLUGIN_HALFPHONE_DB_EBML_ELEMENT_ID__ */
