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
/* DATE    : 17 November 2009                                                       */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Object system example.                                                           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#include "speect.h"

typedef struct
{
	/* Class members */
	SObjectClass  _inherit;	  /* Inherit from SObjectClass. */

	/* Class methods */
	const char *(*get_name) (Person *self, s_erc *error);
	void        (*set_name) (Person *self, const char *name, s_erc *error);
} PersonClass;

/* PersonClass init function */
static void PersonInit(void *obj, s_erc *error);

/* PersonClass destroy function */
static void PersonDestroy(void *obj, s_erc *error);

/* PersonClass functions */
static const char *get_person_name(Person *self, s_erc *error);
static void set_person_name(Person *self, const char *name, s_erc *error);

static PersonClass PClass =
{
	/* SObjectClass */
	{
		"Person",             /* name    */
        	sizeof(Person),       /* size    */
		{ 0, 1},              /* version */
		PersonInit,           /* init    */
		PersonDestroy,        /* destroy */
		NULL,                 /* compare */
		NULL,                 /* print   */
		NULL,                 /* copy    */
		NULL,                 /* read    */
		NULL,                 /* write   */
	},
	/* PersonClass */
	get_person_name,              /* get_name  */
	set_person_name               /* read_at   */
};
