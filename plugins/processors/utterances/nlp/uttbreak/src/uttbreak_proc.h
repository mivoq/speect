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
/* DATE    : June 2012                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A Utterance break utterance processor.                                           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_UTTPROCESSOR_UTTBREAK_H__
#define _SPCT_PLUGIN_UTTPROCESSOR_UTTBREAK_H__


/**
 * @file uttbreak_proc.h
 * A Utterance break utterance processor.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "speect.h"


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
 * Return the given #SUttBreakUttProc child/parent class object as a
 * #SUttBreakUttProc object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SUttBreakUttProc* type.
 * @note This casting is not safety checked.
 */
#define S_UTTBREAK_UTTPROC(SELF)    ((SUttBreakUttProc *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SUttBreakUttProc,
 * see full description #S_UTTBREAK_UTTPROC_CALL for usage.
 *
 * @param SELF The given #SUttBreakUttProc*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_UTTBREAK_UTTPROC_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_UTTBREAK_UTTPROC_CALL(SELF, FUNC)				\
	((SUttBreakUttProcClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SUttBreakUttProc
 * can be called.
 *
 * @param SELF The given #SUttBreakUttProc*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_UTTBREAK_UTTPROC_METH_VALID(SELF, FUNC)			\
	S_UTTBREAK_UTTPROC_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* SUttBreakUttProc definition                                                      */
/*                                                                                  */
/************************************************************************************/

/**
 * The SUttBreakUttProc structure.
 * An utterance processor to break a tokenstream into utterances.
 * @extends SUttProcessor
 */
typedef struct
{
	/**
	 * @protected Inherit from #SUttProcessor.
	 */
	SUttProcessor obj;

	/**
	 * @protected Preceding white-space symbols.
	 */
	char         *white_space_chars;

	/**
	 * @protected Single character symbols.
	 */
	char         *single_char_symbols;

	/**
	 * @protected Pre-punctuation symbols.
	 */
	char         *pre_punc_symbols;

	/**
	 * @protected Post-punctuation symbols.
	 */
	char         *post_punc_symbols;
} SUttBreakUttProc;


/************************************************************************************/
/*                                                                                  */
/* SUttBreakUttProcClass definition                                                 */
/*                                                                                  */
/************************************************************************************/

/**
 * The utterance break utterance processor class structure.
 * @extends SObjectClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SUttProcessorClass.
	 */
	SUttProcessorClass _inherit;

	/* Class methods */
	/**
	 * Set the token stream's symbols to that of this utterance break
	 * processor.
	 *
	 * @param self The given UttProcessor.
	 * @param ts The token stream from which to create the utterances.
	 * @param error Error code.
	 */
	void (* const set_tokenstream_symbols)(const SUttBreakUttProc *self,
										   STokenstream *ts, s_erc *error);


 	/**
	 * @protected Get the next utterance of the given token stream
	 * The returned utterance will have a token relation
	 * with items corresponding to the tokens of the token stream.
	 *
	 * @param self The given UttProcessor.
	 * @param voice The voice associated with the utterances.
	 * @param ts The token stream from which to create the utterances.
	 * @param error Error code.
	 *
	 * @return The next utterance in the given token stream. @c NULL
	 * if no more utterances.
	 *
	 * @note Caller is responsible for the memory of the returned
	 * utterance.
	 */
	SUtterance *(* const get_next_utt)(const SUttBreakUttProc *self,
									   const SVoice *voice,
									   STokenstream *ts, s_erc *error);
} SUttBreakUttProcClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SUttBreakUttProc plug-in class with the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_uttbreak_utt_proc_class_reg(s_erc *error);


/**
 * Free the #SUttBreakUttProc plug-in class from the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_uttbreak_utt_proc_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_UTTPROCESSOR_UTTBREAK_H__ */
