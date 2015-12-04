/************************************************************************************/
/* Copyright (c) 2012 The Department of Arts and Culture,                           */
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
/* DATE    : May 2012                                                               */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A part-of-speech tagger utterance processor using hunpos.                        */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_UTTPROCESSOR_HUNPOS_H__
#define _SPCT_PLUGIN_UTTPROCESSOR_HUNPOS_H__


/**
 * @file hunpos_proc.h
 * A part-of-speech tagger utterance processor using hunpos.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "speect.h"
#include <hunpos.h>


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Typedef                                                                          */
/*                                                                                  */
/************************************************************************************/

/**
 * The SHunposUttProc structure.
 * Typedef of the hunpos part-of-speech utterance processor.
 * @extends SUttProcessor
 */
typedef struct
{
	/**
	 * @protected Inherit from #SUttProcessor.
	 */
	SUttProcessor obj;

	/**
	 * @protected Maximum number of guessed tags.
	 */
	int           max_guessed_tags;

	/**
	 * @protected Theta
	 */
	int           theta;

	/**
	 * @protected The hunpos model file.
	 */
	char         *model_file;

	/**
	 * @protected Maximum lenght of token passed in a single call to hunpos.
	 */
	int           max_tokens_number;

	/**
	 * @protected The hunpos instance.
	 */
	Hunpos       *hunpos_instance;
} SHunposUttProc;


/************************************************************************************/
/*                                                                                  */
/* SHunposUttProcClass definition                                                   */
/*                                                                                  */
/************************************************************************************/

/**
 * Typedef of the hunpos part-of-speech utterance processor class. Does not add any
 * new methods, therefore exactly the same as #SUttProcessorClass.
 */
typedef SUttProcessorClass SHunposUttProcClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SHunposUttProc plug-in class with the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_hunpos_utt_proc_class_reg(s_erc *error);


/**
 * Free the #SHunposUttProc plug-in class from the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_hunpos_utt_proc_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_UTTPROCESSOR_HUNPOS_H__ */

