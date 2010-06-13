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
/* DATE    : 25 March 2008                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Implementation of version numbers data types.                                    */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "base/utils/vernum.h"


/************************************************************************************/
/*                                                                                  */
/* Defines                                                                          */
/*                                                                                  */
/************************************************************************************/

/* Minimum major version of Speect Engine ABI from plug-ins that will
 * work with this version of Speect Engine */
#define SPCT_MAJOR_VERSION_MIN 1

/* Minimum minor version of Speect Engine ABI from plug-ins that will
 * work with this version of Speect Engine */
#define SPCT_MINOR_VERSION_MIN 0


/************************************************************************************/
/*                                                                                  */
/* Static variables                                                                 */
/*                                                                                  */
/************************************************************************************/

/* definitions in src/include/version.h */
static const s_lib_version speect_version =
{
	S_MAJOR_VERSION,   /*!< Major version number. */
	S_MINOR_VERSION,   /*!< Minor version number. */
	S_PATCHLEVEL,      /*!< Patch number.         */
	S_RELEASE          /*!< Release name.         */
};


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_API s_lib_version s_speect_version(void)
{
	return speect_version;
}


S_API s_bool s_lib_version_ok(uint8 major_min, uint8 minor_min)
{
	uint plugin_min_version;
	uint speect_version;


	plugin_min_version = 1000 * major_min + minor_min;
	speect_version = 1000 * S_MAJOR_VERSION + S_MINOR_VERSION;

	if (speect_version >= plugin_min_version)
		return TRUE;

	return FALSE;
}


S_LOCAL s_bool s_version_ok(const s_version version)
{
	uint plugin_version;
	uint speect_min_version;


	plugin_version = 1000 * version.major + version.minor;
	speect_min_version = 1000 * SPCT_MAJOR_VERSION_MIN + SPCT_MINOR_VERSION_MIN;

	if (plugin_version >= speect_min_version)
		return TRUE;

	return FALSE;
}
