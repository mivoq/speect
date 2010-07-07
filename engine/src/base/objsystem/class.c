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
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

/*
 * If SPCT_DEBUG_OBJECTS is defined then all Speect object creation
 * and deletion information is printed to stdout.
 */
#if 0
#define SPCT_DEBUG_OBJECTS
#endif


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/containers/hashtable/hash_table.h"
#include "base/utils/types.h"
#include "base/utils/alloc.h"
#include "base/utils/math.h"
#include "base/strings/strings.h"
#include "base/objsystem/object_def.h"
#include "base/objsystem/object_macros.h"
#include "base/objsystem/class.h"

#ifdef SPCT_DEBUGMODE
#include <stdio.h>
#define SPCT_DEBUG_STDIO_INCLUDED
#endif /* SPCT_DEBUGMODE */

#ifdef SPCT_DEBUG_OBJECTS
#include "base/objsystem/object.h"
  #ifndef SPCT_DEBUG_STDIO_INCLUDED
  #include <stdio.h>
  #endif /* SPCT_DEBUG_STDIO_INCLUDED */
#endif /* SPCT_DEBUG_OBJECTS */


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static s_hash_table *s_classes = NULL;    /* Speect class repository */


/************************************************************************************/
/*                                                                                  */
/* Data types (opaque)                                                              */
/*                                                                                  */
/************************************************************************************/

/* class initialization function pointer */
typedef void (*s_init_fp)(void *obj, s_erc *error);


/* class destroy function pointer */
typedef void (*s_destroy_fp)(void *obj, s_erc *error);


/* Class hierarchy initialization and destroy functions. */
typedef struct s_class_info_s
{
	const void    *cls;                /*!< Class pointer.                     */
	uint           n_hier;             /*!< Number of classes in hierarchy.    */
	s_init_fp     *init_hier;          /*!< Initialization function hierarchy. */
	s_destroy_fp  *destroy_hier;       /*!< Destroy function hierarchy.        */
} s_class_info;


/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static void s_class_get_hierarchy(const SObjectClass *self, SObjectClass ***hier,
								  int *n_hier, s_erc *error);

static const s_class_info *s_class_find_info(const char *name, s_erc *error);

static void s_hash_table_delete_classes(void *key, void *data, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API void s_class_add(const void *cls, s_erc *error)
{
	const SObjectClass *baseClass;
	const char         *class_name;
	s_class_info       *class_info;
	s_hash_element     *he;


	S_CLR_ERR(error);

	baseClass = (const SObjectClass*)cls;
	class_name = s_class_name(baseClass, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_class_add",
				  "Failed to get class name"))
		return;

	class_info = S_MALLOC(s_class_info, 1);

	if (class_info == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_class_add",
				  "Failed to allocate memory for class info object");
		return;
	}

	class_info->cls = cls;
	class_info->n_hier = 0;
	class_info->init_hier = NULL;
	class_info->destroy_hier = NULL;

	/*
	 * first check if class_name exists in _spct_classes, one should
	 * not be able to reregister a class.
	 */
	he = s_hash_table_find(s_classes, (void*)class_name,
						   s_strzsize(class_name, error), error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_class_add",
				  "Call to s_hash_table_find failed"))
	{
		S_FREE(class_info);
		return;
	}

	if (he)
	{
		S_FREE(class_info);
		S_CTX_ERR(error, S_FAILURE,
				  "s_class_add",
				  "Failed to add class '%s', class names must be unique",
				  class_name);
		return;
	}

	/* now try to add it. */
	s_hash_table_add(s_classes, (void*)class_name,
					 s_strzsize(class_name, error), (void*)class_info,
					 error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_class_add",
				  "Failed to add class info for class '%s' to hash table",
				  class_name))
	{
		S_FREE(class_info);
		return;
	}
}


S_API void s_class_init(const void *cls, s_erc *error)
{
	const SObjectClass  *baseClass;
	SObjectClass       **hier = NULL;
	s_class_info        *class_info;
	const char          *class_name;
	int                  n_hier;
	int                  i;
	s_init_fp           *init_hier;
	s_destroy_fp        *destroy_hier;


	S_CLR_ERR(error);

	baseClass = (const SObjectClass*)cls;
	class_name = s_class_name(baseClass, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_class_init",
				  "Failed to get class name"))
		return;

	/* get inheritance hierarchy */
	s_class_get_hierarchy(baseClass, &hier, &n_hier, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_class_init",
				  "Failed to get class hierarchy"))
		return;

	if (n_hier > 0)
	{
		/* get the class info */
		class_info = (s_class_info*)s_class_find_info(class_name, error);

		if (S_CHK_ERR(error, S_CONTERR,
					  "s_class_init",
					  "Failed to find class info for class '%s'", class_name))
			return;

		init_hier = S_MALLOC(s_init_fp, n_hier);

		if (init_hier == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "s_class_init",
					  "Failed to allocate memory for init_hier");
			return;
		}

		destroy_hier = S_MALLOC(s_destroy_fp, n_hier);

		if (init_hier == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "s_class_init",
					  "Failed to allocate memory for destroy_hier");
			return;
		}

		/* set the inheritance hierarchy init and destroy functions */
		for (i = 0; i < n_hier; i++)
		{
			init_hier[i] = hier[i]->init;
			destroy_hier[i] = hier[i]->destroy;
		}

		S_FREE(hier);

		class_info->init_hier = init_hier;
		class_info->destroy_hier = destroy_hier;
		class_info->n_hier = n_hier;
	}
}


S_API void s_class_reg(const void *cls, s_erc *error)
{
	S_CLR_ERR(error);

	s_class_add(cls, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_class_reg",
				  "Failed to add class to object system"))
		return;

	s_class_init(cls, error);
	S_CHK_ERR(error, S_CONTERR,
			  "s_class_reg",
			  "Failed to init class with object system");
}


S_API void s_class_free(const void *cls, s_erc *error)
{
	const SObjectClass *baseClass;
	const char         *class_name;
	s_hash_element     *hte;


	S_CLR_ERR(error);

	baseClass = (const SObjectClass*)cls;
	class_name = s_class_name(baseClass, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_class_free",
				  "Failed to get class name"))
		return;

	hte = s_hash_table_find(s_classes, (uchar*)class_name,
							s_strzsize(class_name, error), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_class_free",
				  "Call to \"s_hash_table_find\" failed"))
		return;

	if (!hte)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_class_free",
				  "Failed to find class '%s'", class_name);
		return;
	}

	s_hash_element_delete(hte, error);
	S_CHK_ERR(error, S_CONTERR,
			  "s_class_free",
			  "Failed to free class '%s'", class_name);
}


S_API const char *s_class_name(const SObjectClass *cls, s_erc *error)
{
	const char *class_name;
	const char *end;


	S_CLR_ERR(error);

	/* get class name */
	end = s_strrchr(cls->name, ':', error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_class_name",
				  "Failed to extract class name from hierarchy"))
		return NULL;

	if (end != NULL)
		class_name = end + 1;
	else
		class_name = cls->name;

	return class_name;
}


S_API const char *s_class_inheritance(const SObjectClass *cls, s_erc *error)
{
	S_CLR_ERR(error);

	/* get class inheritance */
	return cls->name;
}


S_API const SObjectClass *s_class_find(const char *name, s_erc *error)
{
	const s_class_info *class_info;


	S_CLR_ERR(error);

	class_info = s_class_find_info(name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_class_find",
				  "Failed to find class info for class '%s'", name))
		return NULL;

	return (const SObjectClass*)class_info->cls;
}


S_API s_bool s_class_is_reg(const char *name, s_erc *error)
{
	s_hash_element *hte;


	S_CLR_ERR(error);

	hte = s_hash_table_find(s_classes, (uchar*)name,
							s_strzsize(name, error), error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_class_is_reg",
				  "Call to \"s_hash_table_find\" failed"))
		return FALSE;

	if (!hte)
		return FALSE;

	return TRUE;
}


S_API SObject *SObjectNewFromName(const char *name, s_erc *error)
{
	SObject *obj;
	const s_class_info *class_info;
	SObjectClass *class;
	uint i;


	S_CLR_ERR(error);

	if (name == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectNewFromName",
				  "Argument \"name\" is NULL");
		return NULL;
	}

	/* get the class information */
	class_info = s_class_find_info(name, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "SObjectNewFromName",
				  "Call to \"s_class_find_info\" for object type '%s' failed",
				  name))
		return NULL;

	if (class_info == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "SObjectNewFromName",
				  "Failed to find class information for object type '%s'",
				  name);
		return NULL;
	}

	class = (SObjectClass *)class_info->cls;

	if (class == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "SObjectNewFromName",
				  "Class information for object type '%s' lacks a class pointer",
				  name);
		return NULL;
	}

	obj = S_MALLOC_SIZE(class->size);

	if (obj == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "SObjectNewFromName",
				  "Failed to allocate memory for object");
		return NULL;
	}

	obj->cls = class;

	/* execute init functions */
	for (i = 0; i < class_info->n_hier; i++)
	{
		if (class_info->init_hier[i] != NULL)
		{
			class_info->init_hier[i](obj, error);

			if (S_CHK_ERR(error, S_CONTERR,
						  "SObjectNewFromName",
						  "Failed to run init[%d] of object type '%s'",
						  i, name))
			{
				S_FORCE_DELETE(obj, "SObjectNewFromName", error);
				return NULL;
			}
		}
	}

#ifdef SPCT_DEBUG_OBJECTS
	printf("created new object of type '%s', at address %p\n", SObjectType(obj, error), obj);
#endif /* SPCT_DEBUG_OBJECTS */

	return obj;
}


S_LOCAL void SObjectInit(SObject *obj, s_erc *error)
{
	const s_class_info *class_info;
	const char         *class_name;
	uint                i;


	S_CLR_ERR(error);

	if (obj == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectInit",
				  "Argument \"obj\" is NULL");
		return;
	}

	if (obj->cls == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "SObjectInit",
				  "Given object has not been properly initialized (no class info)");
		return;
	}

	/* get class name */
	class_name = s_class_name(obj->cls, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SObjectInit",
				  "Failed to find object class name"))
		return;

	/* get class info */
	class_info = s_class_find_info(class_name, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SObjectInit",
				  "Failed to find class info for class '%s'", class_name))
		return;

	/* execute init functions */
	for (i = 0; i < class_info->n_hier; i++)
	{
		if (class_info->init_hier[i] != NULL)
		{
			class_info->init_hier[i](obj, error);

			if (S_CHK_ERR(error, S_CONTERR,
						  "SObjectInit",
						  "Failed to run init[%d] of class '%s'",
						  i, class_name))
				return;
		}
	}
}


/*
 * Decrement the reference counter (done by macro, see object_macros.h)
 * for the object and release the resources allocated, if it is no
 * longer referenced.
 */
S_API void SObjectDelete(SObject *self, s_erc *error)
{
	const s_class_info *class_info;
	const char         *class_name;
	int                 i;


	S_CLR_ERR(error);

	if (self == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "SObjectDelete",
				  "Argument \"self\" is NULL");
		return;
	}

	if (self->cls == NULL)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "SObjectDelete",
				  "Given object has not been properly initialized (no class)");
		return;
	}

#ifdef SPCT_DEBUG_OBJECTS
	printf("deleting object of type '%s', at address %p\n", SObjectType(self, NULL), self);
#endif /* SPCT_DEBUG_OBJECTS */

	/* get class name */
	class_name = s_class_name(self->cls, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SObjectDelete",
				  "Failed to find object class name"))
		return;

	/* get class info */
	class_info = s_class_find_info(class_name, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "SObjectDelete",
				  "Failed to find class info for class '%s'", class_name))
		return;

	/* execute destroy functions */
	for (i = class_info->n_hier - 1; i >= 0; i--)
	{
		if (class_info->destroy_hier[i] != NULL)
		{
			class_info->destroy_hier[i](self, error);

			if (S_CHK_ERR(error, S_CONTERR,
						  "SObjectDelete",
						  "Failed to run init[%d] of class '%s'",
						  i, class_name))
				return;
		}
	}

	S_FREE(self);
}


S_LOCAL void _s_classes_create(size_t size, s_erc *error)
{
	S_CLR_ERR(error);

	if (s_classes != NULL)
		return;

	s_classes = s_hash_table_new(s_hash_table_delete_classes,
								 (size_t)ceil(s_log2(size)), error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_classes_create",
				  "Call to \"s_hash_table_new\" failed"))
	{
		s_classes = NULL;
		return;
	}
}


/* iterate through class repository and initialize each one */
S_LOCAL void _s_classes_init(s_erc *error)
{
	const char *class_name;
	const s_class_info *class_info;
	const SObjectClass *class;
	s_hash_element *hte;


	S_CLR_ERR(error);

	hte = s_hash_table_first(s_classes, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_init_classes",
				  "Call to \"s_hash_table_first\" failed"))
		return;

	if (!hte)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "_s_init_classes",
				  "Failed to get first class in class table");
		return;
	}

	do
	{
		class_name = (const char *)s_hash_element_key(hte, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_classes_init",
					  "Call to \"s_hash_element_key\" failed"))
			return;

		class_info = (const s_class_info *)s_hash_element_get_data(hte, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_classes_init",
					  "Call to \"s_hash_element_data\" failed"))
			return;

		class = (const SObjectClass*)class_info->cls;

		/* initialize if not already */
		if (class_info->n_hier == 0)
		{
			s_class_init(class, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "_s_classes_init",
						  "Call to \"s_class_init\" failed"))
				return;
		}

		hte = s_hash_element_next(hte, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_classes_init",
					  "Call to \"s_hash_table_next\" failed"))
			return;
	} while (hte != NULL);
}


/* iterate through class repository and free each one */
S_LOCAL void _s_classes_clear(s_erc *error)
{
	S_CLR_ERR(error);

	s_hash_table_delete(s_classes, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_classes_clear",
				  "Call to \"s_hash_table_delete\" failed"))
		return;

	s_classes = NULL;
}


#ifdef SPCT_DEBUGMODE
S_LOCAL void _s_classes_print(s_erc *error)
{
	s_hash_element *hte;
	const char *class_name;
	uint32 pos;
	char *buf;


	S_CLR_ERR(error);

	hte = s_hash_table_first(s_classes, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_classes_print",
				  "Call to \"s_hash_table_first\" failed"))
		return;

	if (hte == NULL)
		return;

	do
	{
		class_name = (const char *)s_hash_element_key(hte, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_classes_print",
					  "Call to \"s_hash_element_key\" failed"))
			return;

		pos = s_hash_element_pos(hte, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_classes_print",
					  "Call to \"s_hash_element_pos\" failed"))
			return;

		S_DEBUG(S_DBG_TRACE, "class '%s' at position %d\n", class_name, pos);

		hte = s_hash_element_next(hte, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_classes_print",
					  "Call to \"s_hash_element_next\" failed"))
			return;
	} while (hte);


	buf = s_hash_table_stats(s_classes, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_classes_print",
				  "Call to \"s_hash_table_stats\" failed"))
	{
		S_FREE(buf);
		return;
	}

	S_DEBUG(S_DBG_TRACE, "hash table stats for class repository:\n%s", buf);
	S_FREE(buf);
}
#endif /* SPCT_DEBUGMODE */


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void s_hash_table_delete_classes(void *key, void *data, s_erc *error)
{
	s_class_info *class_info;
	const char *class_name;


	S_CLR_ERR(error);
	class_name = (const char *)key;
	class_info = (s_class_info *)data;

	S_FREE(class_info->init_hier);
	S_FREE(class_info->destroy_hier);
	S_FREE(class_info);
}


static void s_class_get_hierarchy(const SObjectClass *self, SObjectClass ***hier,
								  int *n_hier, s_erc *error)
{
	char               *cname;
	char               *c;
	char               *cp;
	const SObjectClass *cls;
	int                 i;


	S_CLR_ERR(error);

	(*n_hier) = 0;

	if (self->name[0] == '\0')
		return;

	/* get number of classes in hierarchy */
	cname = s_strdup(self->name, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_class_get_hierarchy",
				  "Failed to duplicate class name"))
		return;

	cp = cname;

	do
	{
		c = s_strtok_r(NULL, ":", &cp, error);

		if (S_CHK_ERR(error, S_CONTERR,
					  "s_class_get_hierarchy",
					  "Failed to extract token from class name"))
		{
			S_FREE(cname);
			return;
		}

		if (c == NULL)
			break;

		(*n_hier)++;

	} while (c != NULL);

	/* add 1 for SObjectClass */
	(*n_hier)++;

	*hier = S_MALLOC(SObjectClass*, (*n_hier));

	if (*hier == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_class_get_hierarchy",
				  "Failed to allocate memory for class hierarchy");
		S_FREE(cname);
		return;
	}

	S_FREE(cname);

	/* get hierarchy */
	cname = s_strdup(self->name, error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_class_get_hierarchy",
				  "Failed to duplicate class name"))
		return;

	cp = cname;
	i = 0;

	/* first one is SObjectClass */
	(*hier)[i++] = (SObjectClass*)s_class_find("SObject", error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_class_get_hierarchy",
				  "Failed to find class 'SObject'"))
	{
		S_FREE(*hier);
		S_FREE(cname);
		return;
	}

	do
	{
		c = s_strtok_r(NULL, ":", &cp, error);

		if (S_CHK_ERR(error, S_CONTERR,
					  "s_class_get_hierarchy",
					  "Failed to extract token from class name"))
		{
			S_FREE(*hier);
			S_FREE(cname);
			return;
		}

		if (c == NULL)
			break;

		cls = s_class_find(c, error);

		if (S_CHK_ERR(error, S_CONTERR,
					  "s_class_get_hierarchy",
					  "Failed to find class '%s'", c))
		{
			S_FREE(*hier);
			S_FREE(cname);
			return;
		}

		(*hier)[i++] = (SObjectClass*)cls;

	} while (c != NULL);

	S_FREE(cname);
}


static const s_class_info *s_class_find_info(const char *name, s_erc *error)
{
	const s_class_info *class_info;
	s_hash_element *hte;


	S_CLR_ERR(error);

	hte = s_hash_table_find(s_classes, (uchar*)name,
							s_strzsize(name, error), error);

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_class_find_info",
				  "Call to \"s_hash_table_find\" failed"))
		return NULL;

	if (!hte)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_class_find_info",
				  "Failed to find class '%s'", name);
		return NULL;
	}

	class_info = (const s_class_info *)s_hash_element_get_data(hte, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_class_find_info",
				  "Call to \"s_hash_element_get_data\" failed"))
		return NULL;

	return class_info;
}
