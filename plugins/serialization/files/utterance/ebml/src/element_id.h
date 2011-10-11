/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
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
/* DATE    : April 2009                                                             */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Utterance EBML element id's.                                                     */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_UTT_EBML_ELEMENT_ID__
#define _SPCT_PLUGIN_UTT_EBML_ELEMENT_ID__


/**
 * @file element_id.h
 * Utterance EBML element id's.
 */


/**
 * @ingroup SUtterance
 * @defgroup SUtteranceEBMLID Utterance EBML ID's
 * Utterance EBML element id's.
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
 * Speect Utterance EBML id
 */
#define S_UTT_EBML 0x1371111a


/**
 * Utterance features
 */
#define S_UTT_EBML_FEATURES     0x1111111e
#define S_UTT_EBML_FEAT_NAME    0x83
#define S_UTT_EBML_FEAT_OBJECT  0x84


/**
 * Utterance Items content
 */
#define S_UTT_EBML_ITEMS_CONTENTS             0x1111111f
#define S_UTT_EBML_ITEMS_CONTENT_ID           0x85
#define S_UTT_EBML_ITEMS_CONTENT_FEATURES     0x86
#define S_UTT_EBML_ITEMS_CONTENT_FEAT_NAME    0x87
#define S_UTT_EBML_ITEMS_CONTENT_FEAT_OBJECT  0x88


/**
 * Utterance Relations
 */
#define S_UTT_EBML_RELATIONS                 0x11111120
#define S_UTT_EBML_RELATION_NAME             0x89
#define S_UTT_EBML_RELATION_ITEMS            0x8a
#define S_UTT_EBML_RELATION_ITEM_NODE        0x8b
#define S_UTT_EBML_RELATION_ITEM_CONTENT_ID  0x8c
#define S_UTT_EBML_RELATION_ITEM_UP_NODE     0x8d
#define S_UTT_EBML_RELATION_ITEM_DOWN_NODE   0x8e
#define S_UTT_EBML_RELATION_ITEM_NEXT_NODE   0x8f
#define S_UTT_EBML_RELATION_ITEM_PREV_NODE   0x90


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

#endif /* _SPCT_PLUGIN_UTT_EBML_ELEMENT_ID__ */
