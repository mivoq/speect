/************************************************************************************/
/* Copyright (c) 2011 The Department of Arts and Culture,                           */
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
/* DATE    : May 2011                                                               */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A syllabification rewrites rule class implementation.                            */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_SYLLABIFICATION_REWRITES_RULE_H__
#define _SPCT_PLUGIN_SYLLABIFICATION_REWRITES_RULE_H__


/**
 * @file syllab_rewrites_rule.h
 * A syllabification rewrites rule class implementation.
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
 * Return the given #SSyllabificationRewritesRule child/parent class object as a
 * #SSyllabificationRewritesRule object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SSyllabificationRewritesRule* type.
 * @note This casting is not safety checked.
 */
#define S_SYLLABIFICATIONREWRITES_RULE(SELF)    ((SSyllabificationRewritesRule *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SSyllabificationRewritesRule,
 * see full description #S_SYLLABIFICATIONREWRITES_RULE_CALL for usage.
 *
 * @param SELF The given #SSyllabificationRewritesRule*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_SYLLABIFICATIONREWRITES_RULE_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_SYLLABIFICATIONREWRITES_RULE_CALL(SELF, FUNC)					\
	((SSyllabificationRewritesRuleClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SSyllabificationRewritesRule
 * can be called.
 *
 * @param SELF The given #SSyllabificationRewritesRule*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_SYLLABIFICATIONREWRITES_RULE_METH_VALID(SELF, FUNC)		\
	S_SYLLABIFICATIONREWRITES_RULE_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* SSyllabificationRewritesRule definition                                          */
/*                                                                                  */
/************************************************************************************/

/**
 * The SSyllabificationRewritesRule structure.
 * Rules are defined as: LC [ A ] RC => B
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject obj;

	/**
	 * @protected Left context.
	 */
	s_str_list *LC;

	/**
	 * @protected Context.
	 */
	s_str_list *A;

	/**
	 * @protected Right context.
	 */
	s_str_list *RC;

	/**
	 * @protected Replacement.
	 */
	s_str_list *B;
} SSyllabificationRewritesRule;


/************************************************************************************/
/*                                                                                  */
/* SSyllabificationRewritesRuleClass definition                                     */
/*                                                                                  */
/************************************************************************************/

/**
 * The SSyllabificationRewritesRuleClass structure.
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
	 * Get the rule replacement.
	 *
	 * @param self The given rule.
	 * @param error Error code.
	 *
	 * @return The rule replacement (@c B).
	 */
	const s_list *(*get_replacement)(const SSyllabificationRewritesRule *self,
									 s_erc *error);

	/**
	 * Query if the given rule matches the given context (@c LC, @c RC),
	 * and if so, return the new right context (@c RC) after matching.
	 *
	 * @param self The given rule.
	 * @param LC The left context of the input tape, that is the
	 * phones (@c LC) to the left of the phone (@c A) currently being
	 * tested for. The left context must be swapped to make matching easier,
	 * i.e. if the context is <em> # c a r </em> then @c LC must be
	 * <em> r a c # </em>. May be @c NULL.
	 * @param RC The right context, that is the context of the phones
	 * (@c RC) to the right of the phone (@c A) currently being tested
	 * for. The context is in normal left-to-right fashion. May be @c NULL.
	 * @param sets The set mapping which may expand a specific
	 * rule. May be @c NULL.
	 * @param error Error code.
	 *
	 * @return The new right context (@c RC) after matching, or @c
	 * NULL if not a match.
	 */
	s_list *(*rule_matches)(const SSyllabificationRewritesRule *self,
							const s_str_list *LC, const s_str_list *RC,
							s_str_list **A, const SMap *sets,
							s_erc *error);
} SSyllabificationRewritesRuleClass;


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Get the rule replacement.
 *
 * @param self The given rule.
 * @param error Error code.
 *
 * @return The rule replacement (@c B).
 */
S_API const s_list *SSyllabificationRewritesRuleGetReplacement(const SSyllabificationRewritesRule *self,
															   s_erc *error);


/**
 * Query if the given rule matches the given context (@c LC, @c RC),
 * and if so, return the new right context (@c RC) after matching.
 *
 * @param self The given rule.
 * @param LC The left context of the input tape, that is the
 * phones (@c LC) to the left of the phone (@c A) currently being
 * tested for. The left context must be swapped to make matching easier,
 * i.e. if the context is <em> # c a r </em> then @c LC must be
 * <em> r a c # </em>. May be @c NULL.
 * @param RC The right context, that is the context of the phones
 * (@c RC) to the right of the phone (@c A) currently being tested
 * for. The context is in normal left-to-right fashion. May be @c NULL.
 * @param sets The set mapping which may expand a specific
 * rule. May be @c NULL.
 * @param error Error code.
 *
 * @return The new right context (@c RC) after matching, or @c
 * NULL if not a match.
 */
S_API s_list *SSyllabificationRewritesRuleMatches(const SSyllabificationRewritesRule *self,
												  const s_str_list *LC, const s_str_list *RC,
												  s_str_list **A, const SMap *sets,
												  s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SSyllabificationRewritesRule plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_syllabification_rewrites_rule_class_reg(s_erc *error);

/**
 * Free the #SSyllabificationRewritesRule plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_syllabification_rewrites_rule_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_SYLLABIFICATION_REWRITES_RULE_H__ */
