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
/* DATE    : 5 July 2008                                                            */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Implementation of the generic object system.                                     */
/*                                                                                  */
/* Adapted from Agar toolkit http://libagar.org/ (1.3.2)                            */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/* libagar license                                                                  */
/*                                                                                  */
/* Copyright (c) 2002-2007 Hypertriton, Inc. <http://hypertriton.com/>              */
/* All rights reserved.                                                             */
/*                                                                                  */
/* Redistribution and use in source and binary forms, with or without               */
/* modification, are permitted provided that the following conditions               */
/* are met:                                                                         */
/* 1. Redistributions of source code must retain the above copyright                */
/*    notice, this list of conditions and the following disclaimer.                 */
/* 2. Redistributions in binary form must reproduce the above copyright             */
/*    notice, this list of conditions and the following disclaimer in the           */
/*    documentation and/or other materials provided with the distribution.          */
/*                                                                                  */
/* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"      */
/* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE        */
/* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE       */
/* ARE DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR       */
/* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL           */
/* DAMAGES (INCLUDING BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR        */
/* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER       */
/* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,    */
/* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE        */
/* USE OF THIS SOFTWARE EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.          */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/alloc.h"
#include "base/strings/strings.h"
#include "base/strings/sprint.h"
#include "base/objsystem/object_def.h"
#include "base/objsystem/object_macros.h"
#include "base/objsystem/class.h"
#include "base/objsystem/object.h"
#include "serialization/serialized_object.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SObjectClass BaseObjectClass; /* Base object class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

/*
 * Allocates a new object instance of the given class and initializes
 * its members.
 */
S_API SObject *SObjectNew(SObjectClass *cls, s_erc *error)
{
	SObject *obj;


	S_CLR_ERR(error);

	if (cls == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectNew",
				  "Argument \"cls\" is NULL");
		return NULL;
	}

	obj = S_MALLOC_SIZE(cls->size);

	if (obj == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "SObjectNew",
				  "Failed to allocate memory for object");
		return NULL;
	}

	obj->cls = cls;
	SObjectInit(obj, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SObjectNew",
				  "Failed to initialize new object"))
	{
		S_FORCE_DELETE(obj, "SObjectNew", error);
		return NULL;
	}

	return obj;
}


S_API s_bool SObjectIsType(const SObject *self, const char *type, s_erc *error)
{
	char *nname;
	char *np;
	char *s;
	int   rv;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectIsType",
				  "Argument \"self\" is NULL");
		return FALSE;
	}

	if (type == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectIsType",
				  "Argument \"type\" is NULL");
		return FALSE;
	}

	nname = s_strdup(self->cls->name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SObjectIsType",
				  "Failed to duplicate object class name"))
		return FALSE;

	np = nname;

	s = s_strtok_r(NULL, ":", &np, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SObjectIsType",
				  "Call to \"s_strtok_r\" failed"))
	{
		S_FREE(nname);
		return FALSE;
	}

	while (s != NULL)
	{
		rv = s_strcmp(type, s, error);

		if (S_CHK_ERR(error, S_CONTERR,
					  "SObjectIsType",
					  "Call to \"s_strcmp\" failed"))
		{
			S_FREE(nname);
			return FALSE;
		}

		if (rv != 0)
		{
			s = s_strtok_r(NULL, ":", &np, error);

			if (S_CHK_ERR(error, S_CONTERR,
						  "SObjectIsType",
						  "Call to \"s_strtok_r\" failed"))
			{
				S_FREE(nname);
				return FALSE;
			}

			continue;
		}
		else
		{
			S_FREE(nname);
			return TRUE;
		}
	}

	S_FREE(nname);

	return FALSE;
}


S_API const char *SObjectType(const SObject *self, s_erc *error)
{
	const char *type;

	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectType",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	type = s_class_name(S_OBJECT_CLS(self), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SObjectType",
				  "Call to \"s_class_name\" failed"))
		return NULL;

	return type;
}


S_API const char *SObjectInheritance(const SObject *self, s_erc *error)
{
	const char *inheritance;

	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectInheritance",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	inheritance = s_class_inheritance(S_OBJECT_CLS(self), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SObjectInheritance",
				  "Call to \"s_class_inheritance\" failed"))
		return NULL;

	return inheritance;
}


S_API size_t SObjectSize(const SObject *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectSize",
				  "Argument \"self\" is NULL");
		return 0;
	}

	if (self->cls == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "SObjectSize",
				  "Given object has not been properly initialized (no class info)");
		return 0;
	}

	return self->cls->size;
}


S_API void SObjectDispose(SObject *self, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectDispose",
				  "Argument \"self\" is NULL");
		return;
	}

	if (!S_OBJECT_METH_VALID(self, dispose))
		return;

	S_OBJECT_CALL(self, dispose)(self, error);
	S_CHK_ERR(error, S_CONTERR,
			  "SObjectDispose",
			  "Call to class method \"dispose\" failed");
}


S_API s_bool SObjectCompare(const SObject *oa, const SObject *ob, s_erc *error)
{
	const char *oa_class_name;
	const char *ob_class_name ;
	int string_comp;
	s_bool object_comp;


	S_CLR_ERR(error);

	if ((oa == NULL) || (ob == NULL))
		return FALSE;

	if (oa == ob)
		return TRUE;

	s_mutex_lock((s_mutex*)&(oa->object_mutex));
	s_mutex_lock((s_mutex*)&(ob->object_mutex));

	oa_class_name = S_OBJECT_CLS(oa)->name;
	ob_class_name = S_OBJECT_CLS(ob)->name;
	string_comp = s_strcmp(oa_class_name, ob_class_name, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SObjectCompare",
				  "Call to \"s_strcmp\" failed"))
	{
		s_mutex_unlock((s_mutex*)&(oa->object_mutex));
		s_mutex_unlock((s_mutex*)&(ob->object_mutex));
		return FALSE;
	}

	if (string_comp != 0) /* class names must be the same */
	{
		s_mutex_unlock((s_mutex*)&(oa->object_mutex));
		s_mutex_unlock((s_mutex*)&(ob->object_mutex));
		return FALSE;
	}

	if (!S_OBJECT_METH_VALID(oa, compare))
	{
		s_mutex_unlock((s_mutex*)&(oa->object_mutex));
		s_mutex_unlock((s_mutex*)&(ob->object_mutex));
		return FALSE; /* first object does not have a compare function */
	}

	object_comp = S_OBJECT_CALL(oa, compare)(oa, ob, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SObjectCompare",
				  "Call to class method \"compare\" failed"))
	{
		s_mutex_unlock((s_mutex*)&(oa->object_mutex));
		s_mutex_unlock((s_mutex*)&(ob->object_mutex));
		return FALSE;
	}

	s_mutex_unlock((s_mutex*)&(oa->object_mutex));
	s_mutex_unlock((s_mutex*)&(ob->object_mutex));

	return object_comp;
}


S_API char *SObjectPrint(const SObject *self, s_erc *error)
{
	char *buf = NULL;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectPrint",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	s_mutex_lock((s_mutex*)&(self->object_mutex));

	if (S_OBJECT_METH_VALID(self, print))
	{
		buf = S_OBJECT_CALL(self, print)(self, error);
		S_CHK_ERR(error, S_CONTERR,
				  "SObjectPrint",
				  "Call to class method \"print\" failed");
	}
	else
	{
		const char *object_type;

		object_type = SObjectType(self, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SObjectPrint",
					  "Call to \"SObjectType\" failed"))
		{
			s_mutex_unlock((s_mutex*)&(self->object_mutex));
			return NULL;
		}

		s_asprintf(&buf, error, "[%s]", object_type);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SObjectPrint",
					  "Call to \"s_asprintf\" failed"))
		{
			s_mutex_unlock((s_mutex*)&(self->object_mutex));
			return NULL;
		}
	}

	s_mutex_unlock((s_mutex*)&(self->object_mutex));

	return buf;
}


S_API SObject *SObjectCopy(const SObject *self, s_erc *error)
{
	SObject *ocopy;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectCopy",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (!S_OBJECT_METH_VALID(self, copy))
		return NULL;

	s_mutex_lock((s_mutex*)&(self->object_mutex));
	ocopy = S_OBJECT_CALL(self, copy)(self, error);
	s_mutex_unlock((s_mutex*)&(self->object_mutex));

	if (S_CHK_ERR(error, S_CONTERR,
				  "SObjectCopy",
				  "Call to class method \"copy\" failed"))
		return NULL;

	return ocopy;
}


S_API void SObjectRead(SObject *self, const SSerializedObject *formater,
					   SDataReader *reader, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectRead",
				  "Argument \"self\" is NULL");
		return;
	}

	if (formater == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectRead",
				  "Argument \"formater\" is NULL");
		return;
	}

	if (reader == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectRead",
				  "Argument \"reader\" is NULL");
		return;
	}

	s_mutex_lock(&(self->object_mutex));
	SSerializedObjectRead(formater, reader, self, error);
	s_mutex_unlock(&(self->object_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "SObjectRead",
			  "Call to \"SSerializedObjectRead\" failed");
}


S_API void SObjectWrite(const SObject *self, const SSerializedObject *formater,
						SDataWriter *writer, s_erc *error)
{
	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectWrite",
				  "Argument \"self\" is NULL");
		return;
	}

	if (formater == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectWrite",
				  "Argument \"formater\" is NULL");
		return;
	}

	if (writer == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectWrite",
				  "Argument \"writer\" is NULL");
		return;
	}

	s_mutex_lock((s_mutex*)&(self->object_mutex));
	SSerializedObjectWrite(formater, writer, self, error);
	s_mutex_unlock((s_mutex*)&(self->object_mutex));

	S_CHK_ERR(error, S_CONTERR,
			  "SObjectWrite",
			  "Call to \"SSerializedObjectWrite\" failed");
}


S_API void SObjectIncRef(SObject *self)
{
	if (self == NULL)
		return;

	s_mutex_lock(&(self->object_mutex));
	self->ref++;
	s_mutex_unlock(&(self->object_mutex));
}


S_API void SObjectDecRef(SObject *self)
{
	if (self == NULL)
		return;

	s_mutex_lock(&(self->object_mutex));
	if (self->ref == 0) /* no references, dont decrement */
	{
		s_mutex_unlock(&(self->object_mutex));
		return;
	}

	self->ref--;
	s_mutex_unlock(&(self->object_mutex));
}


S_API void *s_safe_cast(SObject *self, const char *cast_to, s_erc *error)
{
	s_bool type_is_good;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_safe_cast",
				  "Argument \"self\" is NULL");
		return NULL;
	}

	if (cast_to == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_safe_cast",
				  "Argument \"cast_to\" is NULL");
		return NULL;
	}

	type_is_good = SObjectIsType(self, cast_to, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_safe_cast",
				  "Call to \"SObjectIsType\" failed"))
		return NULL;

	if (type_is_good == FALSE)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_safe_cast",
				  "Failed to cast object of type '%s' to '%s'",
				  S_OBJECT_CLS(self)->name, cast_to);
		return NULL;
	}

	return self;
}


/************************************************************************************/
/*                                                                                  */
/* Class registration                                                               */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_object_class_add(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_add(&BaseObjectClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_object_class_add",
			  "Failed to add SObjectClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void InitObject(void *obj, s_erc *error)
{
	SObject *self = obj;


	S_CLR_ERR(error);
	self->ref = 0;
	s_mutex_init(&(self->object_mutex));
}


/*
 * We only destroy the mutex if SPCT_USE_THREADS
 * is defined, else there is no need for this
 * destructor.
 */
#ifdef SPCT_USE_THREADS
static void DestroyObject(void *obj, s_erc *error)
{
	SObject *self = obj;


	S_CLR_ERR(error);

	s_mutex_lock(&(self->object_mutex));

	self->ref = 0;

	s_mutex_unlock(&(self->object_mutex));
	s_mutex_destroy(&(self->object_mutex));
}
#endif /* SPCT_USE_THREADS */


static void DisposeObject(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


/************************************************************************************/
/*                                                                                  */
/* SObject class initialization                                                     */
/*                                                                                  */
/************************************************************************************/

/*
 * The base class of all objects.
 */
static SObjectClass BaseObjectClass =
{
	"SObject",
	sizeof(SObject),
	{ 0, 1},
	InitObject,	/* init    */
#ifdef SPCT_USE_THREADS
	DestroyObject,  /* destroy */
#else /* !SPCT_USE_THREADS */
	NULL,           /* destroy */
#endif /* SPCT_USE_THREADS */
	DisposeObject,  /* dispose */
	NULL,           /* compare */
	NULL,           /* print   */
	NULL            /* copy    */
};
