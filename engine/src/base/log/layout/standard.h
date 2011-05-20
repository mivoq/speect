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
/* DATE    : 29 September 2009                                                      */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Standard layout functions.                                                       */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_LAYOUT_STANDARD_H__
#define _SPCT_LAYOUT_STANDARD_H__


/**
 * @file standard.h
 * Standard layout functions.
 */


/**
 * @ingroup SLayout
 * @defgroup SLayoutStd Standard
 * A standard layout for the Speect engine logging functions.
 *
 * <h3> Error messages layout </h3>
 * The error messages layout is as follows:
 *
 *
 * @code "%s [%s (%s) %ul] %s (in function '%s', %s, %d)\n" @endcode
 *
 * <br>
 * <table>
 * <tr>
 * <td> Date & time </td>
 * <td> Logging event level <sup>1</sup> </td>
 * <td> Error message <sup>2</sup> </td>
 * <td> Thread id <sup>3</sup> </td>
 * <td> User message </td>
 * <td> Function name <sup>4</sup> </td>
 * <td> File name <sup>4</sup> </td>
 * <td> File line number <sup>4</sup> </td>
 * </tr>
 * </table>
 *
 * <ol>
 * <li> Derived automatically from #s_log_event </li>
 * <li> Derived automatically from #s_erc </li>
 * <li> Only if compiled with threads </li>
 * <li> Optional </li>
 * </ol>
 * For example: <br> <br>
 * <tt> Mon Sep 28 13:36:55 2009 [ERROR (Memory error) 10] Failed to allocate object memory (in function 'func', /home/nobody/test.c, 121) </tt>
 * <br> <br>
 * with all the elements available.
 *
 * <h3> Debug messages format </h3>
 * The debug messages layout is as follows:
 *
 *
 * @code "%s [%s %ul] %s\n" @endcode
 *
 * <br>
 * <table>
 * <tr>
 * <td> Date & time </td>
 * <td> Debug message type <sup>1</sup> </td>
 * <td> Thread id <sup>2</sup> </td>
 * <td> User message </td>
 * </tr>
 * </table>
 *
 * <ol>
 * <li> Derived automatically from #s_dbg_lvl and #s_log_event </li>
 * <li> Only if compiled with threads </li>
 * </ol>
 * For example: <br> <br>
 * <tt> Mon Sep 28 13:36:55 2009 [TRACE 10] Iterator 10, value = 20.321 </tt>
 *
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/log/layout/layouts.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Function prototypes                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * Create a new @a standard layout formatter. The layout implements
 * the function pointers of #s_layout (#s_layout::format and
 * #s_layout::destroy).
 *
 * @return Pointer to newly created standard layout formatter that can
 * be used with the functions defined in @ref SLayout, or @c NULL on
 * error.
 *
 * @note Only thread safe if compiled with threading library see @ref
 * SThreads.
 *
 * @todo check MT safety
 */
S_API s_layout *s_layout_std_new(void);


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

#endif /* _SPCT_LAYOUT_STANDARD_H__ */

