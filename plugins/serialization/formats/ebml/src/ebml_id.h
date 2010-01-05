/************************************************************************************/
/* Copyright (c) 2008-2009 The Department of Arts and Culture,                      */
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
/* DATE    : 20 August 2008                                                         */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Definition of EBML IDs (version 1.0 compliant).                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#ifndef _SPCT_EBML_ID_H__
#define _SPCT_EBML_ID_H__


/**
 * @file ebml_id.h
 * Definition of EBML id's.
 */


/**
 * @ingroup SEbml
 * @defgroup SEbmlID EBML IDs
 * Definition of EBML IDs (version 1.0 compliant).
 * <tt> M. Nilsson, "Extensible Binary Markup Language", 15th March 2004, ebml-1.0.txt. </tt>
 * @{
 */


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "include/common.h"
#include "base/utils/types.h"


/************************************************************************************/
/*                                                                                  */
/* Begin external c declaration                                                     */
/*                                                                                  */
/************************************************************************************/
S_BEGIN_C_DECLS


/************************************************************************************/
/*                                                                                  */
/* Defines of EBML IDs                                                              */
/*                                                                                  */
/************************************************************************************/

/**
 * EBML standard version.
 */
#define S_EBML_VERSION 1


/**
 * @name EBML standard elements
 * @{
 */


/* level 0: */
/**
 * EBML header
 */
#define S_EBML_ID_HEADER             0x1A45DFA3


/* level 1 : IDs in the header */
/**
 * EBMLVersion
 */
#define S_EBML_ID_EBMLVERSION        0x4286


/**
 * EBMLReadVersion
 */
#define S_EBML_ID_EBMLREADVERSION    0x42F7


/**
 * EBMLMaxIDWidth
 */
#define S_EBML_ID_EBMLMAXIDLENGTH    0x42F2


/**
 * EBMLMaxSizeWidth
 */
#define S_EBML_ID_EBMLMAXSIZELENGTH  0x42F3


/**
 * DocType
 */
#define S_EBML_ID_DOCTYPE            0x4282


/**
 * DocTypeVersion
 */
#define S_EBML_ID_DOCTYPEVERSION     0x4287


/**
 * DocTypeReadVersion
 */
#define S_EBML_ID_DOCTYPEREADVERSION 0x4285


/* level 1+ : Global elements IDs */
/**
 * CRC32
 */
#define S_EBML_ID_CRC32              0xBF


/**
 * Void
 */
#define S_EBML_ID_VOID               0xEC


/**
 * @}
 */


/**
 * @name Other EBML definitions
 * @{
 */


/**
 * Unknown size, all bits set to 1.
 */
#define S_EBML_SIZE_UNKNOWN          0xffffffff


/**
 * Date offset from the unix epoch in seconds, 2001/01/01 00:00:00 UTC.
 */
#define S_EBML_DATE_OFFSET           978307200


/**
 * Defined nanosecond.
 */
#define S_EBML_NANO_SEC              0.000000001


/**
 * Master element dummy size (litte endian).
 */
#define S_EBML_MASTER_ELEMENT_DUMMY_SIZE     0x1000000


/**
 * @}
 */


/************************************************************************************/
/*                                                                                  */
/* Data types                                                                       */
/*                                                                                  */
/************************************************************************************/

/**
 * Ebml header structure.
 */
typedef struct
{
        uint8  ebml_version;         /*!< Ebml version.                  */
        uint8  ebml_read_version;    /*!< Ebml reader version.           */
        uint8  max_id_width;         /*!< Maximum width of id (bytes).   */
        uint8  max_size_width;       /*!< Maximum width of size (bytes). */
        char  *doctype;              /*!< Doctype string.                */
        uint8  doctype_version;      /*!< Doctype version.               */
        uint8  doctype_read_version; /*!< Doctype reader version.        */
} s_ebml_header;


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

#endif /* _SPCT_EBML_ID_H__ */
