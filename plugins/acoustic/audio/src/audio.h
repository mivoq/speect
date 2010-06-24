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
/* A simple audio (waveform) class that can be used as an SObject.                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_AUDIO_H__
#define _SPCT_PLUGIN_AUDIO_H__


/**
 * @file audio.h
 * A simple audio (waveform) class that can be used as an SObject.
 */


/**
 * @defgroup SAudio Audio
 * A simple audio (waveform) class that can be used as an #SObject.
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
 * Return the given #SAudio child/parent class object as a
 * #SAudio object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SAudio* type.
 * @note This casting is not safety checked.
 */
#define S_AUDIO(SELF)    ((SAudio *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SAudio,
 * see full description #S_AUDIO_CALL for usage.
 *
 * @param SELF The given #SAudio*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_AUDIO_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_AUDIO_CALL(SELF, FUNC)				\
	((SAudioClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SAudio
 * can be called.
 *
 * @param SELF The given #SAudio*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_AUDIO_METH_VALID(SELF, FUNC)		\
	S_AUDIO_CALL(SELF, FUNC) ? TRUE : FALSE



/************************************************************************************/
/*                                                                                  */
/* SAudio definition                                                                */
/*                                                                                  */
/************************************************************************************/

/**
 * The SAudio structure.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject       obj;

	/**
	 * @protected Sample rate.
	 */
	uint32       sample_rate;

	/**
	 * @protected Number of samples.
	 */
	uint32       num_samples;

	/**
	 * @protected Samples.
	 */
	float       *samples;
} SAudio;


/************************************************************************************/
/*                                                                                  */
/* SAudioClass definition                                                           */
/*                                                                                  */
/************************************************************************************/

/**
 * The SAudioClass structure.
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
	 * Resize the given audio object to the given size.
	 *
	 * @param self The given audio object.
	 * @param new_size The new size of the audio object.
	 * @param error Error code.
	 *
	 * @note The samples are truncated if the new size is smaller than
	 * the current size.
	 */
	void (*resize)(SAudio *self, uint32 new_size, s_erc *error);

	/**
	 * Scale the given audio object with the given factor.
	 *
	 * @param self The given audio object.
	 * @param factor The factor with which to scale the samples.
	 * @param error Error code.
	 */
	void (*scale)(SAudio *self, float factor, s_erc *error);
} SAudioClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SAudio plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_audio_class_reg(s_erc *error);


/**
 * Free the #SAudio plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_audio_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_AUDIO_H__ */
