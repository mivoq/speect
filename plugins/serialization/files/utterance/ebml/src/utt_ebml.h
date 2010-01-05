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
/* SSerializedFile implementation for EBML format utterances.                       */
/* Read/Write utterances from/to EBML formatted files.                              */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_SERIALIZED_EBML_UTTERANCES__
#define _SPCT_PLUGIN_SERIALIZED_EBML_UTTERANCES__


/**
 * @file utt_ebml.h
 * SSerializedFile implementation for EBML format utterances.
 */


/**
 * @ingroup SSerializedFile
 * @defgroup SEbmlUtterance Serialized EBML Utterance
 * SSerializedFile implementation for EBML format utterances. Loads
 * EBML plug-in as well as primitives EBML plug-in.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "speect.h"
#include "ebml.h"
#include "element_id.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_serialized_ebml_utt_reg(s_erc *error);


/**
 * Free the plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_serialized_ebml_utt_free(s_erc *error);


/**
 * Read an EBML format utterance from the given data source.
 *
 * @param ds The data source from where to read the utterance.
 * @param error Error code.
 *
 * @return Loaded utterance or @c NULL on error.
 */
S_LOCAL SUtterance *s_read_utt_ebml(SDatasource *ds, s_erc *error);


/**
 * Write an utterance in EBML format to the given data source.
 *
 * @param utt The utterance to write.
 * @param ds The data source where to write to.
 * @param error Error code.
 */
S_LOCAL void s_write_utt_ebml(const SUtterance *utt, SDatasource *ds, s_erc *error);


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

#endif /* _SPCT_PLUGIN_SERIALIZED_EBML_UTTERANCES__ */
