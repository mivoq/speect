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
/* DATE    : 8 September 2008                                                       */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A Viterbi decoder implementation.                                                */
/* Loosly based on EST_Viterbi of Edinburgh Speech Tools,                           */
/* http://www.cstr.ed.ac.uk/projects/speech_tools (1.2.96).                         */
/* Note that this is a derived work of EST_Viterbi                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_VITERBI_H__
#define _SPCT_PLUGIN_VITERBI_H__


/************************************************************************************/
/* Edinburgh Speech Tools license,  EST_Viterbi                                     */
/*                                                                                  */
/*                     Centre for Speech Technology Research                        */
/*                          University of Edinburgh, UK                             */
/*                            Copyright (c) 1996,1997                               */
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
 * @file viterbi.h
 * A Viterbi decoder implementation.
 */


/**
 * @defgroup SViterbi
 * A Viterbi decoder implementation.
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "speect.h"
#include "vit_point.h"
#include "vit_path.h"
#include "vit_candidate.h"


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
 * Return the given #SViterbi child/parent class object as a
 * #SViterbi object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SViterbi* type.
 * @note This casting is not safety checked.
 */
#define S_VITERBI(SELF)    ((SViterbi *)(SELF))


/**
 * @hideinitializer
 * Call the given function method of the given #SViterbi,
 * see full description #S_VITERBI_CALL for usage.
 *
 * @param SELF The given #SViterbi*.
 * @param FUNC The function method of the given object to call.
 *
 * @note This casting is not safety checked.
 * @note Example usage: @code S_VITERBI_CALL(self, func)(param1, param2, ..., paramN); @endcode
 * where @c param1, @c param2, ..., @c paramN are the parameters passed to the object function
 * @c func.
 */
#define S_VITERBI_CALL(SELF, FUNC)				\
	((SViterbiClass *)S_OBJECT_CLS(SELF))->FUNC


/**
 * @hideinitializer
 * Test if the given function method of the given #SViterbi
 * can be called.
 *
 * @param SELF The given #SViterbi*.
 * @param FUNC The function method of the given object to check.
 *
 * @return #TRUE if function can be called, otherwise #FALSE.
 *
 * @note This casting is not safety checked.
 */
#define S_VITERBI_METH_VALID(SELF, FUNC)		\
	S_VITERBI_CALL(SELF, FUNC) ? TRUE : FALSE


/************************************************************************************/
/*                                                                                  */
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

/**
 * Type definition of the user callback @e path function. This
 * callback function assigns scores on the path extension (probability
 * of going from candidate in one state to a candidate in another
 * state).
 */
typedef SViterbiPath *(*pathFunc)(SViterbiPath *p, SViterbiCandidate *c, SMap *features,
								  s_erc *error);


/**
 * Type definition of the user callback @e candidate function. This
 * callback function assigns scores on the state candidates (probability
 * of choosing a candidate in one state).
 */
typedef SViterbiCandidate *(*candFunc)(const SItem *s, SMap *features, s_erc *error);


/************************************************************************************/
/*                                                                                  */
/* SViterbi definition                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * The SViterbi structure.
 * @extends SObject
 */
typedef struct
{
	/**
	 * @protected Inherit from #SObject.
	 */
	SObject        obj;

	/**
	 * @protected Number of states in viterbi.
	 */
	int            num_states;

	/**
	 * @protected Beam width (for general beam-search).
	 */
	int            beam_width;

	/**
	 * @protected Candidate width (for general beam-search).
	 */
	int            cand_width;

	/**
	 * @protected Bigger is better flag.
	 */
	s_bool         big_is_good;

	/**
	 * @protected Path callback function.
	 */
	pathFunc       userPathFunc;

	/**
	 * @protected Candidates callback function.
	 */
	candFunc       userCandFunc;

	/**
	 * @protected Viterbi time-line.
	 */
	SViterbiPoint *timeLine;

	/**
	 * @protected Debug flag.
	 */
	s_bool         debug;

	/**
	 * @protected Trace flag.
	 */
	s_bool         trace;

	/**
	 * @protected Pruning flag.
	 */
	s_bool         do_pruning;

	/**
	 * @protected Path pruning envelope width.
	 */
	float          path_prune_envelope_width;

	/**
	 * @protected Candidate pruning envelope width.
	 */
	float          cand_prune_envelope_width;

	/**
	 * @protected Viterbi features.
	 */
	SMap          *features;
} SViterbi;


/************************************************************************************/
/*                                                                                  */
/* SViterbiClass definition                                                         */
/*                                                                                  */
/************************************************************************************/

/**
 * The SViterbiClass structure.
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
	 * Initialize the viterbi.
	 *
	 * @param self The viterbi to initialize.
	 * @param rel The relation which contains the viterbi time points.
	 * @param userCandFunc The user @e candidate callback function.
	 * @param userPathFunc The user @e path callback function.
	 * @param num_states The number of states at each time point (-1
	 * for a dynamic number of states).
	 * @param config A map with configuration settings for the
	 * viterbi (can be @c NULL). The settings read from the map are:
	 * <ul>
	 * <li> @e debug (@c TRUE or @c FALSE) (@c string) </li>
	 * <li> @e trace (@c TRUE or @c FALSE) (@c string) </li>
	 * <li> @e beam-width (for general beam-search) (@c int) </li>
	 * <li> @e cand-width (for general beam-search) (@c int) </li>
	 * <li> @e do-pruning (@c TRUE or @c FALSE) </li>
	 * <li> @e path-prune-envelope-width (@c float) </li>
	 * <li> @e cand-prune-envelope-width (@c float) </li>
	 * </ul>
	 * All of these configuration parameters are optional.
	 * @param error Error code.
	 *
	 * @note If this function fails then the viterbi will be deleted
	 * and the @c self variable set to @c NULL.
	 */
	void (*init_viterbi)(SViterbi **self, SRelation *rel,
						 candFunc userCandFunc, pathFunc userPathFunc,
						 int num_states, const SMap *config, s_erc *error);

	/**
	 * Set the beam width of the viterbi.
	 *
	 * @param self The given viterbi.
	 * @param width The beam width to set.
	 * @param error Error code.
	 */
	void (*set_beam_width)(SViterbi *self, int width, s_erc *error);

	/**
	 * Set the candidate width of the viterbi.
	 *
	 * @param self The given viterbi.
	 * @param width The candidate width to set.
	 * @param error Error code.
	 */
	void (*set_cand_width)(SViterbi *self, int width, s_erc *error);

	/**
	 * Set bigger numbers are better (for probabilities).
	 * Default is that @b smaller numbers are better.
	 *
	 * @param self The given viterbi.
	 * @param error Error code.
	 */
	void (*set_big_is_good)(SViterbi *self, s_erc *error);

	/**
	 * Set the debug flag to @c TRUE (default is @c FALSE).
	 *
	 * @param self The given viterbi.
	 * @param error Error code.
	 */
	void (*set_debug)(SViterbi *self, s_erc *error);

	/**
	 * Set the trace flag to @c TRUE (default is @c FALSE).
	 *
	 * @param self The given viterbi.
	 * @param error Error code.
	 */
	void (*set_trace)(SViterbi *self, s_erc *error);

	/**
	 * Set a feature for the viterbi.
	 *
	 * @param self The given viterbi.
	 * @param key The feature key.
	 * @param object The feature object.
	 * @param error Error code.
	 */
	void (*set_feature)(SViterbi *self, const char *key,
						const SObject *object, s_erc *error);

	/**
	 * Set the viterbi pruning parameters. .
	 *
	 * @param self The given viterbi.
	 * @param cand_beam The candidate beam pruning value.
	 * @param cand_beam The path beam pruning value.
	 * @param error Error code.
	 */
	void (*set_pruning_params)(SViterbi *self, float cand_beam, float path_beam,
							   s_erc *error);

	/**
	 * Do the the actual search.
	 *
	 * @param self The given viterbi.
	 * @param error Error code.
	 */
	void  (*search)(SViterbi *self, s_erc *error);

	/**
	 * Finds best path through the search space (previously searched)
	 * and adds best searched unit under feature name 'feat_name' to
	 * relation with which viterbi was initialized.
	 *
	 * @param self The given viterbi.
	 * @param feat_name The feature name under which the best
	 * candidate unit will be set in the items of the relation with
	 * which the viterbi was initialized.
	 * @param error Error code.
	 */
	void  (*copy_best_result)(SViterbi *self, const char *feat_name, s_erc *error);

	/**
	 * Extract the @b end point of the best path found during search.
	 *
	 * @param self The given viterbi.
	 * @param error Error code.
	 *
	 * @return The @b end point of the best path or @c NULL if no path is found.
	 */
	const SViterbiPath *(*get_best_end)(SViterbi *self, s_erc *error);
} SViterbiClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SViterbi plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_viterbi_class_reg(s_erc *error);


/**
 * Free the #SViterbi plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_viterbi_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_VITERBI_H__  */
