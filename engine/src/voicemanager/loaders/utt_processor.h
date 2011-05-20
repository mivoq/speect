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
/* DATE    : 29 October 2009                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A loader for the voice utterance processors in the voice configuration file.     */
/* This is (currently) an exact copy of the feature processors loader in            */
/* feat_processor.c, with just the names that are changed.                          */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_LOADER_VOICE_UTTERANCE_PROC_H__
#define _SPCT_LOADER_VOICE_UTTERANCE_PROC_H__


/**
 * @file loaders/utt_processor.h
 * A loader for the voice utterance processors in the voice configuration file.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "containers/containers.h"
#include "base/errdbg/errdbg.h"
#include "voicemanager/voice.h"


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
 * Parse the voice utterance processors section in the voice
 * configuration file, create the utterance processor objects,
 * and return them in a map.
 *
 * @private
 * @param voiceConfig The #SMap containing the voice configuration.
 * @param voice The voice that the utterance processors belong to.
 * @param error Error code.
 *
 * @return A #SMap containing the voice utterance processors.
 */
S_LOCAL void _s_load_voice_utterance_processors(const SMap *voiceConfig,
												SVoice *voice,
												s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* End external c declaration                                                       */
/*                                                                                  */
/************************************************************************************/
S_END_C_DECLS


#endif /* _SPCT_LOADER_VOICE_UTTERANCE_PROC_H__ */

