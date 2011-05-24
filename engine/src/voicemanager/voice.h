/************************************************************************************/
/* Copyright (c) 2008-2011 The Department of Arts and Culture,                      */
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
/* DATE    : 6 January 2009                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Definition of the voice object.                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_VOICE_H__
#define _SPCT_VOICE_H__


/**
 * @file voice.h
 * Definition of the voice object.
 */


/**
 * @ingroup SVoices
 * @defgroup SVoiceObject Voice
 * Voice objects.
 * @{
 * @todo document
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/utils/vernum.h"
#include "base/utils/types.h"
#include "base/errdbg/errdbg.h"
#include "base/objsystem/objsystem.h"
#include "hrg/hrg.h"
#include "containers/containers.h"


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
 * @name Utility Macros
 * @{
 */


/**
 * @hideinitializer
 * Return the given #SVoice child class object as a voice object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SVoice* type.
 *
 * @note This casting is not safety checked.
 */
#define S_VOICE(SELF)  ((SVoice *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SVoice,
 * see full description #S_VOICE_CALL for usage.
 *
 * @param SELF The given #SVoice*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_VOICE_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_VOICE_CALL(SELF, FUNC)				\
	((SVoiceClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SVoice
 * can be called.
 *
 * @param SELF The given #SVoice*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_VOICE_METH_VALID(SELF, FUNC)			\
	S_VOICE_CALL(SELF, FUNC) ? TRUE : FALSE


/**
 * @}
 */


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/**
 * The voice information structure.
 */
typedef struct
{
	/**
	 * The name of the voice.
	 */
	char       *name;

	/**
	 * A short description of the voice.
	 */
	char       *description;

	/**
	 * Voice gender.
	 */
	char       *gender;

	/**
	 * The voice language.
	 */
	char       *language;

	/**
	 * The voice language code, <tt>ISO 639-2</tt>.
	 * @note See http://en.wikipedia.org/wiki/List_of_ISO_639-2_codes
	 */
	char       *lang_code;

	/**
	 * Voice version.
	 */
	s_version   version;
} s_voice_info;


/************************************************************************************/
/*                                                                                  */
/* Voice data definition (opaque)                                                   */
/*                                                                                  */
/************************************************************************************/

/**
 * Type definition of the opaque voice data.
 */
typedef struct s_voice_data s_voice_data;


/************************************************************************************/
/*                                                                                  */
/* SVoice definition                                                                */
/*                                                                                  */
/************************************************************************************/

/* typedef in hrg/hrg_defs.h */
/**
 * The SVoice structure.
 * @extends SObject
 */
struct SVoice
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject       obj;

	/**
	 * @protected Voice information.
	 */
	s_voice_info *info;

	/**
	 * @protected Plugin container.
	 */
	SList        *plugins;

	/**
	 * @protected Opaque data container.
	 */
	s_voice_data *data;

	/**
	 * @protected Features.
	 */
	SMap         *features;

	/**
	 * @protected Feature processors.
	 */
	SMap         *featProcessors;

	/**
	 * @protected Utterance processors.
	 */
	SMap         *uttProcessors;

	/**
	 * @protected Utterance types.
	 */
	SMap         *uttTypes;

	/**
	 * @protected Locking mutex.
	 */
	S_DECLARE_MUTEX(voice_mutex);
};


/************************************************************************************/
/*                                                                                  */
/* SVoiceClass definition                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * The SVoiceClass structure.
 * @extends SObjectClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SObjectClass.
	 */
	SObjectClass _inherit;

	/* Class methods */
	/**
	 * @protected SynthUtt function pointer.
	 * Synthesize an utterance with the given utterance type and
	 * input.
	 *
	 * @param self The voice used for synthesis.
	 * @param utt_type The key of the utterance type as registered in the #SVoice::uttTypes
	 * container.
	 * @param input The input to the synthesizer.
	 * @param error Error code.
	 *
	 * @return The synthesized utterance.
	 *
	 * @note The voice takes hold of the @c input #SObject
	 */
	SUtterance  *(*synth_utt)(const SVoice *self, const char *utt_type,
							  SObject *input, s_erc *error);

	/**
	 * @protected ReSynthUtt function pointer.
	 * Re-synthesize an utterance with the given utterance type. This
	 * is used when an utterance was synthesized with a certain
	 * utterance type, and now it must be synthesized with a different
	 * utterance type.
	 *
	 * @param self The voice used for synthesis.
	 * @param utt_type The key of the utterance type as registered in the #SVoice::uttTypes
	 * container.
	 * @param utt The utterance to re-synthesize.
	 * @param error Error code.
	 */
	void         (*re_synth_utt)(const SVoice *self, const char *utt_type,
								 SUtterance *utt, s_erc *error);
} SVoiceClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * @name Synthesis
 * @{
 */


/**
 * Synthesize an utterance with the given utterance type and input.
 *
 * @public @memberof SVoice
 * @param self The voice used for synthesis.
 * @param utt_type The key of the utterance type as registered in the
 * #SVoice::uttTypes container.
 * @param input The input to the synthesizer.
 * @param error Error code.
 *
 * @return The synthesized utterance.
 *
 * @note The voice takes hold of the @c input #SObject
 */
S_API SUtterance *SVoiceSynthUtt(const SVoice *self, const char *utt_type,
								 SObject *input, s_erc *error);


/**
 * Re-synthesize an utterance with the given utterance type. This
 * is used when an utterance was synthesized with a certain
 * utterance type, and now it must be synthesized with a different
 * utterance type.
 *
 * @public @memberof SVoice
 * @param self The voice used for synthesis.
 * @param utt_type The key of the utterance type as registered in the
 * #SVoice::uttTypes container.
 * @param utt The utterance to re-synthesize.
 * @param error Error code.
 */
S_API void SVoiceReSynthUtt(const SVoice *self, const char *utt_type,
							SUtterance *utt, s_erc *error);


/**
 * @}
 */


/**
 * @name Info
 * @{
 */


/**
 * Get the given voice's name.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param error Error code.
 *
 * @return Pointer to voice's name.
 */
S_API const char *SVoiceGetName(const SVoice *self, s_erc *error);


/**
 * Get the given voice's description.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param error Error code.
 *
 * @return Pointer to voice's description.
 */
S_API const char *SVoiceGetDescription(const SVoice *self, s_erc *error);


/**
 * Get the given voice's gender.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param error Error code.
 *
 * @return Pointer to voice's gender.
 */
S_API const char *SVoiceGetGender(const SVoice *self, s_erc *error);


/**
 * Get the given voice's language.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param error Error code.
 *
 * @return Pointer to voice's language.
 */
S_API const char *SVoiceGetLanguage(const SVoice *self, s_erc *error);


/**
 * Get the given voice's  <tt>ISO 639-2</tt> language code.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param error Error code.
 *
 * @return Pointer to voice's language code.
 *
 * @note See http://en.wikipedia.org/wiki/List_of_ISO_639-2_codes for the
 * language codes.
 */
S_API const char *SVoiceGetLangCode(const SVoice *self, s_erc *error);


/**
 * Get the given voice's version.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param error Error code.
 *
 * @return Pointer to voice's version.
 */
S_API const s_version *SVoiceGetVersion(const SVoice *self, s_erc *error);


/**
 * @}
 */


/**
 * @name Data
 * @{
 */


/**
 * Get a list of the @a data keys in the voice.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param error Error code.
 *
 * @return #SList of keys, or @a NULL if the data container is empty.
 *
 * The list objects (keys) are string objects and are accessed with #SObjectGetString.
 * @note The caller is responsible for the memory of the returned #SList object.
 */
S_API SList *SVoiceGetDataKeys(const SVoice *self, s_erc *error);


/**
 * Query if named @a data object is present in the given voice.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param name The data object name.
 * @param error Error code.
 *
 * @return #TRUE if present or #FALSE if not.
 */
S_API s_bool SVoiceDataIsPresent(const SVoice *self, const char *name,
								 s_erc *error);


/**
 * Get the voice @a data object of the named key.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param key The string key of the data object to get.
 * @param error Error code.
 *
 * @return Pointer to the data object of the named key, or @a NULL if
 * data of named key is not present in voice.
 */
S_API const SObject *SVoiceGetData(const SVoice *self, const char *key,
								   s_erc *error);


/**
 * Set the value of the named voice @a data key to the
 * given #SObject. If the named key already exists
 * then it's #SObject will be deleted (if not referenced) and
 * replaced.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param key The string key of the data object to set.
 * @param object Pointer to the data object of the named key.
 * @param error Error code.
 *
 * @note The voice takes hold of the @c object #SObject.
 */
S_API void SVoiceSetData(SVoice *self, const char *key,
						 SObject *object,  s_erc *error);


/**
 * Delete the value of the named key from the voice @a data container.
 * The key is removed and value deleted if it is not referenced.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param key The string key of the data object to delete.
 * @param error Error code.
 */
S_API void SVoiceDelData(SVoice *self, const char *key, s_erc *error);


/**
 * @}
 */


/**
 * @name Features
 * @{
 */


/**
 * Get a list of the @a feature keys in the voice.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param error Error code.
 *
 * @return #SList of keys, or @a NULL if the @a feature container is empty.
 * The list objects (keys) are string objects and are accessed with
 * #SObjectGetString.
 *
 * @note The caller is responsible for the memory of the returned #SList object.
 */
S_API SList *SVoiceGetFeatureKeys(const SVoice *self, s_erc *error);


/**
 * Query if named @a feature object is present in the given voice.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param name The feature object name.
 * @param error Error code.
 *
 * @return #TRUE if present or #FALSE if not.
 */
S_API s_bool SVoiceFeatureIsPresent(const SVoice *self, const char *name,
									s_erc *error);


/**
 * Get the voice @a feature object of the named key.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param key The string key of the feature object to get.
 * @param error Error code.
 *
 * @return Pointer to the feature object of the named key, or @a NULL if
 * feature of named key is not present in voice.
 */
S_API const SObject *SVoiceGetFeature(const SVoice *self, const char *key, s_erc *error);


/**
 * Set the value of the named voice @a feature key to the
 * given #SObject. If the named key already exists
 * then it's #SObject will be deleted (if not referenced) and
 * replaced.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param key The string key of the feature object to set.
 * @param object Pointer to the feature object of the named key.
 * @param error Error code.
 *
 * @note The voice takes hold of the @c object #SObject.
 */
S_API void SVoiceSetFeature(SVoice *self, const char *key,
							SObject *object,  s_erc *error);


/**
 * Set the value of the named voice @a feature key to the
 * given #SObject. If the named key already exists
 * then it's #SObject will be deleted (if not referenced) and
 * replaced. This is almost the same as #SVoiceSetFeature except that
 * no mutex locking is performed. This function *must* not be used
 * outside of the VoiceManager.
 *
 * @private @memberof SVoice
 * @param self The given voice.
 * @param key The string key of the feature object to set.
 * @param object Pointer to the feature object of the named key.
 * @param error Error code.
 *
 * @note The voice takes hold of the @c object #SObject.
 * @note Not thread safe, VoiceManager does mutex locking.
 */
S_LOCAL void _SVoiceSetFeature_no_lock(SVoice *self, const char *key,
									   SObject *object,  s_erc *error);


/**
 * Delete the value of the named key from the voice @a feature container.
 * The key is removed and value deleted if it is not referenced.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param key The string key of the feature object to delete.
 * @param error Error code.
 */
S_API void SVoiceDelFeature(SVoice *self, const char *key, s_erc *error);


/**
 * @}
 */


/**
 * @name Feature Processors
 * @{
 */


/**
 * Get a list of the <i> feature processor </i> keys in the voice.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param error Error code.
 *
 * @return #SList of keys, or @a NULL if the <i> feature processor </i> container is empty.
 * The list objects (keys) are string objects and are accessed with
 * #SObjectGetString.
 *
 * @note The caller is responsible for the memory of the returned #SList object.
 */
S_API SList *SVoiceGetFeatProcKeys(const SVoice *self, s_erc *error);


/**
 * Query if named <i> feature processor </i> is present in the given
 * voice.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param name The feature processor name.
 * @param error Error code.
 *
 * @return #TRUE if present or #FALSE if not.
 */
S_API s_bool SVoiceFeatProcIsPresent(const SVoice *self, const char *name,
									 s_erc *error);


/**
 * Get the voice <i> feature processor </i> object of the named key.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param key The string key of the feature processor object to get.
 * @param error Error code.
 *
 * @return Pointer to the feature processor object of the named key, or @a NULL if
 * feature processor of named key is not present in voice.
 */
S_API const SFeatProcessor *SVoiceGetFeatProc(const SVoice *self, const char *key,
											  s_erc *error);


/**
 * Set the value of the named voice <i> feature processor </i> key to the
 * given #SFeatProcessor. If the named key already exists
 * then it's #SFeatProcessor will be deleted (if not referenced) and
 * replaced.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param key The string key of the feature processor object to set.
 * @param featProc Pointer to the feature processor of the named key.
 * @param error Error code.
 *
 * @note The voice takes hold of the @c featProc #SFeatProcessor.
 */
S_API void SVoiceSetFeatProc(SVoice *self, const char *key,
							 SFeatProcessor *featProc,  s_erc *error);


/**
 * Delete the value of the named key from the voice <i> feature processor </i> container.
 * The key is removed and value deleted if it is not referenced.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param key The string key of the feature processor object to delete.
 * @param error Error code.
 */
S_API void SVoiceDelFeatProc(SVoice *self, const char *key, s_erc *error);


/**
 * @}
 */


/**
 * @name Utterance Processors
 * @{
 */


/**
 * Get a list of the <i> utterance processor </i> keys in the voice.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param error Error code.
 *
 * @return #SList of keys, or @a NULL if the <i> utterance processor </i> container is empty.
 * The list objects (keys) are string objects and are accessed with
 * #SObjectGetString.
 *
 * @note The caller is responsible for the memory of the returned #SList object.
 */
S_API SList *SVoiceGetUttProcKeys(const SVoice *self, s_erc *error);


/**
 * Query if named <i> utterance processor </i> is present in the given
 * voice.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param name The utterance processor name.
 * @param error Error code.
 *
 * @return #TRUE if present or #FALSE if not.
 */
S_API s_bool SVoiceUttProcIsPresent(const SVoice *self, const char *name,
									s_erc *error);


/**
 * Get the voice <i> utterance processor </i> object of the named key.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param key The string key of the utterance processor object to get.
 * @param error Error code.
 *
 * @return Pointer to the utterance processor object of the named key, or @a NULL if
 * utterance processor of named key is not present in voice.
 */
S_API const SUttProcessor *SVoiceGetUttProc(const SVoice *self, const char *key,
											s_erc *error);


/**
 * Set the value of the named voice <i> utterance processor </i> key to the
 * given #SUttProcessor. If the named key already exists
 * then it's #SUttProcessor will be deleted (if not referenced) and
 * replaced.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param key The string key of the utterance processor object to set.
 * @param uttProc Pointer to the utterance processor of the named key.
 * @param error Error code.
 *
 * @note The voice takes hold of the @c uttProc #SUttProcessor.
 */
S_API void SVoiceSetUttProc(SVoice *self, const char *key,
							SUttProcessor *uttProc,  s_erc *error);


/**
 * Delete the value of the named key from the voice <i> utterance processor </i> container.
 * The key is removed and value deleted if it is not referenced.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param key The string key of the utterance processor object to delete.
 * @param error Error code.
 */
S_API void SVoiceDelUttProc(SVoice *self, const char *key, s_erc *error);


/**
 * @}
 */


/**
 * @name Utterance Types
 * @{
 */


/**
 * Get a list of the voice <i> utterance type </i> keys in the voice.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param error Error code.
 *
 * @return #SList of keys, or @a NULL if the <i> utterance type </i> container is empty.
 * The list objects (keys) are string objects and are accessed with
 * #SObjectGetString.
 *
 * @note The caller is responsible for the memory of the returned #SList object.
 */
S_API SList *SVoiceGetUttTypesKeys(const SVoice *self, s_erc *error);


/**
 * Query if named <i> utterance type </i> is present in the given
 * voice.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param name The utterance type name.
 * @param error Error code.
 *
 * @return #TRUE if present or #FALSE if not.
 */
S_API s_bool SVoiceUttTypeIsPresent(const SVoice *self, const char *name,
									s_erc *error);


/**
 * Get the voice <i> utterance type </i> object of the named key.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param key The string key of the utterance type to get.
 * @param error Error code.
 *
 * @return #SList of string objects (access with #SObjectGetString)
 * of the <i> utterance processors </i> defined for the named
 * utterance type, or @a NULL if utterance type of named key is
 * not present in voice.
 */
S_API const SList *SVoiceGetUttType(const SVoice *self, const char *key, s_erc *error);


/**
 * Set the value of the named voice <i> utterance type </i> key to the
 * given #SList of string objects of the <i> utterance processors </i>
 * defined for the utterance type.
 * If the named key already exists then it's #SList will be deleted
 * (if not referenced) and replaced.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param key The string key of the utterance type object to set.
 * @param uttType Pointer to the #SList of utterance processors for
 * the named utterance type
 * @param error Error code.
 *
 * @note The voice takes hold of the @c uttType #SList.
 */
S_API void SVoiceSetUttType(SVoice *self, const char *key,
							SList *uttType,  s_erc *error);


/**
 * Delete the value of the named key from the voice <i> utterance type </i> container.
 * The key is removed and value deleted if it is not referenced.
 *
 * @public @memberof SVoice
 * @param self The given voice.
 * @param key The string key of the utterance type object to delete.
 * @param error Error code.
 */
S_API void SVoiceDelUttType(SVoice *self, const char *key, s_erc *error);


/**
 * @}
 */


/**
 * Load the voice data in the voice data configuration. This function is used
 * after a voice is loaded with #s_vm_load_voice with the @c load_data
 * flag set to @c FALSE. This allows one to change the voice data
 * configuration before loading the voice data, thereby eliminating
 * lengthy load times for data that is not required for specific
 * utterance types. If data has already been loaded then this function
 * will issue a warning.
 *
 * @param self The given voice.
 * @param dataConfig A map with the data configuration.
 * @param error Error code.
 */
S_LOCAL void _s_voice_load_data(SVoice *self, const SMap *dataConfig, s_erc *error);


/**
 * Add the SVoice class to the object system.
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_voice_class_add(s_erc *error);


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

#endif /* _SPCT_VOICE_H__ */

