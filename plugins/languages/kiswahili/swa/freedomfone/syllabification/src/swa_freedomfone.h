/************************************************************************************/
/* Copyright (c) 2010-2011 The Department of Arts and Culture,                      */
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
/* AUTHOR  : Aby Louw                                                               */
/* DATE    : August 2010                                                            */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* A Kiswahili (swa) syllabification class implementation for                       */
/* the Freedomfone project.                                                         */
/* Based on syllabification algorithm of the Festival Kiswahili voice               */
/* implementation of the University of Nairobi. See README for their license.       */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_SYLL_SWA_FREEDOMFONE_H__
#define _SPCT_PLUGIN_SYLL_SWA_FREEDOMFONE_H__


/**
 * @file swa_freedomfone.h
 * A Kiswahili (swa) syllabification class implementation for
 * the Freedomfone project.
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "speect.h"
#include "syllabification.h"


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
 * Return the given #SSyllabification child/parent class object as a
 * #SSyllabification object.
 *
 * @param SELF The given object.
 *
 * @return Given object as #SSyllabification* type.
 * @note This casting is not safety checked.
 */
#define S_SYLLAB_SWA_FREEDOMFONE(SELF)    ((SSyllabification *)(SELF))


/************************************************************************************/
/*                                                                                  */
/* SSyllabSwaFreedomfone definition                                                 */
/*                                                                                  */
/************************************************************************************/

/**
 * The SSyllabSwaFreedomfone structure. Does not add any members to the
 * #SSyllabification object, therefore exactly the same.
 */
typedef SSyllabification SSyllabSwaFreedomfone;


/************************************************************************************/
/*                                                                                  */
/* SSyllabSwaFreedomfoneClass definition                                            */
/*                                                                                  */
/************************************************************************************/

/**
 * The SSyllabSwaFreedomfoneClass structure. Does not add any methods to
 * SSyllabificationClass, so exactly the same.
 */
typedef SSyllabificationClass SSyllabSwaFreedomfoneClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SSyllabSwaFreedomfoneClass plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_syllab_swa_freedomfone_class_reg(s_erc *error);

/**
 * Free the #SSyllabSwaFreedomfoneClass plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_syllab_swa_freedomfone_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_SYLL_SWA_FREEDOMFONE_H__ */
