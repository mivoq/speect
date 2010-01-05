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

#ifndef _SPCT_PLUGIN_G2P_REWRITES_RULE_H__
#define _SPCT_PLUGIN_G2P_REWRITES_RULE_H__


/**
 * @file g2p_rewrites_rule.h
 * A g2p rewrites rule class implementation.
 */


/**
 * @ingroup SG2PRewrites G2P Rewrites Rule
 * A g2p rewrites rule class implementation.
 *
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "speect.h"


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
 * @hideinitializer
 * Return the given #SG2PRewritesRule child/parent class object as a
 * #SG2PRewritesRule object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SG2PRewritesRule* type.
 * @note This casting is not safety checked.
 */
#define S_G2PREWRITES_RULE(SELF)    ((SG2PRewritesRule *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SG2PRewritesRule,
 * see full description #S_G2PREWRITES_RULE_CALL for usage.
 *
 * @param SELF The given #SG2PRewritesRule*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_G2PREWRITES_RULE_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_G2PREWRITES_RULE_CALL(SELF, FUNC)					\
	((SG2PRewritesRuleClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SG2PRewritesRule
 * can be called.
 *
 * @param SELF The given #SG2PRewritesRule*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_G2PREWRITES_RULE_METH_VALID(SELF, FUNC)		\
	S_G2PREWRITES_RULE_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* SG2PRewritesRule definition                                                      */
/*                                                                                  */
/************************************************************************************/

/**
 * The SG2PRewritesRule structure.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject     obj;

	/**
	 * @protected Left context.
	 */
	char *left_context;

	/**
	 * @protected Right context.
	 */
	char *right_context;

	/**
	 * @protected Rule phone.
	 */
	char *phone;
} SG2PRewritesRule;


/************************************************************************************/
/*                                                                                  */
/* SG2PRewritesRuleClass definition                                                 */
/*                                                                                  */
/************************************************************************************/

/**
 * The SG2PRewritesRuleClass structure.
 * @extends SObjectClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SObjectClass.
	 */
	SObjectClass _inherit;

	/* Class methods */
	/**
	 * Query if the given rule matches the given word left context and
	 * right context.
	 *
	 * @param self The given rule.
	 * @param phone The phone of the given rule. The phone is set when
	 * a match occurs, if not matched then set to @c NULL.
	 * @param word_lc The word left context, that is the context of the
	 * word to the left of the grapheme currently being tested
	 * for. The left context must be swapped to make matching easier,
	 * i.e. if the context is <em> #car </em> then the @c word_lc must
	 * be <em> rac# </em>.
	 * @param word_rc The word right context, that is the context of the
	 * word to the right of the grapheme currently being tested
	 * for. The context is in normal left-to-right fashion.
	 * @param error Error code.
	 *
	 * @return @c TRUE if the rule matches, else @c FALSE.
	 */
	s_bool  (*rule_matches)(const SG2PRewritesRule *self, const char **phone,
							const char *word_lc, const char *word_rc, s_erc *error);
} SG2PRewritesRuleClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SG2PRewritesRule plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_g2p_rewrites_rule_class_reg(s_erc *error);

/**
 * Free the #SG2PRewritesRule plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_g2p_rewrites_rule_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_G2P_REWRITES_RULE_H__ */
