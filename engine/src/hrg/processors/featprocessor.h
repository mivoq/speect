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
/* The Feature Processor class implementation. A Feature Processor                  */
/* extracts features from an item.                                                  */
/*                                                                                  */
/* Loosely based on FeatureProcessor of FreeTTS,                                    */
/* http://sourceforge.net/projects/freetts (1.2).                                   */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_FEATPROCESSOR_H__
#define _SPCT_FEATPROCESSOR_H__


/************************************************************************************/
/*                                                                                  */
/* FreeTTS license, FeatureProcessor                                                */
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
 * @file featprocessor.h
 * FeatProcessor class implementation.
 */


/**
 * @ingroup SHrgProcessors
 * @defgroup SFeatureProc Feature Processor
 * The <i> Feature Processor </i> class implementation. A Feature Processor
 * extracts features from an item. The feature processor object and
 * class are basically just wrappers around the feature function
 * pointer and it's meta information.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/errdbg/errdbg.h"
#include "hrg/item.h"


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
 * Return the given #SFeatProcessor child class object as a #SFeatProcessor object.
 * @param SELF The given object.
 * @return Given object as #SFeatProcessor* type.
 * @note This casting is not safety checked.
 */
#define S_FEATPROCESSOR(SELF)    ((SFeatProcessor *)(SELF))


/**
 * @}
 */


/************************************************************************************/
/*                                                                                  */
/* SFeatProcessor definition                                                        */
/*                                                                                  */
/************************************************************************************/

/**
 * The SFeatProcessor structure. Does not add any members to #SObject,
 * and therefore exactly the same.
 */
typedef SObject SFeatProcessor;


/************************************************************************************/
/*                                                                                  */
/* SFeatProcessorClass definition                                                   */
/*                                                                                  */
/************************************************************************************/

/**
 * The SFeatProcessorClass structure.
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
	 * Initialize the feature processor. Used for initialization of
	 * the feature processor, if required, used for the load of
	 * extra parameters needed.
	 *
	 * @param self The FeatProcessor to initialize.
	 * @param voice The voice that the feature processor belongs to.
	 * @param map that holds the configuration params from the voice.json
	 * @param error Error code.
	 *
	 * @note Not necessarily implemented.
  	 */
  	void (* const initialize)(SFeatProcessor *self, SMap *feat, s_erc *error);

 	/**
	 * @protected Run function pointer.
	 * Execute the FeatProcessor on the given item.
	 *
	 * @param self The FeatProcessor to execute.
	 * @param item The item to extract features from.
	 * @param error Error code.
	 *
	 * @return An #SObject type feature.
	 *
	 * @note The caller is responsible for the memory of the returned #SObject.
	 */
	SObject *(* const run) (const SFeatProcessor *self, const SItem *item, s_erc *error);
} SFeatProcessorClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Initialize the feature processor. Used for initialization of
 * the feature processor features classes on loading of the
 * voices.
 * @public @memberof SFeatProcessor
 *
 * @param self The FeatProcessor to initialize.
 * @param voice The voice that the utterance processor belongs to.
 * @param feat The map holding the configurations to be loaded.
 * @param error Error code.
 *
 * @note If this function failed the utterance processor is
 * deleted and the @c self variable is set to @c NULL.
 */
S_API void SFeatProcessorInit(SFeatProcessor **self, SMap *feat, s_erc *error);

/**
 * Execute the FeatProcessor on the given item.
 *
 * @public
 * @param self The FeatProcessor to execute.
 * @param item The item to extract features from.
 * @param error Error code.
 *
 * @return An #SObject type feature.
 */
S_API SObject *SFeatProcessorRun(const SFeatProcessor *self, const SItem *item, s_erc *error);


/**
 * Add the SFeatProcessor class to the object system.
 *
 * @private
 * @param error Error code.
 */
S_LOCAL void _s_featprocessor_class_add(s_erc *error);


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

#endif /* _SPCT_FEATPROCESSOR_H__ */
