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
/* DATE    : 5 October 2009                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Forward declarations of the Heterogeneous Relation Graphs objects.               */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_HRG_DEF_H__
#define _SPCT_HRG_DEF_H__


/**
 * @file hrg_defs.h
 * Declarations of the HRG objects.
 */


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Typedefs                                                                         */
/*                                                                                  */
/************************************************************************************/

/*
 * forward declare as everything from here needs these.
 */

/**
 * A class for containing the shared contents of items.
 */
typedef struct SItmContent SItmContent;


/**
 * A class for containing the items (arbitrary objects).
 */
typedef struct SItem SItem;


/**
 * A Relation is a named list of an arbitrary number of #SItem.
 */
typedef struct SRelation SRelation;


/**
 * The Utterance class implementation.
 */
typedef struct SUtterance SUtterance;


/**
 * Typedef of struct SVoice.
 */
typedef struct SVoice SVoice;


/************************************************************************************/
/*                                                                                  */
/* End external c declaration                                                       */
/*                                                                                  */
/************************************************************************************/
S_END_C_DECLS


#endif /* _SPCT_HRG_DEF_H__ */
