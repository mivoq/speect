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
/* Floating point track EBML element id's.                                          */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_PLUGIN_TRACK_FLOAT_EBML_ELEMENT_ID__
#define _SPCT_PLUGIN_TRACK_FLOAT_EBML_ELEMENT_ID__


/**
 * @file element_id.h
 * Floating point track EBML element id's.
 */


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

/*
 * EBML Definition of a STrackFloat object's ObjectData (0x82)
 *
 * S_EBML_ID_OBJECT_DATA := 82 container [ card:1; ] [ ordered:yes ]
 * {
 *     S_TRACK_FLOAT_EBML_ROW_COUNT := 83 uint;
 *     S_TRACK_FLOAT_EBML_COL_COUNT := 84 uint;
 *     S_TRACK_FLOAT_EBML_ROW_DATA := 85 container [ card:*; ]
 *     {
 *         S_TRACK_FLOAT_EBML_TIME := 86 float;
 *         S_TRACK_FLOAT_EBML_COL_DATA := 87 container [ card:+; ]
 *         {
 *             S_TRACK_FLOAT_EBML_DATA_ELEMENT := 88 float;
 *         }
 *     }
 * }
 */


/**
 * Floating point tracks EBML element id's.
 */
#define S_TRACK_FLOAT_EBML_ROW_COUNT     0x83
#define S_TRACK_FLOAT_EBML_COL_COUNT     0x84
#define S_TRACK_FLOAT_EBML_ROW_DATA      0x85
#define S_TRACK_FLOAT_EBML_TIME          0x86
#define S_TRACK_FLOAT_EBML_COL_DATA      0x87
#define S_TRACK_FLOAT_EBML_DATA_ELEMENT  0x88


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

#endif /* _SPCT_PLUGIN_TRACK_FLOAT_EBML_ELEMENT_ID__  */
