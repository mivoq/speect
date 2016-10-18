/************************************************************************************/
/* Copyright (c) 2010-2011 The Department of Arts and Culture,                      */
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
/* AUTHOR  : Federico Rossetto                                                    */
/* DATE    : July 2016                                                              */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Get features from rules.                                                         */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_UTTPROCESSOR_RULE_BASED_FEATURES_H__
#define _SPCT_PLUGIN_UTTPROCESSOR_RULE_BASED_FEATURES_H__


/**
 * @file rule_based_endtone.h
 * Get features from rules.
 */


/**
 * @ingroup SFeatureProc
 * @defgroup SHTSLabelsGeneratorSimple2FeatProc Get features from rules.
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
 * Typedef of the rulebasedfeatures utterance processor. Does not add any
 * new members, therefore exactly the same as #SUttProcessor.
 */
typedef struct
{
	SUttProcessor obj;

	/* 'prosSymbols' holds the complete map read from voice.json's 'list definitions' */
	/* Modified to contain SMaps and not SLists*/
	SMap* prosSymbols;
} SRuleBasedEndtoneUttProc;

/**
 * Typedef of the rulebasedfeatures utterance processor class. Does not add any
 * new methods, therefore exactly the same as #SUttProcessor.
 */
typedef SUttProcessorClass SRuleBasedEndtoneUttProcClass;

/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_rule_based_endtone_class_reg(s_erc *error);

S_LOCAL void _s_rule_based_endtone_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_UTTPROCESSOR_RULE_BASED_FEATURES_H__ */
