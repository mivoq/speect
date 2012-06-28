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
/* DATE    : 28 January 2009                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Initialization and quit of the Speect Engine modules.                            */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "main/modules.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_modules_init(s_erc *error)
{
	s_erc local_err;


	S_CLR_ERR(&local_err);
	S_CLR_ERR(error);

	_s_base_init(error);                                   /* 5 classes */
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_modules_init",
				  "Failed to intialize base module"))
		local_err = *error;

	_s_containers_init(error);                             /* 10 classes */
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_modules_init",
				  "Failed to intialize containers module"))
		local_err = *error;

	_s_datasources_init(error);                            /* 5 classes */
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_modules_init",
				  "Failed to intialize datasources module"))
		local_err = *error;

	_s_utils_init(error);                                  /* 4 classes */
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_modules_init",
				  "Failed to intialize utils module"))
		local_err = *error;

	_s_hrg_init(error);                                    /* 6 classes */
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_modules_init",
				  "Failed to intialize hrg module"))
		local_err = *error;

	_s_pluginmanager_init(error);                          /* 2 classes */
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_modules_init",
				  "Failed to intialize pluginmanager module"))
		local_err = *error;

	_s_serialization_init(error);                          /* 3 classes */
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_modules_init",
				  "Failed to intialize serialization module"))
		local_err = *error;

	_s_voicemanager_init(error);                           /* 1 class */
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_modules_init",
				  "Failed to intialize voicemanager module"))
		local_err = *error;

	/* if there was an error local_err will have it */
	if ((local_err != S_SUCCESS) && (*error == S_SUCCESS))
		*error = local_err;
}


/* reverse order */
S_LOCAL void _s_modules_quit(s_erc *error)
{
	s_erc local_err;


	S_CLR_ERR(&local_err);
	S_CLR_ERR(error);

	_s_voicemanager_quit(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_modules_quit",
				  "Failed to quit voicemanager module"))
		local_err = *error;

	_s_serialization_quit(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_modules_quit",
				  "Failed to quit serialization module"))
		local_err = *error;

	_s_pluginmanager_quit(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_modules_quit",
				  "Failed to quit pluginmanager module"))
		local_err = *error;

	_s_hrg_quit(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_modules_quit",
				  "Failed to quit hrg module"))
		local_err = *error;

	_s_utils_quit(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_modules_quit",
				  "Failed to quit utils module"))
		local_err = *error;

	_s_datasources_quit(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_modules_quit",
				  "Failed to quit datasources module"))
		local_err = *error;

	_s_containers_quit(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_modules_quit",
				  "Failed to quit containers module"))
		local_err = *error;

	_s_base_quit(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_modules_quit",
				  "Failed to quit base module"))
		local_err = *error;

	/* if there was an error local_err will have it */
	if ((local_err != S_SUCCESS) && (*error == S_SUCCESS))
		*error = local_err;
}

