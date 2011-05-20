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
/* DATE    : 3 November 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* The Utterance Processor class implementation. An Utterance Processor processes   */
/* an utterance by extracting information from it and then modifying it in some     */
/* way.                                                                             */
/*                                                                                  */
/* Loosely based on UtteranceProcessor of FreeTTS,                                  */
/* http://sourceforge.net/projects/freetts (1.2).                                   */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_UTTPROCESSOR_H__
#define _SPCT_UTTPROCESSOR_H__


/************************************************************************************/
/*                                                                                  */
/* FreeTTS license, UtteranceProcessor                                              */
/*                                                                                  */
/*      Portions Copyright 2001-2004 Sun Microsystems, Inc.                         */
/*      Portions Copyright 1999-2001 Language Technologies Institute,               */
/*      Carnegie Mellon University.                                                 */
/*      All Rights Reserved.  Use is subject to license terms.                      */
/*                                                                                  */
/*      Permission is hereby granted, free of charge, to use and distribute         */
/*      this software and its documentation without restriction, including          */
/*      without limitation the rights to use, copy, modify, merge, publish,         */
/*      distribute, sublicense, and/or sell copies of this work, and to             */
/*      permit persons to whom this work is furnished to do so, subject to          */
/*      the following conditions:                                                   */
/*                                                                                  */
/*       1. The code must retain the above copyright notice, this list of           */
/*          conditions and the following disclaimer.                                */
/*       2. Any modifications must be clearly marked as such.                       */
/*       3. Original authors' names are not deleted.                                */
/*       4. The authors' names are not used to endorse or promote products          */
/*          derived from this software without specific prior written               */
/*          permission.                                                             */
/*                                                                                  */
/*      SUN MICROSYSTEMS, INC., CARNEGIE MELLON UNIVERSITY AND THE                  */
/*      CONTRIBUTORS TO THIS WORK DISCLAIM ALL WARRANTIES WITH REGARD TO THIS       */
/*      SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND           */
/*      FITNESS, IN NO EVENT SHALL SUN MICROSYSTEMS, INC., CARNEGIE MELLON          */
/*      UNIVERSITY NOR THE CONTRIBUTORS BE LIABLE FOR ANY SPECIAL, INDIRECT OR      */
/*      CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF      */
/*      USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR       */
/*      OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR      */
/*      PERFORMANCE OF THIS SOFTWARE.                                               */
/*                                                                                  */
/*      Authors: Willie Walker, Paul Lamere, Philip Kwok                            */
/*                                                                                  */
/************************************************************************************/


/**
 * @file uttprocessor.h
 * UttProcessor class implementation.
 */


/**
 * @ingroup SHrgProcessors
 * @defgroup SUttProc Utterance Processor
 * The <i> Utterance Processor </i> class implementation. An Utterance Processor processes an
 * utterance by extracting information from it and then modifying it in some way.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "hrg/utterance.h"
#include "containers/containers.h"
#include "base/errdbg/errdbg.h"


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
 * Return the given #SUttProcessor child class object as a #SUttProcessor object.
 * @param SELF The given object.
 * @return Given object as #SUttProcessor* type.
 * @note This casting is not safety checked.
 */
#define S_UTTPROCESSOR(SELF)    ((SUttProcessor *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SUttProcessor,
 * see full description #S_UTTPROCESSOR_CALL for usage.
 * @param SELF The given #SUttProcessor*.
 * @param FUNC The function method of the given object to call.
 * @note This casting is not safety checked.
 * @note Example usage: @code S_UTTPROCESSOR_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_UTTPROCESSOR_CALL(SELF, FUNC)					\
	((SUttProcessorClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SUttProcessor
 * can be called.
 * @param SELF The given #SUttProcessor*.
 * @param FUNC The function method of the given object to check.
 * @return #TRUE if function can be called, otherwise #FALSE.
 * @note This casting is not safety checked.
 */
#define S_UTTPROCESSOR_METH_VALID(SELF, FUNC)			\
	S_UTTPROCESSOR_CALL(SELF, FUNC) ? TRUE : FALSE


/**
 * @}
 */


/************************************************************************************/
/*                                                                                  */
/* SUttProcessor definition                                                         */
/*                                                                                  */
/************************************************************************************/

/**
 * The SUttProcessor structure.
 * A processor for utterances.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject  obj;

	/**
	 * @protected UttProcessor features.
	 */
	SMap    *features;
} SUttProcessor;


/************************************************************************************/
/*                                                                                  */
/* SUttProcessorClass definition                                                    */
/*                                                                                  */
/************************************************************************************/

/**
 * The SUttProcessorClass structure.
 * @extends SObjectClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SObjectClass.
	 */
	SObjectClass  _inherit;

  	/* Class methods */
	/**
	 * @protected Initialize function pointer.
	 * Initialize the utterance processor. Used for initialization of
	 * the utterance processor feature classes on loading of the
	 * voices. May not be implemented, use #S_UTTPROCESSOR_METH_VALID
	 * to check.
	 *
	 * @param self The UttProcessor to initialize.
	 * @param voice The voice that the utterance processor belongs to.
	 * @param error Error code.
	 *
	 * @note If an error occurs the @c error variable must be set so
	 * that #SUttProcessorInit can delete the utterance processor.
	 */
	void (*initialize)(SUttProcessor *self, const SVoice *voice, s_erc *error);

 	/**
	 * @protected Run function pointer.
	 * Execute the UttProcessor on the given utterance.
	 *
	 * @param self The UttProcessor to execute.
	 * @param utt The utterance on which to execute the UttProcessor.
	 * @param error Error code.
	 */
	void (*run) (const SUttProcessor *self, SUtterance *utt, s_erc *error);
} SUttProcessorClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Initialize the utterance processor. Used for initialization of
 * the utterance processor feature classes on loading of the
 * voices.
 * @public @memberof SUttProcessor
 *
 * @param self The UttProcessor to initialize.
 * @param voice The voice that the utterance processor belongs to.
 * @param error Error code.
 *
 * @note If this function failed the utterance processor is
 * deleted and the @c self variable is set to @c NULL.
 */
S_API void SUttProcessorInit(SUttProcessor **self, const SVoice *voice, s_erc *error);


/**
 * Execute the UttProcessor on the given utterance.
 * @public @memberof SUttProcessor
 * @param self The UttProcessor to execute.
 * @param utt The utterance on which to execute the UttProcessor.
 * @param error Error code.
 * @return Pointer to @a utt.
 */
S_API void SUttProcessorRun(const SUttProcessor *self, SUtterance *utt, s_erc *error);


/**
 * Query if named feature is present in the given UttProcessor.
 * @public @memberof SUttProcessor
 * @param self The given UttProcessor.
 * @param name The feature name.
 * @param error Error code.
 * @return #TRUE if present or #FALSE if not.
 */
S_API s_bool SUttProcessorFeatureIsPresent(const SUttProcessor *self,
										   const char *name,
										   s_erc *error);


/**
 * Get the feature object of the named key from the given UttProcessor.
 * @public @memberof SUttProcessor
 * @param self The given UttProcessor.
 * @param key The string key of the feature processor object to get.
 * @param error Error code.
 * @return Pointer to #SObject object feature value.
 */
S_API const SObject *SUttProcessorGetFeature(const SUttProcessor *self, const char *key,
											 s_erc *error);


/**
 * Set the feature of the named key to the given #SObject.
 * If the named key already exists then it's #SObject will be deleted
 * (if not referenced) and replaced.
 * @public @memberof SUttProcessor
 * @param self The given name.
 * @param key The string key of the feature object to set.
 * @param object The feature object (value).
 * @param error Error code.
 */
S_API void SUttProcessorSetFeature(SUttProcessor *self, const char *key,
								   const SObject *object, s_erc *error);


/**
 * Delete the feature of the named key of the the given UttProcessor.
 * The key is removed and the feature object deleted if it is not referenced.
 * @public @memberof SUttProcessor
 * @param self The given name.
 * @param name The feature name (key).
 * @param error Error code.
 */
S_API void SUttProcessorDelFeature(SUttProcessor *self, const char *name,
								   s_erc *error);


/**
 * Add the SUttProcessor class to the object system.
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_uttprocessor_class_add(s_erc *error);


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

#endif /* _SPCT_UTTPROCESSOR_H__ */
