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
/* DATE    : 19 May 2008                                                            */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Utf8 aware regular expressions.                                                  */
/*                                                                                  */
/* Adapted from Unix port of the Plan 9 regular expression library.                 */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Unix port of the Plan 9 regular expression library license                       */
/*                                                                                  */
/*                                                                                  */
/* The authors of this software is Rob Pike.                                        */
/*		Copyright (c) 2002 by Lucent Technologies.                                  */
/* Permission to use, copy, modify, and distribute this software for any            */
/* purpose without fee is hereby granted, provided that this entire notice          */
/* is included in all copies of any software which is or includes a copy            */
/* or modification of this software and in all copies of the supporting             */
/* documentation for such software.                                                 */
/* THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED          */
/* WARRANTY.  IN PARTICULAR, NEITHER THE AUTHORS NOR LUCENT TECHNOLOGIES MAKE ANY   */
/* REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY            */
/* OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.                      */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include <string.h>
#include "base/containers/buffer/buffer.h"
#include "base/utils/alloc.h"
#include "base/strings/strings.h"
#include "base/strings/char.h"
#include "base/strings/regexp.h"


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

#define	S_REGEX_NSTACK  20
/* #define S_REGEX_DEBUG  */

/* Maximum number of sub expression matches */
#define S_REGEX_NSUBEXP 32


/*
 * Actions and Tokens (Reinst types)
 *
 *	02xx are operators, value == precedence
 *	03xx are tokens, i.e. operands for operators
 */
#define S_REGEX_RUNE		0177
#define	S_REGEX_OPERATOR	0200	/* Bitmask of all operators */
#define	S_REGEX_START	    0200	/* Start, used for marker on stack */
#define	S_REGEX_RBRA		0201	/* Right bracket, ) */
#define	S_REGEX_LBRA		0202	/* Left bracket, ( */
#define	S_REGEX_OR		    0203	/* Alternation, | */
#define	S_REGEX_CAT		    0204	/* Concatentation, implicit operator */
#define	S_REGEX_STAR		0205	/* Closure, * */
#define	S_REGEX_PLUS		0206	/* a+ == aa* */
#define	S_REGEX_QUEST	    0207	/* a? == a|nothing, i.e. 0 or 1 a's */
#define	S_REGEX_ANY		    0300	/* Any character except newline, . */
#define	S_REGEX_ANYNL	    0301	/* Any character including newline, . */
#define	S_REGEX_NOP		    0302	/* No operation, internal use only */
#define	S_REGEX_BOL		    0303	/* Beginning of line, ^ */
#define	S_REGEX_EOL		    0304	/* End of line, $ */
#define	S_REGEX_CCLASS	    0305	/* Character class, [] */
#define	S_REGEX_NCCLASS	    0306	/* Negated character class, [^ ] */
#define	S_REGEX_END		    0377	/* Terminate: match found */


/*
 *  regexec execution lists
 */
#define S_REGEX_LISTSIZE	10
#define S_REGEX_BIGLISTSIZE	(10*S_REGEX_LISTSIZE)


#define nelem(x) (sizeof(x)/sizeof((x)[0]))

/* max rune ranges per character class */
#define S_REGEX_NCCRUNE	(sizeof(s_regex_char_class)/sizeof(uint32))

/* debug, produces valgrind errors */
/* #define	S_REGEX_DEBUG */

/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/*
 * Regular expression sub expression matches.
 */
typedef struct s_regex_sub
{
 	char   *sp;        /*!< Start pointer to sub expression match. */
 	char   *ep;        /*!< End pointer to sub expression match.   */
} s_regex_sub;


/*
 * Sub-expression elements of a matched regular expression.
 */
struct s_regexsub
{
	s_regex_sub m[S_REGEX_NSUBEXP];   /*!< Sub-expression matches.           */
	uint8       n;                    /*!< Number of sub-expression matches. */
};


/*
 * Regular expression sub expression list matches.
 */
typedef struct
{
	s_regex_sub   m[S_REGEX_NSUBEXP];   /*!< Sub expression matches. */
} s_regex_sub_list;


/*
 * Character class.
 */
typedef struct
{
	uint32 *end;
	uint32 spans[64];
} s_regex_char_class;


/*
 * Regular expression instructions.
 */
typedef struct s_regex_inst_s
{
	int type;
	union
	{
		s_regex_char_class     *cp;        /*!< Class pointer.                      */
		uint32                  r;         /*!< character.                          */
		int                     subid;     /*!< sub-expression id for RBRA and LBRA */
		struct s_regex_inst_s  *right;     /*!< right child of OR                   */
	} u1;
	union   /* regex relies on these two being in the same union */
	{
		struct s_regex_inst_s  *left;      /*!< left child of OR                   */
		struct s_regex_inst_s  *next;      /*!< next instruction for CAT & LBRA    */
	} u2;
} s_regex_inst;


/*
 * Regular expression lists.
 */
typedef struct
{
	s_regex_inst      *inst;    /*!< Reinstruction of the thread.           */
	s_regex_sub_list   se;      /*!< matched subexpressions in this thread. */
} s_regex_list;


/*
 * Regular expression ljunk.
 */
typedef struct
{
	s_regex_list *relist[2];
	s_regex_list *reliste[2];
	int	          starttype;
	uint32	      startchar;
	char         *starts;
	char         *eol;
	uint32       *rstarts;
	uint32       *reol;
} s_regex_ljunk;

/* max character classes per program */
#define	S_REGEX_NCLASS 16

/*
 * Typedef of struct s_regex.
 */
struct s_regex
{
	s_regex_inst       *start_inst;      /*!< start pc */
	s_regex_char_class  class[S_REGEX_NCLASS];       /*!< data     */
	s_regex_inst        first_inst[5];   /*!< text     */
};


/*
 * Parser Information
 */
typedef struct
{
	s_regex_inst *first;
	s_regex_inst *last;
} s_regex_node;

/************************************************************************************/
/*                                                                                  */
/* Static function prototypes                                                       */
/*                                                                                  */
/************************************************************************************/

static s_regex *s_regcomp1(const char *s, int literal, int dot_type, s_erc *error);

static void s_pushator(int t, int **atorp, int **subidp, int cursubid,
					   int *atorstack, s_erc *error);

static int s_nextc(uint32 *rp, s_bool *lexdone, char **exprp, s_erc *error);

static int s_char_to_ucs4(uint32 *ucs4, char *str, s_erc *error);

static s_bool s_utf8_forbidden(uchar octet, s_erc *error);

static void s_operator(int t, int *nbra, int *cursubid, s_bool *lastwasand,
					   int **atorp, int *atorstack, int **subidp,
					   s_regex_node *andstack, s_regex_node **andp,
					   s_regex_inst **freep, s_erc *error);

static int s_popator(int **atorp, int *atorstack, int **subidp, s_erc *error);

static s_regex_node *s_popand(s_regex_node *andstack, s_regex_node **andp,
							  int op, s_erc *error);

static s_regex_inst *s_newinst(int t, s_regex_inst **freep, s_erc *error);

static void s_pushand(s_regex_inst *f, s_regex_inst *l,
					  s_regex_node *andstack,
					  s_regex_node **andp,
					  s_erc *error);

static int s_bldcclass(s_regex_char_class **yyclassp, uint *nclass,
					   s_regex_char_class *classp, s_bool *lexdone,
					   char **exprp, s_erc *error);

static int s_lex(int literal, int dot_type, uint32 *yyrune,
				 s_bool *lexdone, char **exprp, s_regex_char_class **yyclassp,
				 uint *nclass, s_regex_char_class *classp,
				 s_erc *error);

static void s_operand(int t, s_bool *lastwasand, int **atorp,
					  int *atorstack, int **subidp,
					  s_regex_node *andstack, s_regex_node **andp,
					  s_regex_inst **freep, int *cursubid, int *nbra, uint32 yyrune,
					  s_regex_char_class *yyclassp, s_erc *error);

static void s_evaluntil(int pri, int **atorp, int *atorstack,
						int **subidp, s_regex_node *andstack,
						s_regex_node **andp, s_regex_inst **freep, s_erc *error);

static	s_regex* s_optimize(s_regex *pp, s_regex_inst *freep, s_erc *error);

static int s_regexec1(s_regex *progp,	/* program to run */
					  const char *string, /* string to run machine on */
					  s_regexsub *mp,	/* subexpression elements */
					  s_regex_ljunk *j,
					  s_erc *error);

static int s_regexec2(s_regex *progp,	/* program to run */
					  const char *string, /* string to run machine on */
					  s_regexsub *mp,	/* subexpression elements */
					  s_regex_ljunk *j,
					  s_erc *error);

static void s_renewmatch(s_regexsub *mp, s_regex_sub_list *sp, s_erc *error);

static s_regex_list *s_renewthread(s_regex_list *lp,	        /* _relist to add to */
								   s_regex_inst *ip,	        /* instruction to add */
								   uint8 n,
								   s_regex_sub_list *sep,
								   s_erc *error);	/* pointers to subexpressions */


static s_regex_list *s_renewemptythread(s_regex_list *lp,	/* _relist to add to */
										s_regex_inst *ip,	/* instruction to add */
										uint8 n,
										char *sp,
										s_erc *error);             /* pointers to subexpressions */

static void count_sub_matches(s_regexsub *mp, s_erc *error);

#ifdef	S_REGEX_DEBUG
static void s_regex_dumpstack(int *atorstack, int *atorp,
							  s_regex_node *andstack, s_regex_node *andp);

static void s_regex_dump(s_regex *pp);
#endif

/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

/*
 * the . metacharacter matches all characters, except newlines. (S_REGEX_ANY)
 */
S_API s_regex *s_regex_comp(const char *string, s_regex_flags flags, s_erc *error)
{
	s_regex *comp;


	S_CLR_ERR(error);

	if (string == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_regex_comp",
				  "Argument \"string\" is NULL");
		return NULL;
	}

	if (flags == S_DOT_INCLD_NEWLINE)
		comp = s_regcomp1(string, 0, S_REGEX_ANYNL, error);
	else if (flags == S_DOT_EXCLD_NEWLINE)
		comp = s_regcomp1(string, 0, S_REGEX_ANY, error);
	else
	{
		S_CTX_ERR(error,  S_ARGERROR,
				  "s_regex_comp",
				  "Argument \"flags\" is not of type s_regex_flags");
		return NULL;
	}

	if (S_CHK_ERR(error, S_CONTERR,
				  "s_regex_comp",
				  "Failed to compile regular expression for string '%s'",
				  string))
		return NULL;

	return comp;
}


S_API int s_regex_match(s_regex *rxcomp, const char *string,
						s_regexsub **rsub, s_erc *error)
{
	s_regex_ljunk j;
	s_regex_list relist0[S_REGEX_LISTSIZE];
	s_regex_list relist1[S_REGEX_LISTSIZE];
	s_regexsub *sub_matches;
	int rv;


	S_CLR_ERR(error);

	if (rxcomp == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_regex_match",
				  "Argument \"rxcomp\" is NULL");
		return 0;
	}

	if (string == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_regex_match",
				  "Argument \"string\" is NULL");
		return 0;
	}

	/*
 	 *  use user-specified starting/ending location if specified
	 */
	j.starts = (char*)string;
	j.eol = NULL;

	if (rsub != NULL)
	{
		int i;


		sub_matches = S_MALLOC(s_regexsub, S_REGEX_NSUBEXP);
		if (sub_matches == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "s_regex_match",
					  "Failed to allocate memory for s_regsub structure");
			return 0;
		}

		for (i = 0; i < S_REGEX_NSUBEXP; i++)
		{
			sub_matches->m[i].sp = NULL;
			sub_matches->m[i].ep = NULL;
		}

		sub_matches->n = S_REGEX_NSUBEXP;
		*rsub = sub_matches;
	}
	else
	{
		sub_matches = NULL;
	}

	j.starttype = 0;
	j.startchar = 0;

	if (rxcomp->start_inst->type == S_REGEX_BOL)
		j.starttype = S_REGEX_BOL;

	/* mark space */
	j.relist[0] = relist0;
	j.relist[1] = relist1;
	j.reliste[0] = relist0 + nelem(relist0) - 2;
	j.reliste[1] = relist1 + nelem(relist1) - 2;

	rv = s_regexec1(rxcomp, string, sub_matches, &j, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_regex_match",
				  "Call to \"s_regexec1\" failed"))
	{
		if (sub_matches != NULL)
			S_FREE(sub_matches);
		*rsub = NULL;
		return 0;
	}

	if (rv >= 0)
	{
		count_sub_matches(sub_matches, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_regex_match",
					  "Call to \"count_sub_matches\" failed"))
		{
			if (sub_matches != NULL)
				S_FREE(sub_matches);
			*rsub = NULL;
			return 0;
		}

		return rv;
	}

	rv = s_regexec2(rxcomp, string, sub_matches, &j, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_regex_match",
				  "Call to \"s_regexec2\" failed"))
	{
		if (sub_matches != NULL)
			S_FREE(sub_matches);
		*rsub = NULL;
		return 0;
	}

	if (rv >= 0)
	{
		count_sub_matches(sub_matches, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_regex_match",
					  "Call to \"count_sub_matches\" failed"))
		{
			if (sub_matches != NULL)
				S_FREE(sub_matches);
			*rsub = NULL;
			return 0;
		}

		return rv;
	}

	return -1;
}


S_API uint8 s_regexsub_num_groups(s_regexsub *rsub, s_erc *error)
{
	S_CLR_ERR(error);

	if (rsub == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_regexsub_num_groups",
				  "Argument \"rsub\" is NULL");
		return 0;
	}

	return rsub->n;
}


S_API char *s_regexsub_group(s_regexsub *rsub, uint8 n, s_erc *error)
{
	s_buffer *buf;
	s_regex_sub group;
	char *start_ptr;
	const uchar *buf_data;
	char *group_string;


	S_CLR_ERR(error);

	if (rsub == NULL)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_regexsub_group",
				  "Argument \"rsub\" is NULL");
		return NULL;
	}

	if (n >= rsub->n)
	{
		S_CTX_ERR(error, S_ARGERROR,
				  "s_regexsub_group",
				  "Argument \"n\" is out of bounds, range is [0, %d]", rsub->n - 1);
		return NULL;
	}

	group = rsub->m[n];
	if (group.sp == NULL)
		return NULL;


	/* whole matched string */
	if (n == 0)
	{
		group_string = s_strdup(group.sp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_regexsub_group",
					  "Call to \"s_strdup\" failed"))
			return NULL;

		return group_string;
	}


	buf = s_buffer_new(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_regexsub_group",
				  "Call to \"s_buffer_new\" failed"))
		return NULL;

	start_ptr = group.sp;

	while (start_ptr != group.ep)
	{
		uchar c;


		c = (*((unsigned char *)start_ptr++));

		s_buffer_append(buf, &c, 1, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_regexsub_group",
					  "Call to \"s_buffer_append\" failed"))
		{
			s_erc local_err = S_SUCCESS;


			s_buffer_delete(buf, &local_err);
			return NULL;
		}
	}

	buf_data = s_buffer_data(buf, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_regexsub_group",
				  "Call to \"s_buffer_data\" failed"))
	{
		s_erc local_err = S_SUCCESS;


		s_buffer_delete(buf, &local_err);
		return NULL;
	}

	group_string = s_strdup((const char*)buf_data, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_regexsub_group",
				  "Call to \"s_strdup\" failed"))
	{
		s_erc local_err = S_SUCCESS;


		s_buffer_delete(buf, &local_err);
		return NULL;
	}

	s_buffer_delete(buf, error);
	return group_string;
}


/************************************************************************************/
/*                                                                                  */
/* Static function implementations                                                  */
/*                                                                                  */
/************************************************************************************/

static void count_sub_matches(s_regexsub *mp, s_erc *error)
{
	uint8 i;

	S_CLR_ERR(error);

	if (mp == NULL)
		return;

	for (i = 0; i < mp->n; i++)
	{
		if ((mp->m[mp->n - i - 1].sp != NULL)
			|| (mp->m[mp->n - i - 1].ep != NULL))
			break;
	}

	mp->n = mp->n - i;
}


static void s_pushator(int t, int **atorp, int **subidp, int cursubid,
					   int *atorstack, s_erc *error)
{
	S_CLR_ERR(error);

	if (*atorp >= &atorstack[S_REGEX_NSTACK])
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_pushator",
				  "Operator stack overflow");
		return;
	}

	*(*atorp) = t;
	(*atorp)++;

	*(*subidp) = cursubid;
	(*subidp)++;
}


static s_bool s_utf8_forbidden(uchar octet, s_erc *error)
{
	S_CLR_ERR(error);


	switch (octet)
	{
	case 0xc0:
	case 0xc1:
	case 0xf5:
	case 0xff:
		return TRUE;
	}

	return FALSE;
}


static int s_char_to_ucs4(uint32 *ucs4, char *str, s_erc *error)
{
	int rv = 0;
	s_bool forbidden_char;


	S_CLR_ERR(error);

	if (str != NULL)
	{
		forbidden_char = s_utf8_forbidden((uchar)str[0], error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_char_to_ucs4",
					  "Call to \"s_utf8_forbidden\" failed"))
			return 0;

		if (forbidden_char)
		{
			S_CHK_ERR(error, S_FAILURE,
					  "s_char_to_ucs4",
					  "Invalid utf8 characters in string");
			return -1;
		}

		rv = s_width(str, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_char_to_ucs4",
					  "Call to \"s_width\" failed"))
			return 0;

		(*ucs4) = s_getat(str, 0, error);
		S_CHK_ERR(error, S_CONTERR,
				  "s_char_to_ucs4",
				  "Call to \"s_getat\" failed");

		return rv;
	}

	return 0;
}


static int s_nextc(uint32 *rp, s_bool *lexdone, char **exprp, s_erc *error)
{
	S_CLR_ERR(error);

	if (*lexdone)
	{
		*rp = 0;
		return 1;
	}

	*exprp += s_char_to_ucs4(rp, *exprp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_nextc",
				  "Call to \"s_char_to_ucs4\" failed"))
		return 0;

	if (*rp == '\\')
	{
		*exprp += s_char_to_ucs4(rp, *exprp, error);
		S_CHK_ERR(error, S_CONTERR,
				  "s_nextc",
				  "Call to \"s_char_to_ucs4\" failed");
		return 1;
	}

	if (*rp == 0)
		*lexdone = TRUE;

	return 0;
}

static s_regex_char_class *s_newclass(uint *nclass, s_regex_char_class *classp,
									  s_erc *error)
{
	if (*nclass >= S_REGEX_NCLASS)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_regex_char_class",
				  "Too many character classes; limit = %d",
				  S_REGEX_NCLASS);
		return NULL;
	}

	return (&(classp[(*nclass)++]));
}


static int s_bldcclass(s_regex_char_class **yyclassp, uint *nclass,
					   s_regex_char_class *classp, s_bool *lexdone,
					   char **exprp, s_erc *error)
{
	int type;
	uint32 r[S_REGEX_NCCRUNE];
	uint32 *p;
	uint32 *ep;
	uint32 *np;
	uint32 rune;
	int quoted;


	S_CLR_ERR(error);

	/* we have already seen the '[' */
	type = S_REGEX_CCLASS;
	*yyclassp = s_newclass(nclass, classp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_bldcclass",
				  "Call to \"s_newclass\" failed"))
		return 0;

	/* look ahead for negation */
	/* SPECIAL CASE!!! negated classes don't match \n */
	ep = r;
	quoted = s_nextc(&rune, lexdone, exprp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_bldcclass",
				  "Call to \"s_nextc\" failed"))
		return 0;

	if ((quoted == 0) && (rune == '^'))
	{
		type = S_REGEX_NCCLASS;
		quoted = s_nextc(&rune, lexdone, exprp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_bldcclass",
					  "Call to \"s_nextc\" failed"))
			return 0;

		*ep++ = '\n';
		*ep++ = '\n';
	}

	/* parse class into a set of spans */
	for (; ep < &r[S_REGEX_NCCRUNE];)
	{
		if (rune == 0)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "s_bldcclass",
					  "malformed '[]'");
			return 0;
		}

		if ((quoted == 0) && (rune == ']'))
			break;

		if ((quoted == 0) && (rune == '-'))
		{
			if (ep == r)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "s_bldcclass",
						  "malformed '[]'");
				return 0;
			}

			quoted = s_nextc(&rune, lexdone, exprp, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_bldcclass",
						  "Call to \"s_nextc\" failed"))
				return 0;

			if (((quoted == 0) && (rune == ']')) || rune == 0)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "s_bldcclass",
						  "malformed '[]'");
				return 0;
			}

			*(ep-1) = rune;
		}
		else
		{
			*ep++ = rune;
			*ep++ = rune;
		}

		quoted = s_nextc(&rune, lexdone, exprp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_bldcclass",
					  "Call to \"s_nextc\" failed"))
			return 0;
	}

	/* sort on span start */
	for (p = r; p < ep; p += 2)
	{
		for (np = p; np < ep; np += 2)
			if (*np < *p)
			{
				rune = np[0];
				np[0] = p[0];
				p[0] = rune;
				rune = np[1];
				np[1] = p[1];
				p[1] = rune;
			}
	}

	/* merge spans */
	np = (*yyclassp)->spans;
	p = r;

	if (r == ep)
		(*yyclassp)->end = np;
	else
	{
		np[0] = *p++;
		np[1] = *p++;

		for (; p < ep; p += 2)
			if (p[0] <= np[1])
			{
				if (p[1] > np[1])
					np[1] = p[1];
			}
			else
			{
				np += 2;
				np[0] = p[0];
				np[1] = p[1];
			}
		(*yyclassp)->end = np+2;
	}

	return type;
}



static int s_lex(int literal, int dot_type, uint32 *yyrune,
				 s_bool *lexdone, char **exprp, s_regex_char_class **yyclassp,
				 uint *nclass, s_regex_char_class *classp,
				 s_erc *error)
{
	int quoted;
	int rv;


	S_CLR_ERR(error);

	quoted = s_nextc(yyrune, lexdone, exprp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_lex",
				  "Call to \"s_nextc\" failed"))
		return S_REGEX_END;

	if (literal || quoted)
	{
		if (*yyrune == 0)
			return S_REGEX_END;
		return S_REGEX_RUNE;
	}

	switch(*yyrune)
	{
	case 0:
		return S_REGEX_END;
	case '*':
		return S_REGEX_STAR;
	case '?':
		return S_REGEX_QUEST;
	case '+':
		return S_REGEX_PLUS;
	case '|':
		return S_REGEX_OR;
	case '.':
		return dot_type;
	case '(':
		return S_REGEX_LBRA;
	case ')':
		return S_REGEX_RBRA;
	case '^':
		return S_REGEX_BOL;
	case '$':
		return S_REGEX_EOL;
	case '[':
	{
		rv = s_bldcclass(yyclassp, nclass, classp, lexdone,
						 exprp, error);
		S_CHK_ERR(error, S_CONTERR,
				  "s_lex",
				  "Call to \"s_bldcclass\" failed");
		return rv;
	}
	}

	return S_REGEX_RUNE;
}


static int s_popator(int **atorp, int *atorstack, int **subidp,
					 s_erc *error)
{
	S_CLR_ERR(error);

	if (*atorp <= &atorstack[0])
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_popator",
				  "Operator stack underflow");
		return 0;
	}

	--(*subidp);
	--(*atorp);

	return *(*atorp);
}


static s_regex_node *s_popand(s_regex_node *andstack, s_regex_node **andp,
							  int op, s_erc *error)
{
	S_CLR_ERR(error);

	if (*andp <= &andstack[0])
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_popand",
				  "Missing operand for '%c'", op);
		return NULL;
	}

	return (--*andp);
}


static s_regex_inst *s_newinst(int t, s_regex_inst **freep, s_erc *error)
{
	S_CLR_ERR(error);

	(*freep)->type = t;
	(*freep)->u2.left = 0;
	(*freep)->u1.right = 0;

	return (*freep)++;
}


static void s_pushand(s_regex_inst *f, s_regex_inst *l,
					  s_regex_node *andstack,
					  s_regex_node **andp,
					  s_erc *error)
{
	S_CLR_ERR(error);

	if (*andp >= &andstack[S_REGEX_NSTACK])
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_pushand",
				  "Operand stack overflow");
		return;
	}

	(*andp)->first = f;
	(*andp)->last = l;
	(*andp)++;
}


static void s_evaluntil(int pri, int **atorp, int *atorstack,
						int **subidp, s_regex_node *andstack,
						s_regex_node **andp, s_regex_inst **freep, s_erc *error)
{
	s_regex_node *op1;
	s_regex_node *op2;
	s_regex_inst *inst1;
	s_regex_inst *inst2;
	int rv;

	S_CLR_ERR(error);


	while ((pri == S_REGEX_RBRA) || ((*atorp)[-1] >= pri))
	{
		rv = s_popator(atorp, atorstack, subidp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_evaluntil",
					  "Call to \"s_popator\" failed"))
			return;

		switch (rv)
		{
		default:
			S_CTX_ERR(error, S_FAILURE,
					  "s_evaluntil",
					  "Unknown operator in evaluntil");
			break;
		case S_REGEX_LBRA:		/* must have been RBRA */
			op1 = s_popand(andstack, andp, '(', error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_popand\" failed"))
				return;

			inst2 = s_newinst(S_REGEX_RBRA, freep, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_newinst\" failed"))
				return;

			inst2->u1.subid = **subidp;
			op1->last->u2.next = inst2;

			inst1 = s_newinst(S_REGEX_LBRA,  freep, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_newinst\" failed"))
				return;

			inst1->u1.subid = **subidp;
			inst1->u2.next = op1->first;

			s_pushand(inst1, inst2, andstack, andp, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_pushand\" failed"))
				return;

			return;
		case S_REGEX_OR:
			op2 = s_popand(andstack, andp, '|', error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_popand\" failed"))
				return;

			op1 = s_popand(andstack, andp, '|', error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_popand\" failed"))
				return;

			inst2 = s_newinst(S_REGEX_NOP, freep, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_newinst\" failed"))
				return;

			op2->last->u2.next = inst2;
			op1->last->u2.next = inst2;

			inst1 = s_newinst(S_REGEX_OR, freep, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_newinst\" failed"))
				return;

			inst1->u1.right = op1->first;
			inst1->u2.left = op2->first;

			s_pushand(inst1, inst2, andstack, andp, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_pushand\" failed"))
				return;

			break;
		case S_REGEX_CAT:
			op2 = s_popand(andstack, andp, 0, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_popand\" failed"))
				return;

			op1 = s_popand(andstack, andp, 0, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_popand\" failed"))
				return;

			op1->last->u2.next = op2->first;

			s_pushand(op1->first, op2->last, andstack, andp, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_pushand\" failed"))
				return;

			break;
		case S_REGEX_STAR:
			op2 = s_popand(andstack, andp, '*', error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_popand\" failed"))
				return;

			inst1 = s_newinst(S_REGEX_OR, freep, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_newinst\" failed"))
				return;

			op2->last->u2.next = inst1;
			inst1->u1.right = op2->first;

			s_pushand(inst1, inst1, andstack, andp, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_pushand\" failed"))
				return;

			break;
		case S_REGEX_PLUS:
			op2 = s_popand(andstack, andp, '+', error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_popand\" failed"))
				return;

			inst1 = s_newinst(S_REGEX_OR, freep, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_newinst\" failed"))
				return;

			op2->last->u2.next = inst1;
			inst1->u1.right = op2->first;

			s_pushand(op2->first, inst1, andstack, andp, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_pushand\" failed"))
				return;

			break;
		case S_REGEX_QUEST:
			op2 = s_popand(andstack, andp, '?', error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_popand\" failed"))
				return;

			inst1 = s_newinst(S_REGEX_OR, freep, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_newinst\" failed"))
				return;

			inst2 = s_newinst(S_REGEX_NOP, freep, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_newinst\" failed"))
				return;

			inst1->u2.left = inst2;
			inst1->u1.right = op2->first;
			op2->last->u2.next = inst2;

			s_pushand(inst1, inst2, andstack, andp, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_evaluntil",
						  "Call to \"s_pushand\" failed"))
				return;

			break;
		}
	}
}


static void s_operator(int t, int *nbra, int *cursubid, s_bool *lastwasand,
					   int **atorp, int *atorstack, int **subidp,
					   s_regex_node *andstack, s_regex_node **andp,
					   s_regex_inst **freep, s_erc *error)
{
	S_CLR_ERR(error);


	if ((t == S_REGEX_RBRA) && (--(*nbra) < 0))
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_operator",
				  "unmatched right parenthesis");
		return;
	}

	if (t == S_REGEX_LBRA)
	{
		if (++(*cursubid) >= S_REGEX_NSUBEXP)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "s_operator",
					  "too many sub-expressions");
			return;
		}

		(*nbra)++;

		if (*lastwasand == TRUE)
		{
			s_operator(S_REGEX_CAT, nbra, cursubid, lastwasand, atorp,
					   atorstack, subidp, andstack, andp, freep, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_operator",
						  "Recursive call to \"s_operator\" failed"))
				return;
		}
	}
	else
	{
		s_evaluntil(t, atorp, atorstack, subidp,
					andstack, andp, freep, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_operator",
					  "Call to \"s_evaluntil\" failed"))
			return;
	}

	if (t != S_REGEX_RBRA)
	{
		s_pushator(t, atorp, subidp, *cursubid, atorstack, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_operator",
					  "Call to \"s_pushator\" failed"))
			return;
	}

	*lastwasand = FALSE;

	if ((t == S_REGEX_STAR)
	    || (t == S_REGEX_QUEST)
	    || (t == S_REGEX_PLUS)
	    || (t == S_REGEX_RBRA))
		*lastwasand = TRUE;	/* these look like operands */
}


static void s_operand(int t, s_bool *lastwasand, int **atorp,
					  int *atorstack, int **subidp,
					  s_regex_node *andstack, s_regex_node **andp,
					  s_regex_inst **freep, int *cursubid, int *nbra, uint32 yyrune,
					  s_regex_char_class *yyclassp, s_erc *error)
{
	s_regex_inst *i;
	S_CLR_ERR(error);


	if (*lastwasand == TRUE)
	{
		/* catenate is implicit */
		s_operator(S_REGEX_CAT, nbra, cursubid, lastwasand, atorp,
				   atorstack, subidp, andstack, andp, freep, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_operand",
					  "Call to \"s_operator\" failed"))
			return;
	}

	i = s_newinst(t, freep, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_operand",
				  "Call to \"s_newinst\" failed"))
		return;

	if ((t == S_REGEX_CCLASS) || (t == S_REGEX_NCCLASS))
		i->u1.cp = yyclassp;

	if (t == S_REGEX_RUNE)
		i->u1.r = yyrune;

	s_pushand(i, i, andstack, andp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_operand",
				  "Call to \"s_pushand\" failed"))
		return;

	*lastwasand = TRUE;
}


static s_regex* s_optimize(s_regex *pp, s_regex_inst *freep, s_erc *error)
{
	s_regex_inst *inst;
	s_regex_inst *target;
	size_t size;
	s_regex *npp;
	s_regex_char_class *cl;
	int diff;


	S_CLR_ERR(error);

	/*
	 *  get rid of NOOP chains
	 */
	for (inst = pp->first_inst; inst->type != S_REGEX_END; inst++)
	{
		target = inst->u2.next;

		while (target->type == S_REGEX_NOP)
			target = target->u2.next;

		inst->u2.next = target;
	}

	/*
	 *  The original allocation is for an area larger than
	 *  necessary.  Reallocate to the actual space used
	 *  and then relocate the code.
	 */
	size = sizeof(s_regex) + (freep - pp->first_inst) * sizeof(s_regex_inst);

	npp = S_REALLOC_SIZE(pp, size);
	if (npp == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_optimize",
				  "Failed to allocate memory for optimized regex structure");
		return NULL;
	}

	if (npp == pp)
		return pp;

	diff = (char *)npp - (char *)pp;
	freep = (s_regex_inst*)((char *)freep + diff);

	for (inst = npp->first_inst; inst < freep; inst++)
	{
		switch (inst->type)
		{
		case S_REGEX_OR:
		case S_REGEX_STAR:
		case S_REGEX_PLUS:
		case S_REGEX_QUEST:
			inst->u1.right = (void*)((char*)inst->u1.right + diff);
			break;
		case S_REGEX_CCLASS:
		case S_REGEX_NCCLASS:
			inst->u1.right = (void*)((char*)inst->u1.right + diff);
			cl = inst->u1.cp;
			cl->end = (void*)((char*)cl->end + diff);
			break;
		}
		inst->u2.left = (void*)((char*)inst->u2.left + diff);
	}
	npp->start_inst = (void*)((char*)npp->start_inst + diff);

	return npp;
}


#ifdef	S_REGEX_DEBUG
static void s_regex_dumpstack(int *atorstack, int *atorp,
							  s_regex_node *andstack, s_regex_node *andp)
{
	s_regex_node *stk;
	int *ip;

	printf("operators\n");

	for (ip = atorstack; ip < atorp; ip++)
		printf("0%o\n", *ip);

	printf("operands\n");

	for (stk = andstack; stk < andp; stk++)
		printf("0%o\t0%o\n", stk->first->type, stk->last->type);
}

static void s_regex_dump(s_regex *pp)
{
	s_regex_inst *l;
	uint32 *p;

	l = pp->first_inst;

	do
	{
		printf("%d:\t0%o\t%d\t%d", l-pp->first_inst, l->type,
			   l->u2.left-pp->first_inst, l->u1.right-pp->first_inst);

		if (l->type == S_REGEX_RUNE)
			printf("\t%c\n", l->u1.r);

		else if ((l->type == S_REGEX_CCLASS) || (l->type == S_REGEX_NCCLASS))
		{
			printf("\t[");

			if (l->type == S_REGEX_NCCLASS)
				printf("^");

			for (p = l->u1.cp->spans; p < l->u1.cp->end; p += 2)
				if (p[0] == p[1])
					printf("%c", p[0]);
				else
					printf("%c-%c", p[0], p[1]);
			printf("]\n");
		}
		else
			printf("\n");
	} while (l++->type);
}
#endif


static s_regex *s_regcomp1(const char *s, int literal, int dot_type, s_erc *error)
{
	int token;
	s_regex *pp;
	size_t ssize;
	s_regex_inst *freep;
	s_regex_char_class *classp;
	s_bool lexdone;
	char *exprp;                             /* pointer to next character in source expression */
	uint nclass;
	int nbra;
	int *atorp;
	s_regex_node *andp;
	int *subidp;
	s_bool lastwasand;                       /* Last token was operand */
	int atorstack[S_REGEX_NSTACK];
	int subidstack[S_REGEX_NSTACK];	         /* parallel to atorstack */
	s_regex_node andstack[S_REGEX_NSTACK];
	int cursubid;                            /* id of current sub-expression */
	int counter;
	uint32 yyrune;
	s_regex_char_class *yyclassp;	/* last lex'd class */


	S_CLR_ERR(error);

	/****** initialize values ********/
	for (counter = 0; counter < S_REGEX_NSTACK; counter++)
	{
		atorstack[counter] = 0;
		subidstack[counter] = 0;
		andstack[counter].first = NULL;
		andstack[counter].last = NULL;
	}

	ssize = s_strzsize(s, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_regcomp1",
				  "Call to \"s_strzsize\" failed"))
		return NULL;

	/* get memory for the program */
	pp = S_MALLOC(char, sizeof(s_regex) + 6 * sizeof(s_regex_inst) * ssize);
	if (pp == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_regcomp1",
				  "Failed to allocate memory for regex");
		return NULL;
	}

	freep = pp->first_inst;
	classp = pp->class;

	/* compile the regular expression */
	lexdone = FALSE;
	exprp = (char*)s;
	nclass = 0;
	nbra = 0;
	atorp = atorstack;
	andp = andstack;
	subidp = subidstack;
	lastwasand = FALSE;
	cursubid = 0;
	yyclassp = NULL;


	/*********************************/


	/* Start with a low priority operator to prime parser */
	s_pushator(S_REGEX_START - 1, &atorp, &subidp, cursubid,
			   atorstack, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_regcomp1",
				  "Call to \"s_pushator\" failed"))
	{
		S_FREE(pp);
		return NULL;
	}

	token = s_lex(literal, dot_type, &yyrune, &lexdone,
				  &exprp, &yyclassp, &nclass, classp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_regcomp1",
				  "Call to \"s_lex\" failed"))
	{
		S_FREE(pp);
		return NULL;
	}

	while (token != S_REGEX_END)
	{
		if ((token & 0300) == S_REGEX_OPERATOR)
		{
			s_operator(token, &nbra, &cursubid, &lastwasand, &atorp,
					   atorstack, &subidp, andstack, &andp, &freep, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_regcomp1",
						  "Call to \"s_operator\" failed"))
			{
				S_FREE(pp);
				return NULL;
			}
		}
		else
		{
			s_operand(token, &lastwasand, &atorp, atorstack, &subidp,
					  andstack, &andp, &freep, &cursubid, &nbra, yyrune,
					  yyclassp, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_regcomp1",
						  "Call to \"s_operand\" failed"))
			{
				S_FREE(pp);
				return NULL;
			}
		}

		token = s_lex(literal, dot_type, &yyrune, &lexdone,
					  &exprp, &yyclassp, &nclass, classp, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_regcomp1",
					  "Call to \"s_lex\" failed"))
		{
			S_FREE(pp);
			return NULL;
		}
	}

	/* Close with a low priority operator */
	s_evaluntil(S_REGEX_START, &atorp, atorstack, &subidp,
				andstack, &andp, &freep, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_regcomp1",
				  "Call to \"s_evaluntil\" failed"))
	{
		S_FREE(pp);
		return NULL;
	}

	/* Force END */
	s_operand(S_REGEX_END,  &lastwasand, &atorp, atorstack, &subidp,
			  andstack, &andp, &freep, &cursubid, &nbra, yyrune,
			  yyclassp, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_regcomp1",
				  "Call to \"s_operand\" failed"))
	{
		S_FREE(pp);
		return NULL;
	}

	s_evaluntil(S_REGEX_START, &atorp, atorstack, &subidp,
				andstack, &andp, &freep, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_regcomp1",
				  "Call to \"s_evaluntil\" failed"))
	{
		S_FREE(pp);
		return NULL;
	}

#ifdef S_REGEX_DEBUG
	s_regex_dumpstack(atorstack, atorp, andstack, andp);
#endif

	if (nbra)
	{
		S_CTX_ERR(error, S_FAILURE,
				  "s_regcomp1",
				  "Unmatched left parenthesis");
		S_FREE(pp);
		return NULL;
	}

	--andp;	/* points to first and only operand */

	/* ABY: strict compile gives : error: array subscript is below array bounds */
	pp->start_inst = andp->first;

	pp = s_optimize(pp, freep, error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "s_regcomp1",
				  "Call to \"s_optimize\" failed"))
	{
		S_FREE(pp);
		return NULL;
	}

#ifdef S_REGEX_DEBUG
	printf("start: 0x%x\n", andp->first - pp->first_inst);
	s_regex_dump(pp);
#endif

	return pp;
}


/*
 *  return	0 if no match
 *		>0 if a match
 *		<0 if we ran out of _relist space
 */
static int s_regexec1(s_regex *progp,	/* program to run */
					  const char *string, /* string to run machine on */
					  s_regexsub *mp,	/* subexpression elements */
					  s_regex_ljunk *j,
					  s_erc *error)
{
	int flag;
	s_regex_inst *inst;
	s_regex_list *tlp;
	char *s;
	int checkstart;
	uint32 r;
	uint32 *rp;
	uint32 *ep;
	int n;
	s_regex_list *tl;	/* This list, next list */
	s_regex_list *nl;
	s_regex_list *tle;	/* ends of this and next list */
	s_regex_list *nle;
	int match;
	char *p;
	uint8 num_match;


	S_CLR_ERR(error);

	flag = 0;
	match = 0;
	checkstart = j->starttype;

	j->relist[0][0].inst = NULL;
	j->relist[1][0].inst = NULL;

	if (mp != NULL)
		num_match = mp->n;
	else
		num_match = 0;

	/* Execute machine once for each character, including terminal NUL */
	s = j->starts;
	do
	{
		/* fast check for first char */
		if (checkstart != 0)
		{
			if (s_strcmp(s, string, error) != 0)
			{
				p = (char*)s_strrchr(s, '\n', error);
				if (S_CHK_ERR(error, S_CONTERR,
							  "s_regexec1",
							  "Call to \"s_strrchr\" failed"))
					return 0;

				if ((p == NULL) || (s == j->eol))
					return match;

				s = p+1;
			}
		}

		r = s_getc(s, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_regexec1",
					  "Call to \"s_getc\" failed"))
			return 0;

		n = s_cwidth(r, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "s_regexec1",
					  "Call to \"s_cwidth\" failed"))
			return 0;

		/* switch run lists */
		tl = j->relist[flag];
		tle = j->reliste[flag];
		nl = j->relist[flag ^= 1];
		nle = j->reliste[flag];
		nl->inst = 0;

		/* Add first instruction to current list */
		if (match == 0)
		{
			s_renewemptythread(tl, progp->start_inst, num_match, s, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "s_regexec1",
						  "Call to \"s_renewemptythread\" failed"))
				return 0;
		}

		/* Execute machine until current list is empty */
		for (tlp = tl; tlp->inst; tlp++)
		{	/* assignment = */
			for (inst = tlp->inst; ; inst = inst->u2.next)
			{
				switch(inst->type)
				{
				case S_REGEX_RUNE:	/* regular character */
					if (inst->u1.r == r)
					{
						s_regex_list *tmp;

						tmp = s_renewthread(nl, inst->u2.next, num_match, &tlp->se, error);
						if (S_CHK_ERR(error, S_CONTERR,
									  "s_regexec1",
									  "Call to \"s_renewthread\" failed"))
							return 0;

						if (tmp == nle)
							return -1;
					}
					break;
				case S_REGEX_LBRA:
					tlp->se.m[inst->u1.subid].sp = s;
					continue;
				case S_REGEX_RBRA:
					tlp->se.m[inst->u1.subid].ep = s;
					continue;
				case S_REGEX_ANY:
					if (r != '\n')
					{
						s_regex_list *tmp;

						tmp = s_renewthread(nl, inst->u2.next, num_match, &tlp->se, error);
						if (S_CHK_ERR(error, S_CONTERR,
									  "s_regexec1",
									  "Call to \"s_renewthread\" failed"))
							return 0;

						if (tmp == nle)
							return -1;
					}
					break;
				case S_REGEX_ANYNL:
				{
					s_regex_list *tmp;

					tmp = s_renewthread(nl, inst->u2.next, num_match, &tlp->se, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "s_regexec1",
								  "Call to \"s_renewthread\" failed"))
						return 0;

					if (tmp == nle)
						return -1;
					break;
				}
				case S_REGEX_BOL:
					if (s == string || *(s-1) == '\n')
						continue;
					break;
				case S_REGEX_EOL:
					if (s == j->eol || r == 0 || r == '\n')
						continue;
					break;
				case S_REGEX_CCLASS:
					ep = inst->u1.cp->end;
					for (rp = inst->u1.cp->spans; rp < ep; rp += 2)
						if (r >= rp[0] && r <= rp[1])
						{
							s_regex_list *tmp;

							tmp = s_renewthread(nl, inst->u2.next, num_match, &tlp->se, error);
							if (S_CHK_ERR(error, S_CONTERR,
										  "s_regexec1",
										  "Call to \"s_renewthread\" failed"))
								return 0;

							if (tmp == nle)
								return -1;

							break;
						}
					break;
				case S_REGEX_NCCLASS:
					ep = inst->u1.cp->end;
					for (rp = inst->u1.cp->spans; rp < ep; rp += 2)
						if (r >= rp[0] && r <= rp[1])
							break;
					if (rp == ep)
					{
						s_regex_list *tmp;

						tmp = s_renewthread(nl, inst->u2.next, num_match, &tlp->se, error);
						if (S_CHK_ERR(error, S_CONTERR,
									  "s_regexec1",
									  "Call to \"s_renewthread\" failed"))
							return 0;

						if (tmp == nle)
							return -1;
					}
					break;
				case S_REGEX_OR:
					/* evaluate right choice later */
				{
					s_regex_list *tmp;

					tmp = s_renewthread(tl, inst->u1.right, num_match, &tlp->se, error);
					if (S_CHK_ERR(error, S_CONTERR,
								  "s_regexec1",
								  "Call to \"s_renewthread\" failed"))
						return 0;

					if (tmp == tle)
						return -1;

					/* efficiency: advance and re-evaluate */
					continue;
				}
				case S_REGEX_END:	/* Match! */
					match = 1;
					tlp->se.m[0].ep = s;

					if (mp != NULL)
					{
						s_renewmatch(mp, &tlp->se, error);
						if (S_CHK_ERR(error, S_CONTERR,
									  "s_regexec1",
									  "Call to \"s_renewmatch\" failed"))
							return 0;
					}
					break;
				}
				break;
			}
		}
		if (s == j->eol)
			break;
		checkstart = j->starttype && nl->inst==0;
		s += n;
	}
	while (r != 0);

	return match;
}


static int s_regexec2(s_regex *progp,	/* program to run */
					  const char *string, /* string to run machine on */
					  s_regexsub *mp,	/* subexpression elements */
					  s_regex_ljunk *j,
					  s_erc *error)
{
	int rv;
	s_regex_list *relist0;
	s_regex_list *relist1;

	S_CLR_ERR(error);

	/* mark space */
	relist0 = S_MALLOC(s_regex_list, S_REGEX_BIGLISTSIZE);

	if (relist0 == NULL)
	{
		S_FTL_ERR(error, S_MEMERROR,
				  "s_regexec2",
				  "Failed to allocate memory to mark space");
		return -1;
	}

	relist1 = S_MALLOC(s_regex_list, S_REGEX_BIGLISTSIZE);

 	if (relist1 == NULL)
	{
		S_FREE(relist0);
		S_FTL_ERR(error, S_MEMERROR,
				  "s_regexec2",
				  "Failed to allocate memory to mark space");
		return -1;
	}

	j->relist[0] = relist0;
	j->relist[1] = relist1;
	j->reliste[0] = relist0 + S_REGEX_BIGLISTSIZE - 2;
	j->reliste[1] = relist1 + S_REGEX_BIGLISTSIZE - 2;

	rv = s_regexec1(progp, string, mp, j, error);
	S_CHK_ERR(error, S_CONTERR,
			  "s_regexec2",
			  "Call to \"s_regexec1\" failed");

	S_FREE(relist0);
	S_FREE(relist1);

	return (rv);
}


/*
 *  save a new match in mp
 */
static void s_renewmatch(s_regexsub *mp, s_regex_sub_list *sp, s_erc *error)
{
	int i;


	S_CLR_ERR(error);

	if (mp == NULL)
		return;

	if ((mp->m[0].sp == NULL)
	    || (sp->m[0].sp < mp->m[0].sp)
	    || ((sp->m[0].sp == mp->m[0].sp)
			&& (sp->m[0].ep > mp->m[0].ep)))
	{
		for (i = 0; i < mp->n && i < S_REGEX_NSUBEXP; i++)
			mp->m[i] = sp->m[i];

		for(; i < mp->n; i++)
		{
			mp->m[i].sp = NULL;
			mp->m[i].ep = NULL;
		}
	}
}


/*
 * Note optimization in _renewthread:
 * 	*lp must be pending when _renewthread called; if *l has been looked
 *		at already, the optimization is a bug.
 */
static s_regex_list *s_renewthread(s_regex_list *lp,	        /* _relist to add to */
								   s_regex_inst *ip,	        /* instruction to add */
								   uint8 n,
								   s_regex_sub_list *sep,
								   s_erc *error)	/* pointers to subexpressions */
{
	s_regex_list *p;


	S_CLR_ERR(error);

	for (p = lp; p->inst; p++)
	{
		if (p->inst == ip)
		{
			if (sep->m[0].sp < p->se.m[0].sp)
			{
				if (n > 1)
					p->se = *sep;
				else
					p->se.m[0] = sep->m[0];
			}
			return NULL;
		}
	}
	p->inst = ip;

	if (n > 1)
		p->se = *sep;
	else
		p->se.m[0] = sep->m[0];

	(++p)->inst = 0;

	return p;
}


/*
 * same as renewthread, but called with
 * initial empty start pointer.
 */
static s_regex_list *s_renewemptythread(s_regex_list *lp,	/* _relist to add to */
										s_regex_inst *ip,	/* instruction to add */
										uint8 n,
										char *sp,
										s_erc *error)              /* pointers to subexpressions */
{
	s_regex_list *p;


	S_CLR_ERR(error);

	for (p = lp; p->inst; p++)
	{
		if (p->inst == ip)
		{
			if (sp < p->se.m[0].sp)
			{
				if (n > 1)
					memset(&p->se, 0, sizeof(p->se));
				p->se.m[0].sp = sp;
			}
			return NULL;
		}
	}
	p->inst = ip;

	if (n > 1)
		memset(&p->se, 0, sizeof(p->se));

	p->se.m[0].sp = sp;

	(++p)->inst = 0;

	return p;
}
