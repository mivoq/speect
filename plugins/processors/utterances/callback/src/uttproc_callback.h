/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
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
/* DATE    : 3 December 2009                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* The Utterance Processor callback class implementation for scripting languages.   */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_UTTPROCESSOR_CALLBACK_H__
#define _SPCT_PLUGIN_UTTPROCESSOR_CALLBACK_H__


/**
 * @file uttproc_callback.h
 * Utterance Processor callback class implementation for scripting languages.
 */


/**
 * @ingroup SUttProc
 * @defgroup SUttProcCallback Callback Utterance Processor
 * An implementation of an Utterance Processor class, with a @e callback
 * function that has been defined in a scripting language. The
 * #SUttProcessorRun function can then take this class and execute
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
 * Return the given #SUttProcessorCB child class object as a
 * #SUttProcessorCB object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SUttProcessorCB* type.
 *
 * @note This casting is not safety checked.
 */
#define S_UTTPROCESSOR_CB(SELF)    ((SUttProcessorCB *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SUttProcessorCB,
 * see full description #S_UTTPROCESSOR_CB_CALL for usage.
 *
 * @param SELF The given #SUttProcessorCB*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_UTTPROCESSOR_CB_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_UTTPROCESSOR_CB_CALL(SELF, FUNC)				\
	((SUttProcessorCBClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SUttProcessorCB
 * can be called.
 *
 * @param SELF The given #SUttProcessorCB*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_UTTPROCESSOR_CB_METH_VALID(SELF, FUNC)		\
	S_UTTPROCESSOR_CB_CALL(SELF, FUNC) ? TRUE : FALSE


/**
 * @}
 */


/************************************************************************************/
/*                                                                                  */
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

/**
 * Type definition of the callback execution function. Takes an utterance,
 * the scripting language <i> callback function </i> and an error
 * variable. The scripting language callback function must have this
 * typedef signature.
 */
typedef void (*callback)(SUtterance *utt, void *sfunction, s_erc *error);


/* forward declaration */
typedef struct SUttProcessorCB SUttProcessorCB;


/**
 * Type definition of the delete function. The delete function takes
 * care of any clean up actions required when deleting the scripting
 * language <i> callback function </i>.
 */
typedef void (*delete)(SUttProcessorCB *uttProc, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* SUttProcessorCB definition                                                       */
/*                                                                                  */
/************************************************************************************/

/**
 * The SUttProcessorCB structure.
 * A scripting language callback utterance processor class.
 * @extends SUttProcessor
 */
struct SUttProcessorCB
{
	/**
	 * @protected Inherit from #SUttProcessor.
	 */
	SUttProcessor obj;

	/**
	 * @protected Pointer to the wrapper function that executes the scripting
	 * language callback function.
	 */
	callback      sexecute;

	/**
	 * @protected Pointer to the wrapper function that takes care of
	 * any clean up actions required when deleting the scripting
	 * language callback function.
	 */
	delete        scleanup;

	/**
	 * @protected The scripting language callback function.
	 */
	void          *sfunction;
};


/************************************************************************************/
/*                                                                                  */
/* SUttProcessorCBClass definition                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * The SUttProcessorCBClass structure.
 * @extends SUttProcessorClass
 */
typedef struct
{
	/* Class members */
	/**
	 * @protected Inherit from #SUttProcessorClass.
	 */
	SUttProcessorClass _inherit;

  	/* Class methods */
	/**
	 * Set the scripting language callback function.
	 *
	 * @param self The callback utterance processor.
	 * @param sexecute The wrapper function that executes the
	 * scripting language callback function @c sfunction.
	 * @param scleanup The wrapper function that takes care of
	 * any clean up actions required when deleting the scripting
	 * language callback function.
	 * @param sfunction The scripting language callback function to execute.
	 */
	void  (*set_callback)(SUttProcessorCB *self,
						  callback sexecute, delete scleanup,
						  void *sfunction, s_erc *error);
} SUttProcessorCBClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SUttProcessorCB plug-in class with the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_uttprocessor_cb_class_reg(s_erc *error);


/**
 * Free the #SUttProcessorCB plug-in class from the Speect Engine
 * object system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_uttprocessor_cb_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_UTTPROCESSOR_CALLBACK_H__ */
