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
/* A token class implementation.                                                    */
/* Loosely based on EST_Token of Edinburgh Speech Tools,                            */
/* http://www.cstr.ed.ac.uk/projects/speech_tools (1.2.96)                          */
/* Note that this is a derived work with no verbatim source code from above         */
/* mentioned project.                                                               */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_TOKEN_H__
#define _SPCT_PLUGIN_TOKEN_H__


/************************************************************************************/
/* Edinburgh Speech Tools license,  EST_Token                                       */
/*                                                                                  */
/*                     Centre for Speech Technology Research                        */
/*                          University of Edinburgh, UK                             */
/*                           Copyright (c) 1996                                     */
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
/*       Author :  Alan W Black                                                     */
/*                                                                                  */
/************************************************************************************/


/**
 * @file token.h
 * A token class implementation.
 */


/**
 * @ingroup STokenizer
 * @defgroup SToken Token
 * A token class. A token consists of four parts, any of which may be
 * empty: the actual token, preceding whitespace, preceding
 * punctuation, and succeeding punctuation.
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
 * Return the given #SToken child/parent class object as a
 * #SToken object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SToken* type.
 * @note This casting is not safety checked.
 */
#define S_TOKEN(SELF)    ((SToken *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SToken,
 * see full description #S_TOKEN_CALL for usage.
 *
 * @param SELF The given #SToken*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_TOKEN_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_TOKEN_CALL(SELF, FUNC)					\
	((STokenClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SToken
 * can be called.
 *
 * @param SELF The given #SToken*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_TOKEN_METH_VALID(SELF, FUNC)		\
	S_TOKEN_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* SToken definition                                                                */
/*                                                                                  */
/************************************************************************************/

/**
 * The token structure.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject  obj;

	/**
	 * @protected Token preceding white-space.
	 */
	char *whitespace;

	/**
	 * @protected Token pre-punctuation.
	 */
	char *pre_punc;

	/**
	 * @protected Token post-punctuation.
	 */
	char *post_punc;

	/**
	 * @protected The actual token string.
	 */
	char *string;
} SToken;


/************************************************************************************/
/*                                                                                  */
/* STokenClass definition                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * The token class structure.
 * @extends SObjectClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SObjectClass.
	 */
	SObjectClass  _inherit;

	/* Class methods */
	/**
	 * Get the given token's preceding white-space.
	 * @protected GetWhitespace function pointer.
	 *
	 * @param self The given token.
	 * @param error Error code.
	 *
	 * @return The token's preceding white-space (may be @c NULL).
	 */
	const char *(*get_whitespace)(SToken *self, s_erc *error);

	/**
	 * Set the given token's preceding white-space.
	 * @protected SetWhitespace function pointer.
	 *
	 * @param self The given token.
	 * @param whitespace The white-space to set.
	 * @param error Error code.
	 */
	void (*set_whitespace)(SToken *self, const char *whitespace, s_erc *error);

	/**
	 * Get the given token's pre-punctuation.
	 * @protected GetPrePunc function pointer.
	 *
	 * @param self The given token.
	 * @param error Error code.
	 *
	 * @return The token's pre-punctuation (may be @c NULL).
	 */
	const char *(*get_pre_punc)(SToken *self, s_erc *error);

	/**
	 * Set the given token's pre-punctuation.
	 * @protected SetPrePunc function pointer.
	 *
	 * @param self The given token.
	 * @param pre_punc The pre-punctuation to set.
	 * @param error Error code.
	 */
	void (*set_pre_punc)(SToken *self, const char *pre_punc, s_erc *error);

	/**
	 * Get the given token's post-punctuation.
	 * @protected GetPostPunc function pointer.
	 *
	 * @param self The given token.
	 * @param error Error code.
	 *
	 * @return The token's post-punctuation (may be @c NULL).
	 */
	const char *(*get_post_punc)(SToken *self, s_erc *error);

	/**
	 * Set the given token's post-punctuation.
	 * @protected SetPostPunc function pointer.
	 *
	 * @param self The given token.
	 * @param post_punc The post-punctuation to set.
	 * @param error Error code.
	 */
	void (*set_post_punc)(SToken *self, const char *post_punc, s_erc *error);

	/**
	 * Get the given token's actual token string.
	 * @protected GetTokenString function pointer.
	 *
	 * @param self The given token.
	 * @param error Error code.
	 *
	 * @return The token's actual token string.
	 */
	const char *(*get_string)(SToken *self, s_erc *error);

	/**
	 * Set the given token's actual token string.
	 * @protected SetTokenString function pointer.
	 *
	 * @param self The given token.
	 * @param string The token string to set.
	 * @param error Error code.
	 */
	void (*set_string)(SToken *self, const char *string, s_erc *error);
} STokenClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SToken plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_token_class_reg(s_erc *error);

/**
 * Free the #SToken plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_token_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_TOKEN_H__ */
