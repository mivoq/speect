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
/* Generate data for HTS Labels for italian voices.                                 */
/* The SObject returned will contain the feature with these keys:                   */
/*                                                                                  */
/*   phoneme context:                                                               */
/*      - 'p.p.phone' : string : previous previous phoneme                          */
/*      - 'p.phone'   : string : previous phoneme                                   */
/*      - 'phone'     : string : current phoneme                                    */
/*      - 'n.phone'   : string : next phoneme                                       */
/*      - 'n.n.phone' : string : next next phoneme                                  */
/*                                                                                  */
/*   syllable context:                                                              */
/*      - 'phones.from.syl.start' : int : number of phonemes before this in syl     */
/*      - 'phones.from.syl.end'   : int : number of phonemes after this in syllable */
/*      - 'p.syl.phones.num'      : int : number of phonemes in the prev syllable   */
/*      - 'syl.phones.num'        : int : number of phonemes in this syllable       */
/*      - 'n.syl.phones.num'      : int : number of phonemes in the next syllable   */
/*      - 'vowel.name'            : string : name of the vowel in the syllable      */
/*                                                                                  */
/*   word context:                                                                  */
/*      - 'syls.from.word.start'  : int : number of syls before this in word        */
/*      - 'syls.from.word.end'    : int : number of syls after this in word         */
/*      - 'p.word.syls.num'       : int : number of syls in the previous word       */
/*      - 'word.syls.num'         : int : number of syls in this word               */
/*      - 'n.word.syls.num'       : int : number of syls in the next word           */
/*      - 'p.word.gpos'           : string : guess-part-of-speech of previous word  */
/*      - 'word.gpos'             : string : guess-part-of-speech of this word      */
/*      - 'n.word.gpos'           : string : guess-part-of-speech of next word      */
/*                                                                                  */
/*   phrase context:                                                                */
/*      - 'syls.from.phrase.start': int : number of syls before this in phrase      */
/*      - 'syls.from.phrase.end'  : int : number of syls after this in phrase       */
/*      - 'p.phrase.syls.num'     : int : number of syls in the previous phrase     */
/*      - 'phrase.syls.num'       : int : number of syls in this phrase             */
/*      - 'n.phrase.syls.num'     : int : number of syls in the next phrase         */
/*      - 'p.phrase.words.num'    : int : number of words in the previous phrase    */
/*      - 'phrase.words.num'      : int : number of words in this phrase            */
/*      - 'n.phrase.words.num'    : int : number of words in the next phrase        */
/*      - 'words.from.phrase.start': int : number of words before this in phrase    */
/*      - 'words.from.phrase.end' : int : number of words after this in phrase      */
/*      - 'content.words.before'  : int : number of content words before in phrase  */
/*      - 'content.words.after'   : int : number of content words after in phrase   */
/*      - 'words.from.prev.cont'  : int : numb of words from the prev. content word */
/*      - 'words.to.next.cont'    : int : numb of words to the next content word    */
/*      - 'tobi.endtone'          : string : TOBI endtone of the current phrase     */
/*                                                                                  */
/*   utterance context:                                                             */
/*      - 'phrases.from.utt.start': int : number of phrases before this in utterance*/
/*      - 'phrases.from.utt.end'  : int : number of phrases after this in utterance */
/*      - 'utterance.syls.num'    : int : number of syls in the utterance           */
/*      - 'utterance.words.num'   : int : number of words in this utterance         */
/*      - 'utterance.phrases.num' : int : number of phrases in this utterance       */
/*                                                                                  */
/*   stress context:                                                                */
/*      - 'p.stress'              : int : previous syllable stress level            */
/*      - 'stress'                : int : this syllable stress level                */
/*      - 'n.stress'              : int : next syllable stress level                */
/*      - 'stressed.syls.before'  : int : num of stressed syls before in the phrase */
/*      - 'stressed.syls.after'   : int : num of stressed syls after in the phrase  */
/*      - 'syls.from.prev.stress' : int : num of syls from the previous stressed one*/
/*      - 'syls.to.next.stress'   : int : num of syls to the next stressed one      */
/*                                                                                  */
/*   Georg's accent context:                                                        */
/*      - 'p.accent'              : int : previous syllable accent level            */
/*      - 'accent'                : int : this syllable accent level                */
/*      - 'n.accent'              : int : next syllable accent level                */
/*      - 'accented.syls.before'  : int : num of accented syls before in the phrase */
/*      - 'accented.syls.after'   : int : num of accented syls after in the phrase  */
/*      - 'syls.from.prev.accent' : int : num of syls from the previous accented one*/
/*      - 'syls.to.next.accent'   : int : num of syls to the next accented one      */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_FEATPROCESSOR_HTS_DATA_COLLECTOR_H__
#define _SPCT_PLUGIN_FEATPROCESSOR_HTS_DATA_COLLECTOR_H__


/**
 * @file hts_data_collector.h
 * Generate data for HTS Labels for italian voices.
 */


/**
 * @ingroup SFeatureProc
 * @defgroup SHTSLabelsConfigurableFeatProc Generate data for HTS Labels for italian voices.
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
 * The HTS label data generator feature processor structure. Does not add any
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
	 * @protected SHTSLabelsDataCollectorFeatProc features.
	 */
	SMap    *features;

} SHTSLabelsDataCollectorFeatProc;


/************************************************************************************/
/*                                                                                  */
/* SHTSLabelsSimple1FeatProcClass definition                                        */
/*                                                                                  */
/************************************************************************************/

/**
 * Typedef of the HTS label data generator feature processor
 * class. Does not add any new methods, therefore exactly the same as
 * #SFeatProcessorClass.
 */
typedef SFeatProcessorClass SHTSLabelsDataCollectorFeatProcClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/


/**
 * Query if named feature is present in the given Label Data Collector.
 * @public @memberof SHTSLabelsDataCollectorFeatProc
 * @param self The given Label Data Collector.
 * @param name The feature name.
 * @param error Error code.
 * @return #TRUE if present or #FALSE if not.
 */
S_API s_bool SHTSLabelDataCollectorFeatureIsPresent(const SHTSLabelsDataCollectorFeatProc *self,
					   const char *name,
					   s_erc *error);


/**
 * Get the feature object of the named key from the given Label Data Collector.
 * @public @memberof SUttProcessor
 * @param self The given Label Data Collector.
 * @param key The string key of the feature processor object to get.
 * @param error Error code.
 * @return Pointer to #SObject object feature value.
 */
S_API const SObject *SHTSLabelDataCollectorGetFeature(const SHTSLabelsDataCollectorFeatProc *self, const char *name,
					     s_erc *error);


/**
 * Set the feature of the named key to the given Label Data Collector.
 * If the named key already exists then it's #SObject will be deleted
 * (if not referenced) and replaced.
 * @public @memberof SUttProcessor
 * @param self The given Label Data Collector.
 * @param key The string key of the feature object to set.
 * @param object The feature object (value).
 * @param error Error code.
 */
S_API void SHTSLabelDataCollectorSetFeature(SHTSLabelsDataCollectorFeatProc *self, const char *name,
				   const SObject *object, s_erc *error);


/**
 * Delete the feature of the named key of the the given Label Data Collector.
 * The key is removed and the feature object deleted if it is not referenced.
 * @public @memberof SUttProcessor
 * @param self The given Label Data Collector.
 * @param name The feature name (key).
 * @param error Error code.
 */
S_API void SHTSLabelDataCollectorDelFeature(SHTSLabelsDataCollectorFeatProc *self, const char *name,
				   s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SHTSLabelsDataCollectorFeatProc plug-in class with the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_hts_labels_data_collector_class_reg(s_erc *error);


/**
 * Free the #SHTSLabelsDataCollectorFeatProc plug-in class from the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_hts_labels_data_collector_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_FEATPROCESSOR_HTS_DATA_COLLECTOR_H__ */
