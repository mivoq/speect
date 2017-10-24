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
/* A Textexp Normalization utterance processor.                                     */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_UTTPROCESSOR_TEXTEXP_NORMALIZATION_H__
#define _SPCT_PLUGIN_UTTPROCESSOR_TEXTEXP_NORMALIZATION_H__


/**
 * @file normalizer_proc.h
 * A Textexp Normalization utterance processor.
 */


/**
 * @ingroup SUttProc
 * @defgroup SUttProcNormalization TextexpNormalization
 * A Normalization utterance processor.
 *
 * Depends on the presence of the @e Token relation in the utterance
 * and creates the @e Word relation.
 *
 * The @e Token relation's items are normalized, and for each word of
 * a token item's normalized name feature a daughter item is created.
 * For each of these daughter item's a new item is created in the @e
 * Word relation, this word item shares it's content with the daughter
 * items of the @e Token relation.
 *
 * If the <i> word-normalization </i> feature processor is
 * present in the voice of the utterance then the @e Token relation's
 * items are sent to it, one after the other, and the <i>
 * word-normalization </i> feature processor must normalize and set
 * the correct item's in the @e Word relation, which will have been
 * created.
 *
 * @dot
 digraph utt_example
 {
 label="Utterance example after running normalization utterance processor."
	{
		node [shape=box, style=filled, fillcolor="#e58989", color="#d41919"]

			utt [ label="Utterance" ]
			}

	{
		node [shape=box, style=filled, fillcolor="#7289ff", color="#0026e7"]

			Token [ label="Token" ]
			Word [ label="Word" ]
			}

	{
		edge [style="invis"]
			Token -> Word
			}

	{
		edge [ label = "relation", labelfloat=true, fontsize=10 ]
			utt -> Token
			utt -> Word
			}

	{
		node [shape=box, style=filled, fillcolor="#ccffcc", color="#558855"]
			rank=same;
		Token
			tokenItem1 [ label = "I" ]
			tokenItem2 [ label = "was" ]
			tokenItem3 [ label = "born" ]
			tokenItem4 [ label = "in" ]
			tokenItem5 [ label = "1956" ]
			tokenItem6 [ label = "at" ]
			tokenItem7 [ label = "12" ]
			tokenItem8 [ label = "noon" ]
			}

	{
		edge [ label = "head", fontsize=10 ]
			Token -> tokenItem1

			edge [ label = "next", fontsize=10 ]
			tokenItem1 ->
			tokenItem2 ->
			tokenItem3 ->
			tokenItem4 ->
			tokenItem5 ->
			tokenItem6 ->
			tokenItem7 ->
			tokenItem8;
	}

	{
		node [shape=box, style=filled, fillcolor="#ccffcc", color="#558855"]
			rank=same;
		tokenDaughterItem1 [ label = "i" ]
			tokenDaughterItem2 [ label = "was" ]
			tokenDaughterItem3 [ label = "born" ]
			tokenDaughterItem4 [ label = "in" ]
			tokenDaughterItem5 [ label = "nineteen" ]
			tokenDaughterItem6 [ label = "fifty" ]
			tokenDaughterItem7 [ label = "six" ]
			tokenDaughterItem8 [ label = "at" ]
			tokenDaughterItem9 [ label = "twelve" ]
			tokenDaughterItem10 [ label = "noon" ]
			}

	{
		edge [ label = "next", fontsize=10 ]
			tokenDaughterItem5 ->
			tokenDaughterItem6->
			tokenDaughterItem7;
	}

	{
		edge [ label="daughter", fontsize=10 ]
			tokenItem1 -> tokenDaughterItem1
			tokenItem2 -> tokenDaughterItem2
			tokenItem3 -> tokenDaughterItem3
			tokenItem4 -> tokenDaughterItem4
			tokenItem5 -> tokenDaughterItem5
			tokenItem6 -> tokenDaughterItem8
			tokenItem7 -> tokenDaughterItem9
			tokenItem8 -> tokenDaughterItem10
			}

	{
		node [shape=box, style=filled, fillcolor="#ccffcc", color="#558855"]
			rank=sink;
		Word
			wordItem1 [ label = "i"];
		wordItem2 [ label = "was"];
		wordItem3 [ label = "born"];
		wordItem4 [ label = "in"];
		wordItem5 [ label = "nineteen" ]
			wordItem6 [ label = "fifty" ]
			wordItem7 [ label = "six" ]
			wordItem8 [ label = "at" ]
			wordItem9 [ label = "twelve" ]
			wordItem10 [ label = "noon" ]
			}

	{
		edge [ label = "head", fontsize=10 ]
			Word -> wordItem1

			edge [ label = "next", fontsize=10 ]
			wordItem1 ->
			wordItem2 ->
			wordItem3 ->
			wordItem4 ->
			wordItem5 ->
			wordItem6 ->
			wordItem7 ->
			wordItem8 ->
			wordItem9 ->
			wordItem10;
	}

	{
		edge [ label = "shared content", style=dotted, fontsize=10 ]

			wordItem1 -> tokenDaughterItem1
			wordItem2 -> tokenDaughterItem2
			wordItem3 -> tokenDaughterItem3
			wordItem4 -> tokenDaughterItem4
			wordItem5 -> tokenDaughterItem5
			wordItem6 -> tokenDaughterItem6
			wordItem7 -> tokenDaughterItem7
			wordItem8 -> tokenDaughterItem8
			wordItem9 -> tokenDaughterItem9
			wordItem10 -> tokenDaughterItem10
			}
}
@enddot
 * @{
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
/* Typedef                                                                          */
/*                                                                                  */
/************************************************************************************/

/**
 * Typedef of the normalization utterance processor. Does not add any
 * new members, therefore exactly the same as #SUttProcessor.
 */
typedef SUttProcessor STextexpNormalizationUttProc;


/**
 * Typedef of the normalization utterance processor class. Does not add any
 * new methods, therefore exactly the same as #SUttProcessor.
 */
typedef SUttProcessorClass STextexpNormalizationUttProcClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #STextexpNormalizationUttProc plug-in class with the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_textexp_normalization_utt_proc_class_reg(s_erc *error);


/**
 * Free the #STextexpNormalizationUttProc plug-in class from the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_textexp_normalization_utt_proc_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_UTTPROCESSOR_TEXTEXP_NORMALIZATION_H__ */

