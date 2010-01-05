/************************************************************************************/
/* Copyright (c) 2008-2009 The Department of Arts and Culture,                      */
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
/* DATE    : 9 January 2009                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Definition of the voice manager.                                                 */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_VOICE_MANAGER_H__
#define _SPCT_VOICE_MANAGER_H__


/**
 * @file voicemanager/manager.h
 * Definition of the VoiceManager.
 */


/**
 * @ingroup SVoices
 * @defgroup SVoiceManager VoiceManager
 * The @c VoiceManager provides methods for loading voices that
 * are then available to the Speect Engine.
 * The loaded voice's data is chached for reuse.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "voicemanager/voice.h"
#include "base/errdbg/errdbg.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Load a voice from the given path. If any of the voice's data
 * objects have already been loaded, then they are shared. This
 * reduces the need for multiple redundant calls.
 *
 * @param path The full path and name of the voice to load.
 * @param load_data Flag to load the voice's data or not. If @c FALSE
 * then only the voice data configuration is loaded. This allows one
 * to change the data configuration of the voice with the
 * #SVoiceSetData, and #SVoiceDelData functions before the data is
 * loaded. The voice data can then be loaded with the #SVoiceLoadData
 * function.
 * @param error Error code.
 *
 * @return Pointer to the loaded voice.
 *
 * @note The returned voice can be normally deleted with #S_DELETE.
 * @note Thread safe.
 */
S_API SVoice *s_vm_load_voice(const char *path, s_bool load_data,
							  s_erc *error);


/**
 * Load the data object described by the given parameters. If
 * the data object has already been loaded, then it is shared. This
 * reduces the need for multiple redundant calls.
 * Used internally by #SVoice.
 *
 * @private
 * @param plugin_path The full path and name of the plug-in of this
 * data object.
 * @param data_path The full path and name of the data file of this
 * data object.
 * @param data_format The format of this data object.
 * @param error Error code.
 *
 * @return The loaded data object.
 */
S_LOCAL const SObject *_s_vm_load_data(const char *plugin_path,
									   const char *data_path,
									   const char *data_format,
									   s_erc *error);


/**
 * Unload the given voice data object. If the object is still
 * referenced by other voices then it is not unloaded.
 * Used internally by #SVoice.
 *
 * @private
 * @param dataObject The voice data object to unload.
 * @param error Error code.
 */
S_LOCAL void _s_vm_unload_data(SObject *dataObject, s_erc *error);


/**
 * Query if a data object has been loaded by the VoiceManager.
 * Used internally by #SVoice.
 *
 * @private
 * @param dataObject The voice data object to query for.
 * @param error Error code.
 *
 * @return #TRUE or #FALSE.
 */
S_LOCAL s_bool _s_vm_data_loaded(SObject *dataObject, s_erc *error);


/**
 * Initialize the VoiceManager. Sets up the voice data cache and
 * mutex's.
 *
 * @private
 * @param error Error code.
 *
 * @note Not thread safe.
 */
S_LOCAL void _s_vm_init(s_erc *error);


/**
 * Quit the VoiceManager. Clears the voice data cache and
 * mutex's.
 *
 * @private
 * @param error Error code.
 *
 * @note Not thread safe.
 */
S_LOCAL void _s_vm_quit(s_erc *error);


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

#endif /* _SPCT_VOICE_MANAGER_H__ */

