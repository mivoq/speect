/************************************************************************************/
/* Copyright (c) Mivoq SRL                                                          */
/*                                                                                  */
/* Contributors:  Simone Daminato, Federico Rossetto, Giulio Paci                   */
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
/*      - 'word.phones.num'       : int : number of phones in this word             */
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
/*      - 'phrase.type'           : string : phrase's type (interrog, excl, decl)   */
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
 * @defgroup SHTSLabelsDataCollectorFeatProc Generate data for HTS Labels for italian voices.
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
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Return the given #SHTSLabelsDataCollectorFeatProc child/parent class object as a
 * #SHTSLabelsDataCollectorFeatProc object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SHTSLabelsDataCollectorFeatProc* type.
 * @note This casting is not safety checked.
 */
#define S_HTSLABELSDATACOLLECTOR(SELF)    ((SHTSLabelsDataCollectorFeatProc *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SHTSLabelsDataCollectorFeatProc,
 * see full description #S_SHTSLABELSDATACOLLECTOR_CALL for usage.
 *
 * @param SELF The given #SHTSLabelsDataCollectorFeatProc*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_SHTSLABELSDATACOLLECTOR_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters
 * passed to the object function @c func.
 */
#define S_HTSLABELSDATACOLLECTOR_CALL(SELF, FUNC)					\
	((SHTSLabelsDataCollectorFeatProcClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SHTSLabelsDataCollectorFeatProc
 * can be called.
 *
 * @param SELF The given #SHTSLabelsDataCollectorFeatProc*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_HTSLABELSDATACOLLECTOR_METH_VALID(SELF, FUNC)		\
	S_HTSLABELSDATACOLLECTOR_CALL(SELF, FUNC) ? TRUE : FALSE



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
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SFeatProcessorClass.
	 */
	SFeatProcessorClass _inherit;

	/* Class methods */
	/**
	 * Get the label data feature object of the named key.
	 *
	 * @param self The given SHTSLabelsDataCollectorFeatProc.
	 * @param key The string key of the feature object to get.
	 * @param error Error code.
	 *
	 * @return Pointer to the feature object of the named key, or @a NULL if
	 * feature of named key is not present in SHTSLabelsDataCollectorFeatProc.
	 */
	const SObject 	*(*get_feature) 	(const SHTSLabelsDataCollectorFeatProc *self, const char *key,
											s_erc *error);

	/**
	 * Query if the label data has the named feature.
	 *
	 * @param self The given SHTSLabelsDataCollectorFeatProc.
	 * @param key The string key of the feature for which the query is done.
	 * @param error Error code.
	 *
	 * @return @c TRUE if the feature is defined,
	 * else @c FALSE.
	 */
	s_bool 		(*has_feature) 	(const SHTSLabelsDataCollectorFeatProc *self, const char *key,
											s_erc *error);


} SHTSLabelsDataCollectorFeatProcClass;

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
