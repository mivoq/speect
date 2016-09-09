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


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/alloc.h"
#include "hrg/processors/featprocessor.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Call the given function method of the given #SFeatProcessor.
 * @param SELF The given #SFeatProcessor*.
 * @param FUNC The function method of the given object to call.
 * @note This casting is not safety checked.
 * @note Example usage:
 @verbatim
 S_FEATPROCESSOR_CALL(self, func)(param1, param2, ..., paramN);
 @endverbatim
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_FEATPROCESSOR_CALL(SELF, FUNC)				\
	((SFeatProcessorClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SFeatProcessor
 * can be called.
 * @param SELF The given #SFeatProcessor*.
 * @param FUNC The function method of the given object to check.
 * @return #TRUE if function can be called, otherwise #FALSE.
 * @note This casting is not safety checked.
 */
#define S_FEATPROCESSOR_METH_VALID(SELF, FUNC)		\
	S_FEATPROCESSOR_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SFeatProcessorClass FeatProcessorClass; /* FeatProcessor class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API void SFeatProcessorInit(SFeatProcessor **self, SMap *feat, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SFeatProcessorInit",
				  "Argument \"self\" is NULL");
		return;
	}

	if (!S_FEATPROCESSOR_METH_VALID((*self), initialize))
	{
		/* nothing to do */
		return;
	}

	S_FEATPROCESSOR_CALL((*self), initialize)((*self), feat , error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SFeatProcessorInit",
				  "Failed to initialize feature processor"))
	{
		S_DELETE((*self), "SFeatProcessorInit", error);
		*self = NULL;
	}

}


S_API SObject *SFeatProcessorRun(const SFeatProcessor *self, const SItem *item, s_erc *error)
{
	SObject *extractedFeat;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SFeatProcessorRun",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (item == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SFeatProcessorRun",
				  "Argument \"item\" is NULL");
		return NULL;
	}


	if (!S_FEATPROCESSOR_METH_VALID(self, run))
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SFeatProcessorRun",
				  "FeatProcessor method \"run\" not implemented");
		return NULL;
	}

	extractedFeat = S_FEATPROCESSOR_CALL(self, run)(self, item, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SFeatProcessorRun",
				  "Class method \"run\" failed"))
		return NULL;

	return extractedFeat;
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_featprocessor_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(S_OBJECTCLASS(&FeatProcessorClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_featprocessor_class_add",
			  "Failed to add SFeatProcessorClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/


static void DisposeFeatProcessor(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


/************************************************************************************/
/*                                                                                  */
/* SFeatProcessor class initialization                                              */
/*                                                                                  */
/************************************************************************************/

static SFeatProcessorClass FeatProcessorClass =
{
	/* SObjectClass */
	{
		"SFeatProcessor",
		sizeof(SFeatProcessor),
		{ 0, 1},
		NULL,                 /* init    */
		NULL,                 /* destroy */
		DisposeFeatProcessor, /* dispose */
		NULL,                 /* compare */
		NULL,                 /* print   */
		NULL,                 /* copy    */
	},
	NULL                      /* run     */
};




