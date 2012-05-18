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
/* An utterance processor to do HTS Engine (mixed excitation) synthesis of a        */
/* segment relation stream.                                                         */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_UTTPROCESSOR_SYNTH_HTS_ENGINE_ME_105_H__
#define _SPCT_PLUGIN_UTTPROCESSOR_SYNTH_HTS_ENGINE_ME_105_H__


/**
 * @file synthesize_hts_engine.h
 * An utterance processor to do HTS Engine (mixed excitation)
 * synthesis of a segment relation stream.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "speect.h"
#include "HTS_engine.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* SHTSEngineMESynthUttProc definition                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * The SHTSEngineMESynthUttProc105 structure.
 * An utterance processor to do HTS Engine (mixed excitation)
 * synthesis of a segment relation stream.
 * @extends SUttProcessor
 */
typedef struct
{
	/**
	 * @protected Inherit from #SUttProcessor.
	 */
	SUttProcessor obj;

	/**
	 * @protected The HTS Engine.
	 */
	HTS_Engine    engine;

	/**
	 * @protected #TRUE if this is a mixed excitation voice.
	 */
	s_bool        me;

	/**
	 * @protected number of filters
	 */
	int      num_filters;

	/**
	 * @protected filter order
	 */
	int      filter_order;

	/**
	 * @protected filter coefficients
	 */
	double **h;

	/**
	 * @protected pulse signal, the size of this should be the filter order
	 */
	double  *xp_sig;

	/**
	 * @protected noise signal, the size of this should be the filter order
	 */
	double  *xn_sig;

	/**
	 * @protected pulse shaping filter, size of the filter order
	 */
	double  *hp;

	/**
	 * @protected noise shaping filter, size of the filter order
	 */
    double  *hn;
} SHTSEngineMESynthUttProc105;


/************************************************************************************/
/*                                                                                  */
/* SHTSEngineMESynthUttProc105Class definition                                      */
/*                                                                                  */
/************************************************************************************/

/**
 * Typedef of the hts engine synthesizer utterance processor class. Does not add any
 * new methods, therefore exactly the same as #SUttProcessor.
 */
typedef SUttProcessorClass SHTSEngineMESynthUttProc105Class;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SHTSEngineMESynthUttProc105 plug-in class with the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_hts_engine_me_synth_utt_proc_105_class_reg(s_erc *error);


/**
 * Free the #SHTSEngineMESynthUttProc105 plug-in class from the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_hts_engine_me_synth_utt_proc_105_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_UTTPROCESSOR_SYNTH_HTS_ENGINE_ME_105_H__ */

