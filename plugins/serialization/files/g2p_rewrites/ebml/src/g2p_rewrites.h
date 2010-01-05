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
/* A g2p rewrites class implementation.                                             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_G2P_REWRITES_H__
#define _SPCT_PLUGIN_G2P_REWRITES_H__


/**
 * @file g2p_rewrites.h
 * A g2p rewrites class implementation.
 */


/**
 * @ingroup SG2PRewrites G2P Rewrites
 * @defgroup SG2PRewrites G2P Rewrites
 * A g2p rewrites class implementation.
 *
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "speect.h"
#include "g2p.h"
#include "g2p_rewrites_rule.h"


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
 * Return the given #SG2PRewrites child/parent class object as a
 * #SG2PRewrites object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SG2PRewrites* type.
 * @note This casting is not safety checked.
 */
#define S_G2PREWRITES(SELF)    ((SG2PRewrites *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SG2PRewrites,
 * see full description #S_G2PREWRITES_CALL for usage.
 *
 * @param SELF The given #SG2PRewrites*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_G2PREWRITES_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_G2PREWRITES_CALL(SELF, FUNC)					\
	((SG2PRewritesClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SG2PRewrites
 * can be called.
 *
 * @param SELF The given #SG2PRewrites*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_G2PREWRITES_METH_VALID(SELF, FUNC)		\
	S_G2PREWRITES_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

typedef struct
{
	char *symbol;
	char *replacement;
} s_gzero;


/************************************************************************************/
/*                                                                                  */
/* SG2PRewrites definition                                                          */
/*                                                                                  */
/************************************************************************************/

/**
 * The SG2PRewrites structure.
 * @extends SG2P
 */
typedef struct
{
	/**
	 * @protected Inherit from #SG2P.
	 */
	SG2P       obj;

	/**
	 * @protected Rewrite rules.
	 */
	SMap      *rules;

	/**
	 * @protected Grapheme zero's.
	 */
	s_gzero   *zeros;
} SG2PRewrites;


/************************************************************************************/
/*                                                                                  */
/* SG2PRewritesClass definition                                                     */
/*                                                                                  */
/************************************************************************************/

/**
 * Typedef of the SG2PRewrites class. Exactly the same as SG2P as no
 * class methods are added.
 */
typedef SG2PClass SG2PRewritesClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SG2PRewrites plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_g2p_rewrites_class_reg(s_erc *error);

/**
 * Free the #SG2PRewrites plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_g2p_rewrites_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_G2P_REWRITES_H__ */
