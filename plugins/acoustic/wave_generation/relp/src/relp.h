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
/* DATE    : December 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A Residual excited linear prediction (RELP) waveform generator.                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_RELP_H__
#define _SPCT_PLUGIN_RELP_H__


/**
 * @file relp.h
 * A Residual excited linear prediction (RELP) waveform generator.
 */


/**
 * @defgroup SRelp Residual Excited Linear Prediction
 * A Residual excited linear prediction (RELP) waveform generator.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "speect.h"
#include "array_int.h"
#include "track_float.h"
#include "window.h"
#include "audio.h"


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
 * Return the given #SRelp child/parent class object as a
 * #SRelp object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SRelp* type.
 * @note This casting is not safety checked.
 */
#define S_RELP(SELF)    ((SRelp *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SRelp,
 * see full description #S_RELP_CALL for usage.
 *
 * @param SELF The given #SRelp*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_RELP_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_RELP_CALL(SELF, FUNC)					\
	((SRelpClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SRelp
 * can be called.
 *
 * @param SELF The given #SRelp*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_RELP_METH_VALID(SELF, FUNC)		\
	S_RELP_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/**
 * The mapping from source to target units.
 * Currently only a linear mapping.
 */
typedef enum
{
	S_RELP_LINEAR = 0  /*!< Linear mapping     */
} s_relp_mapping;


/************************************************************************************/
/*                                                                                  */
/* SRelp definition                                                                 */
/*                                                                                  */
/************************************************************************************/

/**
 * The SRelp structure.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject       obj;

	/**
	 * @protected Residuals.
	 */
	SArrayInt   **residuals;

	/**
	 * @protected Track.
	 */
	STrackFloat  *track;

	/**
	 * @protected Number of frames (time points).
	 */
	uint32        num_frames;

	/**
	 * @protected Number of channels (coefficients).
	 */
	uint8         num_channels;

	/**
	 * @protected Sample rate.
	 */
	uint32        sample_rate;

	/**
	 * @protected Window function.
	 */
	SWinFunc     *window;

	/**
	 * @protected Symmetric window flag.
	 */
	s_bool        symmetric;

	/**
	 * @protected Source to target mapping.
	 */
	s_relp_mapping mapping;

	/**
	 * @protected Map factors.
	 */
	SArrayInt    *map;

	/**
	 * @protected Target track.
	 */
	STrackFloat  *target;

	/**
	 * @protected The combine residuals of all the units.
	 */
	SAudio       *wave_res;

	/**
	 * @protected Pitch mark indices's.
	 */
	SArrayInt    *pmIndx;

	/**
	 * @protected Window gain factor.
	 */
	float         window_factor;
} SRelp;


/************************************************************************************/
/*                                                                                  */
/* SRelpClass definition                                                            */
/*                                                                                  */
/************************************************************************************/

/**
 * The SRelpClass structure.
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
	 * Initialize the given #SRelp object.
	 *
	 * @param self The #SRelp object to initialize.
	 * @param sample_rate The sample rate of the units.
	 * @param window_class The class name of the window to use.
	 * @param window_is_symmetric @c TRUE if the window to be used
	 * must be symmetric else @c FALSE.
	 * @param mapping The source to target mapping (currently only @c
	 * S_RELP_LINEAR).
	 * @param window_gain The gain factor of the window.
	 * @param error Error code.
	 *
	 * @note If the function fails then the #SRelp object is deleted
	 * and the @c self variable is set to @c NULL.
	 */
	void (*init_relp)(SRelp **self, uint32 sample_rate, const char *window_class,
					  s_bool window_is_symmetric, s_relp_mapping mapping,
					  float window_gain, s_erc *error);

	/**
	 * Concatenate the units in the given relation.
	 *
	 * @param self The given #SRelp object.
	 * @param unitRel The unit relation.
	 * @param error Error code.
	 */
	void (*concat_units)(SRelp *self, const SRelation *unitRel, s_erc *error);

	/**
	 * Map the source to target tracks.
	 *
	 * @param self The given #SRelp object.
	 * @param error Error code.
	 */
	void (*map_tracks)(SRelp *self, s_erc *error);

	/**
	 * Generate the audio from the tracks.
	 *
	 * @param self The given #SRelp object.
	 * @param error Error code.
	 *
	 * @return The generated audio.
	 *
	 * @note The caller is responsible for the memory of the returned
	 * audio object.
	 */
	SAudio *(*generate)(SRelp *self, s_erc *error);
} SRelpClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SRelp plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_relp_class_reg(s_erc *error);

/**
 * Free the #SRelp plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_relp_class_free(s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* Local function prototypes                                                        */
/*                                                                                  */
/************************************************************************************/

/**
 * Concatenate unit tracks.
 * @private
 *
 * @param self The given #SRelp object.
 * @param unitRel The unit relation.
 * @param error Error code.
 */
S_LOCAL void concat_units(SRelp *self, const SRelation *unitRel, s_erc *error);


/**
 * Do a linear mapping from source tracks to target tracks.
 * @private
 *
 * @param self The given #SRelp object.
 * @param error Error code.
 */
S_LOCAL void linear_mapping(SRelp *self, s_erc *error);


/**
 * Synthesize the tracks in the #SRelp object.
 * @private
 *
 * @param self The given #SRelp object.
 * @param error Error code.
 *
 * @return The resulting audio object.
 *
 * @note The caller is responsible for the memory of the returned
 * audio object.
 */
S_LOCAL SAudio *synthesis(SRelp *self, s_erc *error);


/**
 * Window the given residuals.
 * @private
 *
 * @param self The given #SRelp object.
 * @param residual The residuals to window.
 * @param coefs The coefficients of the residuals.
 * @param frame_number The frame number.
 * @param error Error code.
 *
 * @note Used internally by #concat_units
 */
S_LOCAL void window_residuals(SRelp *self, SArrayInt *residual, STrackFloat *coefs,
							  uint frame_number, s_erc *error);


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

#endif /* _SPCT_PLUGIN_RELP_H__ */
