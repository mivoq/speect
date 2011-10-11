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
/* A South African English (eng-za) syllabification class                           */
/* implementation for the Lwazi project. Based on T.A. Hall,                        */
/* "English syllabification as the interaction of markedness                        */
/* constraints*", Studia Linguistica, vol. 60, 2006, pp. 1-33                       */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_SYLL_ENG_ZA_LWAZI_H__
#define _SPCT_PLUGIN_SYLL_ENG_ZA_LWAZI_H__


/**
 * @file eng_za_lwazi.h
 * A South African English (eng-za) syllabification class
 * implementation for the Lwazi project.
 */


/**
 * @ingroup SSyllabification Syllabification
 * @defgroup SSyllabEngZaLwazi South African English Lwazi Syllabification
 * A South African English (eng-za) syllabification class
 * implementation for the Lwazi project. Based on T.A. Hall, "English
 * syllabification as the interaction of markedness constraints*",
 * Studia Linguistica, vol. 60, 2006, pp. 1-33
 * @{
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
#define S_SYLLAB_ENG_ZA_LWAZI(SELF)    ((SSyllabification *)(SELF))




/************************************************************************************/
/*                                                                                  */
/* SSyllabEngZaLwazi definition                                                     */
/*                                                                                  */
/************************************************************************************/

/**
 * The SSyllabEngZaLwazi structure. Does not add any members to the
 * #SSyllabification object, therefore exactly the same.
 */
typedef SSyllabification SSyllabEngZaLwazi;


/************************************************************************************/
/*                                                                                  */
/* SSyllabEngZaLwaziClass definition                                                */
/*                                                                                  */
/************************************************************************************/

/**
 * The SSyllabEngZaLwaziClass structure. Does not add any methods to
 * SSyllabificationClass, so exactly the same.
 */
typedef SSyllabificationClass SSyllabEngZaLwaziClass;


/************************************************************************************/
/*                                                                                  */
/* Plug-in class registration/free                                                  */
/*                                                                                  */
/************************************************************************************/

/**
 * Register the #SSyllabEngZaLwaziClass plug-in class with the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_syllab_eng_za_lwazi_class_reg(s_erc *error);

/**
 * Free the #SSyllabEngZaLwaziClass plug-in class from the Speect Engine object
 * system.
 * @private
 *
 * @param error Error code.
 */
S_LOCAL void _s_syllab_eng_za_lwazi_class_free(s_erc *error);


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

#endif /* _SPCT_PLUGIN_SYLL_ENG_ZA_LWAZI_H__ */
