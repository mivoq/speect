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
/* DATE    : 5 October 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A map class, an abstract data type composed of a collection of unique string     */
/* keys and a collection of values, where each key is associated with one value.    */
/*                                                                                  */
/* Loosely based on EST_TKVL of Edinburgh Speech Tools,                             */
/* http://www.cstr.ed.ac.uk/projects/speech_tools (1.2.96)                          */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/* Edinburgh Speech Tools license,  EST_TKVL                                        */
/*                                                                                  */
/*                     Centre for Speech Technology Research                        */
/*                          University of Edinburgh, UK                             */
/*                           Copyright (c) 1995,1996                                */
/*                             All Rights Reserved.                                 */
/*       Permission is hereby granted, free of charge, to use and distribute        */
/*       this software and its documentation without restriction, including         */
/*       without limitation the rights to use, copy, modify, merge, publish,        */
/*       distribute, sublicense, and/or sell copies of this work, and to            */
/*       permit persons to whom this work is furnished to do so, subject to         */
/*       the following conditions:                                                  */
/*        1. The code must retain the above copyright notice, this list of          */
/*           conditions and the following disclaimer.                               */
/*        2. Any modifications must be clearly marked as such.                      */
/*        3. Original authors' names are not deleted.                               */
/*        4. The authors' names are not used to endorse or promote products         */
/*           derived from this software without specific prior written              */
/*           permission.                                                            */
/*       THE UNIVERSITY OF EDINBURGH AND THE CONTRIBUTORS TO THIS WORK              */
/*       DISCLAIM ALL WARRANTIES WITH REGARD TO THIS SOFTWARE, INCLUDING            */
/*       ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS, IN NO EVENT         */
/*       SHALL THE UNIVERSITY OF EDINBURGH NOR THE CONTRIBUTORS BE LIABLE           */
/*       FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES          */
/*       WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN         */
/*       AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION,                */
/*       ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF             */
/*       THIS SOFTWARE.                                                             */
/*                                                                                  */
/*       Author :  Paul Taylor                                                      */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/objsystem/class.h"
#include "base/objsystem/primitives.h"
#include "containers/map/map.h"


/************************************************************************************/
/*                                                                                  */
/*  Static variables                                                                */
/*                                                                                  */
/************************************************************************************/

static SMapClass MapClass; /* Map class declaration. */


/************************************************************************************/
/*                                                                                  */
/*  Function implementations                                                        */
/*                                                                                  */
/************************************************************************************/

/**** Accessor functions ****/

S_API sint32 SMapGetInt(const SMap *self, const char *key, s_erc *error)
{
	const SObject *tmp;
	sint32 i;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapGetInt",
				  "Argument \"self\" is NULL");
		return 0;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapGetInt",
				  "Argument \"key\" is NULL");
		return 0;
	}

	if (!S_MAP_METH_VALID(self, val_get))
	{
		S_WARNING(S_METHINVLD,
				  "SMapGetInt",
				  "Map method \"val_get\" not implemented");
		return 0;
	}

	S_LOCK_CONTAINER;
	tmp = S_MAP_CALL(self, val_get)(self, key, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapGetInt",
				  "Call to class method \"val_get\" failed"))
	{
		S_UNLOCK_CONTAINER;
		return 0;
	}

	i = SObjectGetInt(tmp, error);
	S_UNLOCK_CONTAINER;

	S_CHK_ERR(error, S_CONTERR,
			  "SMapGetInt",
			  "Call to SObjectGetInt failed");



	return i;
}


S_API float SMapGetFloat(const SMap *self, const char *key, s_erc *error)
{
	const SObject *tmp;
	float f;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapGetFloat",
				  "Argument \"self\" is NULL");
		return 0.0;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapGetFloat",
				  "Argument \"key\" is NULL");
		return 0.0;
	}

	if (!S_MAP_METH_VALID(self, val_get))
	{
		S_WARNING(S_METHINVLD,
				  "SMapGetFloat",
				  "Map method \"val_get\" not implemented");
		return 0.0;
	}

	S_LOCK_CONTAINER;
	tmp = S_MAP_CALL(self, val_get)(self, key, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapGetFloat",
				  "Call to class method \"val_get\" failed"))
	{
		S_UNLOCK_CONTAINER;
		return 0.0;
	}

	f = SObjectGetFloat(tmp, error);
	S_UNLOCK_CONTAINER;

	S_CHK_ERR(error, S_CONTERR,
			  "SMapGetFloat",
			  "Call to SObjectGetFloat failed");

	return f;
}


S_API const char *SMapGetString(const SMap *self, const char *key, s_erc *error)
{
	const SObject *tmp;
	const char *string;

	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapGetString",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapGetString",
				  "Argument \"key\" is NULL");
		return NULL;
	}

	if (!S_MAP_METH_VALID(self, val_get))
	{
		S_WARNING(S_METHINVLD,
				  "SMapGetString",
				  "Map method \"val_get\" not implemented");
		return NULL;
	}

	S_LOCK_CONTAINER;
	tmp = S_MAP_CALL(self, val_get)(self, key, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapGetString",
				  "Call to class method \"val_get\" failed"))
	{
		S_UNLOCK_CONTAINER;
		return NULL;
	}

	string = SObjectGetString(tmp, error);
	S_UNLOCK_CONTAINER;

	S_CHK_ERR(error, S_CONTERR,
			  "SMapGetString",
			  "Call to SObjectGetString failed");

	return string;
}


S_API const SObject *SMapGetObject(const SMap *self, const char *key, s_erc *error)
{
	const SObject *tmp;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapGetObject",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapGetObject",
				  "Argument \"key\" is NULL");
		return NULL;
	}

	if (!S_MAP_METH_VALID(self, val_get))
	{
		S_WARNING(S_METHINVLD,
				  "SMapGetObject",
				  "Map method \"val_get\" not implemented");
		return NULL;
	}

	S_LOCK_CONTAINER;
	tmp = S_MAP_CALL(self, val_get)(self, key, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapGetObject",
				  "Call to class method \"val_get\" failed"))
	{
		S_UNLOCK_CONTAINER;
		return NULL;
	}

	S_UNLOCK_CONTAINER;
	return tmp;
}


S_API SList *SMapGetKeys(const SMap *self, s_erc *error)
{
	SList *tmp;
	size_t size;

	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapGetKeys",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_MAP_METH_VALID(self, val_keys))
	{
		S_WARNING(S_METHINVLD,
				  "SMapGetKeys",
				  "Map method \"val_keys\" not implemented");
		return NULL;
	}

	S_LOCK_CONTAINER;

	size = SMapSize(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapGetKeys",
				  "Call to \"SMapSize\" failed"))
	{
		S_UNLOCK_CONTAINER;
		return NULL;
	}

	if (size == 0)
	{
		S_UNLOCK_CONTAINER;
		return NULL;
	}

	tmp = S_MAP_CALL(self, val_keys)(self, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapGetKeys",
				  "Call to class method \"val_keys\" failed"))
	{
		S_UNLOCK_CONTAINER;
		return NULL;
	}

	S_UNLOCK_CONTAINER;
	return tmp;
}


/**** Accessor functions with defaults ****/

S_API sint32 SMapGetIntDef(const SMap *self, const char *key,
						   sint32 def, s_erc *error)
{
	const SObject *tmp;
	sint32 i;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapGetIntDef",
				  "Argument \"self\" is NULL");
		return def;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapGetIntDef",
				  "Argument \"key\" is NULL");
		return def;
	}

	if (!S_MAP_METH_VALID(self, val_get))
	{
		S_WARNING(S_METHINVLD,
				  "SMapGetIntDef",
				  "Map method \"val_get\" not implemented");
		return def;
	}

	S_LOCK_CONTAINER;
	tmp = S_MAP_CALL(self, val_get)(self, key, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapGetIntDef",
				  "Call to class method \"val_get\" failed"))
	{
		S_UNLOCK_CONTAINER;
		return def;
	}

	if (tmp == NULL)
	{
		S_UNLOCK_CONTAINER;
		return def;
	}

	i = SObjectGetInt(tmp, error);
	S_UNLOCK_CONTAINER;

	S_CHK_ERR(error, S_CONTERR,
			  "SMapGetIntDef",
			  "Call to SObjectGetInt failed");

	return i;
}


S_API float SMapGetFloatDef(const SMap *self, const char *key,
							float def, s_erc *error)
{
	const SObject *tmp;
	float f;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapGetFloatDef",
				  "Argument \"self\" is NULL");
		return def;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapGetFloatDef",
				  "Argument \"key\" is NULL");
		return def;
	}

	if (!S_MAP_METH_VALID(self, val_get))
	{
		S_WARNING(S_METHINVLD,
				  "SMapGetFloatDef",
				  "Map method \"val_get\" not implemented");
		return def;
	}

	S_LOCK_CONTAINER;
	tmp = S_MAP_CALL(self, val_get)(self, key, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapGetFloatDef",
				  "Call to class method \"val_get\" failed"))
	{
		S_UNLOCK_CONTAINER;
		return def;
	}

	if (tmp == NULL)
	{
		S_UNLOCK_CONTAINER;
		return def;
	}

	f = SObjectGetFloat(tmp, error);
	S_UNLOCK_CONTAINER;

	S_CHK_ERR(error, S_CONTERR,
			  "SMapGetFloatDef",
			  "Call to SObjectGetFloat failed");

	return f;
}


S_API const char *SMapGetStringDef(const SMap *self, const char *key,
								   const char *def, s_erc *error)
{
	const SObject *tmp;
	const char *string;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapGetStringDef",
				  "Argument \"self\" is NULL");
		return def;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapGetStringDef",
				  "Argument \"key\" is NULL");
		return def;
	}

	if (!S_MAP_METH_VALID(self, val_get))
	{
		S_WARNING(S_METHINVLD,
				  "SMapGetStringDef",
				  "Map method \"val_get\" not implemented");
		return def;
	}

	S_LOCK_CONTAINER;
	tmp = S_MAP_CALL(self, val_get)(self, key, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapGetStringDef",
				  "Call to class method \"val_get\" failed"))
	{
		S_UNLOCK_CONTAINER;
		return def;
	}

	if (tmp == NULL)
	{
		S_UNLOCK_CONTAINER;
		return def;
	}

	string = SObjectGetString(tmp, error);
	S_UNLOCK_CONTAINER;

	S_CHK_ERR(error, S_CONTERR,
			  "SMapGetStringDef",
			  "Call to SObjectGetString failed");

	return string;
}


S_API const SObject *SMapGetObjectDef(const SMap *self, const char *key,
									  const SObject *def, s_erc *error)
{
	const SObject *tmp;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapGetObjectDef",
				  "Argument \"self\" is NULL");
		return def;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapGetObjectDef",
				  "Argument \"key\" is NULL");
		return def;
	}

	if (!S_MAP_METH_VALID(self, val_get))
	{
		S_WARNING(S_METHINVLD,
				  "SMapGetObjectDef",
				  "Map method \"val_get\" not implemented");
		return def;
	}

	S_LOCK_CONTAINER;
	tmp = S_MAP_CALL(self, val_get)(self, key, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapGetObjectDef",
				  "Call to class method \"val_get\" failed"))
	{
		S_UNLOCK_CONTAINER;
		return def;
	}

	if (tmp == NULL)
	{
		S_UNLOCK_CONTAINER;
		return def;
	}

	S_UNLOCK_CONTAINER;
	return tmp;
}


/**** Setting functions ****/

S_API void SMapSetInt(SMap *self, const char *key, sint32 i, s_erc *error)
{
	SObject *val;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapSetInt",
				  "Argument \"self\" is NULL");
		return;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapSetInt",
				  "Argument \"key\" is NULL");
		return;
	}

	if (!S_MAP_METH_VALID(self, val_set))
	{
		S_WARNING(S_METHINVLD,
				  "SMapSetInt",
				  "Map method \"val_set\" not implemented");
		return;
	}

	S_LOCK_CONTAINER;
	val = SObjectSetInt(i, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapSetInt",
				  "Call to SObjectSetInt failed"))
	{
		S_UNLOCK_CONTAINER;
		return;
	}

	S_MAP_CALL(self, val_set)(self, key, val, error);
	S_UNLOCK_CONTAINER;

	S_CHK_ERR(error, S_CONTERR,
			  "SMapSetInt",
			  "Call to class method \"val_set\" failed");
}


S_API void SMapSetFloat(SMap *self, const char *key, float f, s_erc *error)
{
	SObject *val;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapSetFloat",
				  "Argument \"self\" is NULL");
		return;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapSetFloat",
				  "Argument \"key\" is NULL");
		return;
	}

	if (!S_MAP_METH_VALID(self, val_set))
	{
		S_WARNING(S_METHINVLD,
				  "SMapSetFloat",
				  "Map method \"val_set\" not implemented");
		return;
	}

	S_LOCK_CONTAINER;
	val = SObjectSetFloat(f, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapSetFloat",
				  "Call to SObjectSetFloat failed"))
	{
		S_UNLOCK_CONTAINER;
		return;
	}

	S_MAP_CALL(self, val_set)(self, key, val, error);
	S_UNLOCK_CONTAINER;

	S_CHK_ERR(error, S_CONTERR,
			  "SMapSetFloat",
			  "Call to class method \"val_set\" failed");
}


S_API void SMapSetString(SMap *self, const char *key, const char *s, s_erc *error)
{
	SObject *val;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapSetString",
				  "Argument \"self\" is NULL");
		return;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapSetString",
				  "Argument \"key\" is NULL");
		return;
	}

	if (s == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapSetString",
				  "Argument \"s\" is NULL");
		return;
	}

	if (!S_MAP_METH_VALID(self, val_set))
	{
		S_WARNING(S_METHINVLD,
				  "SMapSetString",
				  "Map method \"val_set\" not implemented");
		return;
	}

	S_LOCK_CONTAINER;
	val = SObjectSetString(s, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapSetString",
				  "Call to SObjectSetString failed"))
	{
		S_UNLOCK_CONTAINER;
		return;
	}

	S_MAP_CALL(self, val_set)(self, key, val, error);
	S_UNLOCK_CONTAINER;

	S_CHK_ERR(error, S_CONTERR,
			  "SMapSetString",
			  "Call to class method \"val_set\" failed");
}


S_API void SMapSetObject(SMap *self, const char *key, const SObject *object, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapSetObject",
				  "Argument \"self\" is NULL");
		return;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapSetObject",
				  "Argument \"key\" is NULL");
		return;
	}

	if (object == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapSetObject",
				  "Argument \"object\" is NULL");
		return;
	}

	if (!S_MAP_METH_VALID(self, val_set))
	{
		S_WARNING(S_METHINVLD,
				  "SMapSetObject",
				  "Map method \"val_set\" not implemented");
		return;
	}

	S_LOCK_CONTAINER;
	S_MAP_CALL(self, val_set)(self, key, object, error);
	S_UNLOCK_CONTAINER;

	S_CHK_ERR(error, S_CONTERR,
			  "SMapSetObject",
			  "Call to class method \"val_set\" failed");
}


/**** Key list requests ****/

S_API void SMapObjectDelete(SMap *self, const char *key, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapObjectDelete",
				  "Argument \"self\" is NULL");
		return;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapObjectDelete",
				  "Argument \"key\" is NULL");
		return;
	}

	if (!S_MAP_METH_VALID(self, val_delete))
	{
		S_WARNING(S_METHINVLD,
				  "SMapObjectDelete",
				  "Map method \"val_delete\" not implemented");
		return;
	}

	S_LOCK_CONTAINER;
	S_MAP_CALL(self, val_delete)(self, key, error);
	S_UNLOCK_CONTAINER;

	S_CHK_ERR(error, S_CONTERR,
			  "SMapObjectDelete",
			  "Call to class method \"val_delete\" failed");
}


S_API SObject *SMapObjectUnlink(SMap *self, const char *key, s_erc *error)
{
	SObject *tmp;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapObjectUnlink",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapObjectUnlink",
				  "Argument \"key\" is NULL");
		return NULL;
	}

	if (!S_MAP_METH_VALID(self, val_unlink))
	{
		S_WARNING(S_METHINVLD,
				  "SMapObjectUnlink",
				  "Map method \"val_unlink\" not implemented");
		return NULL;
	}

	S_LOCK_CONTAINER;
	tmp = S_MAP_CALL(self, val_unlink)(self, key, error);
	S_UNLOCK_CONTAINER;

	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapObjectUnlink",
				  "Call to class method \"val_unlink\" failed"))
		return NULL;

	return tmp;
}


S_API s_bool SMapObjectPresent(const SMap *self, const char *key, s_erc *error)
{
	s_bool o_present;

	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapObjectPresent",
				  "Argument \"self\" is NULL");
		return FALSE;
	}

	if (key == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapObjectPresent",
				  "Argument \"key\" is NULL");
		return FALSE;
	}

	if (!S_MAP_METH_VALID(self, val_present))
	{
		S_WARNING(S_METHINVLD,
				  "SMapObjectPresent",
				  "Map method \"val_present\" not implemented");
		return FALSE;
	}

	S_LOCK_CONTAINER;
	o_present = S_MAP_CALL(self, val_present)(self, key, error);
	S_UNLOCK_CONTAINER;

	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapObjectPresent",
				  "Call to class method \"val_present\" failed"))
		return FALSE;

	return o_present;
}


S_API size_t SMapSize(const SMap *self, s_erc *error)
{
	size_t sizem;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapSize",
				  "Argument \"self\" is NULL");
		return 0;
	}

	if (!S_MAP_METH_VALID(self, size))
	{
		S_WARNING(S_METHINVLD,
				  "SMapSize",
				  "Map method \"size\" not implemented");
		return 0;
	}

	S_LOCK_CONTAINER;
	sizem = S_MAP_CALL(self, size)(self, error);
	S_UNLOCK_CONTAINER;

	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapSize",
				  "Call to class method \"size\" failed"))
		return 0;

	return sizem;
}


S_API SMap *SMapCopy(SMap *dst, const SMap *src, s_erc *error)
{
	SMap *tDst;


	S_CLR_ERR(error);

	if (src == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SMapCopy",
				  "Argument \"src\" is NULL");
		return dst;
	}

	if (!S_MAP_METH_VALID(src, copy))
	{
		S_WARNING(S_METHINVLD,
				  "SMapCopy",
				  "Map method \"copy\" not implemented");
		return dst;
	}

	/* lock both containers */
	if (dst != NULL)
		s_mutex_lock(&(S_CONTAINER(dst)->container_mutex));
	s_mutex_lock(&(S_CONTAINER(src)->container_mutex));

	tDst = S_MAP_CALL(src, copy)(dst, src, error);

	/* unlock both containers */
	if (dst != NULL)
		s_mutex_unlock(&(S_CONTAINER(dst)->container_mutex));
	s_mutex_unlock(&(S_CONTAINER(src)->container_mutex));

	if (S_CHK_ERR(error, S_CONTERR,
				  "SMapSize",
				  "Call to class method \"copy\" failed"))
		return dst;

	return tDst;
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_map_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(&MapClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_map_class_add",
			  "Failed to add SMapClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void DisposeMap(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


/************************************************************************************/
/*                                                                                  */
/* SMap class initialization                                                        */
/*                                                                                  */
/************************************************************************************/

static SMapClass MapClass =
{
	{
		/* SObjectClass */
		{
			"SContainer:SMap",
			sizeof(SMap),
			{ 0, 1},
			NULL,              /* init    */
			NULL,              /* destroy */
			DisposeMap,        /* dispose */
			NULL,              /* compare */
			NULL,              /* print   */
			NULL,              /* copy    */
		},
		/* SContainerClass */
		NULL,                  /* get_iterator */
	},
	/* SMapClass */
	NULL,                          /* val_get      */
	NULL,                          /* val_set      */
	NULL,                          /* val_delete   */
	NULL,                          /* val_unlink   */
	NULL,                          /* val_present  */
	NULL,                          /* val_keys     */
	NULL,                          /* size         */
	NULL                           /* copy         */
};
