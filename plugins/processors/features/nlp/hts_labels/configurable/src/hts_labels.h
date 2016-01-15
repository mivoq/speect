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
/* AUTHOR  : Simone Daminato                                                        */
/* DATE    : December 2015                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Generate HTS Labels for italian voices (configurable).                           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_FEATPROCESSOR_HTS_LABELS_CONFIGURABLE_H__
#define _SPCT_PLUGIN_FEATPROCESSOR_HTS_LABELS_CONFIGURABLE_H__


/**
 * @file hts_labels.h
 * Generate HTS Labels for italian voices (configurable).
 */


/**
 * @ingroup SFeatureProc
 * @defgroup SHTSLabelsConfigurableFeatProc Generate HTS Labels for italian voices (configurable).
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
/* SHTSLabelsConfigurableFeatProc definition                                        */
/*                                                                                  */
/************************************************************************************/

/**
 * The HTS label generator (configurable) feature processor structure. Does not add any
 * new members, therefore exactly the same as #SFeatProcessor.
 * Typedef of the HTS label generator (cconfigurable) feature processor.
 * @extends SFeatProcessor
 */
typedef struct
{
	/**
	 * @protected Inherit from #SFeatProcessor.
	 */
	SFeatProcessor 	obj;
	
	/**
	 * @protected If we should create A context.
	 */
	s_bool		enable_A_context;
	
	/**
	 * @protected If we should create B context.
	 */
	s_bool		enable_B_context;
	
	/**
	 * @protected If we should create C context.
	 */
	s_bool		enable_C_context;
	
	/**
	 * @protected If we should create D context.
	 */
	s_bool		enable_D_context;
	
	/**
	 * @protected If we should create E context.
	 */
	s_bool		enable_E_context;
	
	/**
	 * @protected If we should create F context.
	 */
	s_bool		enable_F_context;
	
	/**
	 * @protected If we should create G context.
	 */
	s_bool		enable_G_context;
	
	/**
	 * @protected If we should create H context.
	 */
	s_bool		enable_H_context;
	
	/**
	 * @protected If we should create I context.
	 */
	s_bool		enable_I_context;
	
	/**
	 * @protected If we should check if a syllabe is stressed or not.
	 */
	s_bool		enable_stress;
	
	/**
	 * @protected If we should check if a syllabe is accented or not.
	 */
	s_bool		enable_accent;
	
	/**
	 * @protected If we should calculate the guess part of speech.
	 */
	s_bool		enable_gpos;
	
	/**
	 * @protected If we should calculate the number of content words in/all
	 */
	s_bool		enable_word_content;
	
	/**
	 * @protected If we should calculate TOBI endtone
	 */
	s_bool		enable_tobi;
	
	
} SHTSLabelsConfigurableFeatProc;


/************************************************************************************/
/*                                                                                  */
/* SHTSLabelsSimple1FeatProcClass definition                                        */
/*                                                                                  */
/************************************************************************************/

/**
 * Typedef of the HTS label generator (configurable) feature processor
 * class. Does not add any new methods, therefore exactly the same as
 * #SFeatProcessorClass.
 */
typedef SFeatProcessorClass SHTSLabelsConfigurableFeatProcClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SHTSLabelsConfigurableFeatProc plug-in class with the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_hts_labels_configurable_class_reg(s_erc *error);


/**
 * Free the #SHTSLabelsConfigurableFeatProc plug-in class from the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_hts_labels_configurable_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_FEATPROCESSOR_HTS_LABELS_CONFIGURABLE_H__ */
