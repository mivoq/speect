/************************************************************************************/
/* Copyright (c) 2009 The Department of Arts and Culture,                           */
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
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A g2p rewrites rule class implementation.                                        */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "g2p_rewrites_rule.h"


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

static SG2PRewritesRuleClass G2PRewritesRuleClass; /* SG2PRewritesRule class declaration. */


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/


/* local functions to register and free classes */
S_LOCAL void _s_g2p_rewrites_rule_class_reg(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_reg(&G2PRewritesRuleClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_g2p_rewrites_rule_class_reg",
			  "Failed to register SG2PRewritesRuleClass");
}


S_LOCAL void _s_g2p_rewrites_rule_class_free(s_erc *error)
{
	S_CLR_ERR(error);
	s_class_free(&G2PRewritesRuleClass, error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_g2p_rewrites_rule_class_free",
			  "Failed to free SG2PRewritesRuleClass");
}


/************************************************************************************/
/*                                                                                  */
/* Static class function implementations                                            */
/*                                                                                  */
/************************************************************************************/

static void Init(void *obj, s_erc *error)
{
	SG2PRewritesRule *self = obj;


	S_CLR_ERR(error);
	self->left_context = NULL;
	self->right_context = NULL;
	self->phone = NULL;
}



static void Destroy(void *obj, s_erc *error)
{
	SG2PRewritesRule *self = obj;


	S_CLR_ERR(error);
	if (self->left_context != NULL)
		S_FREE(self->left_context);

	if (self->right_context != NULL)
		S_FREE(self->right_context);

	if (self->phone != NULL)
		S_FREE(self->phone);
}


static void Dispose(void *obj, s_erc *error)
{
	S_CLR_ERR(error);
	SObjectDecRef(obj);
}


static s_bool RuleMatches(const SG2PRewritesRule *self, const char **phone,
						  const char *word_lc, const char *word_rc, s_erc *error)
{
	uint32 word_char;
	uint32 rule_char;
	uint index;


	S_CLR_ERR(error);
	*phone = NULL;

	/* test left context of word against left context rule */
	index = 0;
	do
	{
		word_char = s_getat(word_lc, index, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "RuleMatches",
					  "Call to \"s_getat\" failed"))
			return FALSE;

		rule_char = s_getat(self->left_context, index++, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "RuleMatches",
					  "Call to \"s_getat\" failed"))
			return FALSE;

		if (rule_char == 0)
			break; /*reached end of rule, still a match */

		if (word_char != rule_char)
			return FALSE;
	} while ((word_char != 0) && (rule_char != 0));

	/* test right context of word against right context rule */
	index = 0;
	do
	{
		word_char = s_getat(word_rc, index, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "RuleMatches",
					  "Call to \"s_getat\" failed"))
			return FALSE;

		rule_char = s_getat(self->right_context, index++, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "RuleMatches",
					  "Call to \"s_getat\" failed"))
			return FALSE;

		if (rule_char == 0)
			break; /*reached end of rule, still a match */

		if (word_char != rule_char)
			return FALSE;
	} while ((word_char != 0) && (rule_char != 0));

	/* got here so rule matches */
	*phone = self->phone;

	return TRUE;
}

/************************************************************************************/
/*                                                                                  */
/* SG2PRewritesRule class initialization                                            */
/*                                                                                  */
/************************************************************************************/

static SG2PRewritesRuleClass G2PRewritesRuleClass =
{
	/* SObjectClass */
	{
		"SG2PRewritesRule",
		sizeof(SG2PRewritesRule),
		{ 0, 1},
		Init,            /* init    */
		Destroy,         /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SG2PRewritesRuleClass */
	RuleMatches          /* rule_matches */
};
