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


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/alloc.h"
#include "hrg/processors/uttprocessor.h"


/************************************************************************************/
/*                                                                                  */
/* Macros                                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * @hideinitializer
 * Call the given function method of the given #SUttProcessor.
 * @param SELF The given #SUttProcessor*.
 * @param FUNC The function method of the given object to call.
 * @note This casting is not safety checked.
 * @note Example usage:
 @verbatim
 S_UTTPROCESSOR_CALL(self, func)(param1, param2, ..., paramN);
 @endverbatim
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
#define S_UTTPROCESSOR_METH_VALID(SELF, FUNC)		\
	S_UTTPROCESSOR_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SUttProcessorClass UttProcessorClass; /* UttProcessor class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API void SUttProcessorInit(SUttProcessor **self, const SVoice *voice, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUttProcessorInit",
				  "Argument \"self\" is NULL");
		return;
	}

	if (voice == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUttProcessorInit",
				  "Argument \"voice\" is NULL");
		return;
	}

	if (!S_UTTPROCESSOR_METH_VALID((*self), initialize))
	{
		/* nothing to do */
		return;
	}

	S_UTTPROCESSOR_CALL((*self), initialize)((*self), voice, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SUttProcessorInit",
				  "Failed to initialize utterance processor"))
	{
		S_DELETE((*self), "SUttProcessorInit", error);
		*self = NULL;
	}
}



S_API void SUttProcessorRun(const SUttProcessor *self, SUtterance *utt, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUttProcessorRun",
				  "Argument \"self\" is NULL");
		return;
	}

	if (utt == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUttProcessorRun",
				  "Argument \"utt\" is NULL");
		return;
	}

	if (!S_UTTPROCESSOR_METH_VALID(self, run))
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUttProcessorRun",
				  "UttProcessor method \"run\" not implemented");
		return;
	}

	S_UTTPROCESSOR_CALL(self, run)(self, utt, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SUttProcessorRun",
			  "Failed to process utterance");
}


S_API s_bool SUttProcessorFeatureIsPresent(const SUttProcessor *self,
										   const char *name,
										   s_erc *error)
{
	s_bool feat_present;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUttProcessorFeatureIsPresent",
				  "Argument \"self\" is NULL");
		return FALSE;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUttProcessorFeatureIsPresent",
				  "Argument \"name\" is NULL");
		return FALSE;
	}

	feat_present = SMapObjectPresent(self->features, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SUttProcessorFeatureIsPresent",
				  "Call to \"SMapObjectPresent\" failed"))
		return FALSE;

	return feat_present;
}


S_API const SObject *SUttProcessorGetFeature(const SUttProcessor *self,
											 const char *name, s_erc *error)
{
	const SObject *object;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUttProcessorGetFeature",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUttProcessorGetFeature",
				  "Argument \"name\" is NULL");
		return NULL;
	}

	object = SMapGetObject(self->features, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SUttProcessorGetFeature",
				  "Call to \"SMapGetObject\" failed"))
		return NULL;

	return object;
}


S_API void SUttProcessorSetFeature(SUttProcessor *self, const char *name,
								   const SObject *object, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUttProcessorSetFeature",
				  "Argument \"self\" is NULL");
		return;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUttProcessorSetFeature",
				  "Argument \"name\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUttProcessorSetFeature",
				  "Argument \"object\" is NULL");
		return;
	}

	SMapSetObject(self->features, name, object, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SUttProcessorSetFeature",
			  "Call to \"SMapSetObject\" failed");
}


S_API void SUttProcessorDelFeature(SUttProcessor *self, const char *name,
								   s_erc *error)
{
	s_bool feat_present;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUttProcessorDelFeature",
				  "Argument \"self\" is NULL");
		return;
	}

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SUttProcessorDelFeature",
				  "Argument \"name\" is NULL");
		return;
	}

	feat_present = SUttProcessorFeatureIsPresent(self, name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SUttProcessorDelFeature",
				  "Call to \"SUttProcessorFeatPresent\" failed"))
		return;

	if (feat_present)
	{
		SMapObjectDelete(self->features, name, error);
		S_CHK_ERR(error, S_CONTERR,
				  "SUttProcessorDelFeature",
				  "Call to \"SMapObjectDelete\" failed");
	}
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_uttprocessor_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(S_OBJECTCLASS(&UttProcessorClass), error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_uttprocessor_class_add",
			  "Failed to add SUttProcessorClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitUttProcessor(void *obj, s_erc *error)
{
	SUttProcessor *self = obj;


	S_CLR_ERR(error);

	self->features = S_MAP(S_NEW("SMapList", error));
	if (S_CHK_ERR(error, S_CONTERR,
				  "InitUttProcessor",
				  "Failed to create new map-list features"))
		self->features = NULL;
}


static void DestroyUttProcessor(void *obj, s_erc *error)
{
	SUttProcessor *self = obj;


	S_CLR_ERR(error);
	if (self->features != NULL)
		S_DELETE(self->features, "DestroyUttProcessor", error);
}


static void DisposeUttProcessor(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


/************************************************************************************/
/*                                                                                  */
/* SUttProcessor class initialization                                               */
/*                                                                                  */
/************************************************************************************/

static SUttProcessorClass UttProcessorClass =
{
	/* SObjectClass */
	{
		"SUttProcessor",
		sizeof(SUttProcessor),
		{ 0, 1},
		InitUttProcessor,    /* init    */
		DestroyUttProcessor, /* destroy */
		DisposeUttProcessor, /* dispose */
		NULL,                /* compare */
		NULL,                /* print   */
		NULL,                /* copy    */
	},
	/* SUttProcessorClass */
	NULL,                    /* initialize */
	NULL                     /* run        */
};




