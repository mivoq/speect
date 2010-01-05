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
/* DATE    : 1 December 2009                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* The Feature Processor callback class implementation for scripting languages.     */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_FEATPROCESSOR_CALLBACK_H__
#define _SPCT_PLUGIN_FEATPROCESSOR_CALLBACK_H__


/**
 * @file featproc_callback.h
 * Feature Processor callback class implementation for scripting languages.
 */


/**
 * @ingroup SFeatureProc
 * @defgroup SFeatureProcCallback Callback Feature Processor
 * An implementation of a Feature Processor class, with a @e callback
 * function that has been defined in a scripting language. The
 * #SFeatProcessorRun function can then take this class and execute
 * the callback function, without Speect being any wiser.
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
 * @name Utility Macros
 * @{
 */


/**
 * @hideinitializer
 * Return the given #SFeatProcessorCB child class object as a
 * #SFeatProcessorCB object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SFeatProcessorCB* type.
 *
 * @note This casting is not safety checked.
 */
#define S_FEATPROCESSOR_CB(SELF)    ((SFeatProcessorCB *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SFeatProcessorCB,
 * see full description #S_FEATPROCESSOR_CB_CALL for usage.
 *
 * @param SELF The given #SFeatProcessorCB*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_FEATPROCESSOR_CB_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_FEATPROCESSOR_CB_CALL(SELF, FUNC)				\
	((SFeatProcessorCBClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SFeatProcessorCB
 * can be called.
 *
 * @param SELF The given #SFeatProcessorCB*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_FEATPROCESSOR_CB_METH_VALID(SELF, FUNC)		\
	S_FEATPROCESSOR_CB_CALL(SELF, FUNC) ? TRUE : FALSE


/**
 * @}
 */


/************************************************************************************/
/*                                                                                  */
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

/**
 * Type definition of the callback execution function. Takes an item,
 * the scripting language <i> callback function </i> and an error
 * variable, and returns an #SObject. The scripting language callback
 * function must have this typedef signature.
 */
typedef SObject *(*callback)(const SObject *item, void *sfunction, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* SFeatProcessorCB definition                                                      */
/*                                                                                  */
/************************************************************************************/

/**
 * The SFeatProcessorCB structure.
 * A Python callback feature processor class.
 * @extends SFeatProcessor
 */
typedef struct
{
	/**
	 * @protected Inherit from #SFeatProcessor.
	 */
	SFeatProcessor obj;

	/**
	 * @protected Pointer to the wrapper function that executes the scripting
	 * language callback function.
	 */
	callback       sexecute;

	/**
	 * @protected The scripting language callback function.
	 */
	void          *sfunction;
} SFeatProcessorCB;


/************************************************************************************/
/*                                                                                  */
/* SFeatProcessorCBClass definition                                                 */
/*                                                                                  */
/************************************************************************************/

/**
 * The SFeatProcessorCBClass structure.
 * @extends SFeatProcessorClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SFeatProcessorClass.
	 */
	SFeatProcessorClass _inherit;

  	/* Class methods */
	/**
	 * Set the scripting language callback function.
	 *
	 * @param self The callback feature processor.
	 * @param sexecute The wrapper function that executes the
	 * scripting language callback function @c sfunction.
	 * @param sfunction The scripting language callback function to execute.
	 */
	void  (*set_callback)(SFeatProcessorCB *self, callback sexecute,
						  void *sfunction, s_erc *error);
} SFeatProcessorCBClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SFeatProcessorCB plug-in class with the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_featprocessor_cb_class_reg(s_erc *error);


/**
 * Free the #SFeatProcessorCB plug-in class from the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_featprocessor_cb_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_FEATPROCESSOR_CALLBACK_H__ */
