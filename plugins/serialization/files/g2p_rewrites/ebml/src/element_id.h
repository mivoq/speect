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
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* G2PRewrites EBML element id's.                                                   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_G2PREWRITES_EBML_ELEMENT_ID__
#define _SPCT_PLUGIN_G2PREWRITES_EBML_ELEMENT_ID__


/**
 * @file element_id.h
 * G2PRewrites EBML element id's.
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
 * Speect G2PRewrites EBML id
 */
#define S_G2PREWRITES_EBML 0x1371111b


/**
 * G2PRewrites definition
 */
#define S_G2PREWRITES_EBML_DEF               0x1111111a
#define S_G2PREWRITES_EBML_DEF_NAME          0x1111111b
#define S_G2PREWRITES_EBML_DEF_LANGUAGE      0x1111111c
#define S_G2PREWRITES_EBML_DEF_LANG_CODE     0x1111111d
#define S_G2PREWRITES_EBML_DEF_DESCRIPTION   0x1111111e
#define S_G2PREWRITES_EBML_DEF_VERSION       0x1111111f
#define S_G2PREWRITES_EBML_DEF_VERSION_MAJOR 0x11111120
#define S_G2PREWRITES_EBML_DEF_VERSION_MINOR 0x11111121


/**
 * Sets
 */
#define S_G2PREWRITES_EBML_SETS                     0x111111a0
#define S_G2PREWRITES_EBML_SETS_NAME                0x83
#define S_G2PREWRITES_EBML_SETS_GRAPHEMES_CONTAINER 0x84
#define S_G2PREWRITES_EBML_SETS_GRAPHEME            0x85


/**
 * Rules
 */
#define S_G2PREWRITES_EBML_RULES_CONTAINER           0x111111a1
#define S_G2PREWRITES_EBML_RULES_GRAPHEME            0x86
#define S_G2PREWRITES_EBML_SINGLE_RULE_CONTAINER     0x87
#define S_G2PREWRITES_EBML_SINGLE_RULE_LEFT_CONTEXT  0x88
#define S_G2PREWRITES_EBML_SINGLE_RULE_RIGHT_CONTEXT 0x89
#define S_G2PREWRITES_EBML_SINGLE_RULE_PHONEME       0x8a


/**
 * Zeros
 */
#define S_G2PREWRITES_EBML_ZEROS             0x111111a2
#define S_G2PREWRITES_EBML_ZEROS_SYMBOL      0x8b
#define S_G2PREWRITES_EBML_ZEROS_REPLACEMENT 0x8c


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

#endif /* _SPCT_PLUGIN_G2PREWRITES_EBML_ELEMENT_ID__ */
