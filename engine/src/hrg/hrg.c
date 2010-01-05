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
/* DATE    : 30 October 2008                                                        */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* HRG includes, initialization and quit.                                           */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Modules used                                                                     */
/*                                                                                  */
/************************************************************************************/

#include "hrg/hrg.h"


/************************************************************************************/
/*                                                                                  */
/* Function implementations                                                         */
/*                                                                                  */
/************************************************************************************/

S_LOCAL void _s_hrg_init(s_erc *error)
{
	s_erc local_err;

	S_CLR_ERR(&local_err);
	S_CLR_ERR(error);

	_s_itm_content_class_add(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_hrg_init",
				  "Failed to intialize SItmContent class"))
		local_err = *error;

	_s_item_class_add(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_hrg_init",
				  "Failed to intialize SItem class"))
		local_err = *error;

	_s_relation_class_add(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_hrg_init",
				  "Failed to intialize SRelation class"))
		local_err = *error;

	_s_utterance_class_add(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_hrg_init",
				  "Failed to intialize SUtterance class"))
		local_err = *error;

	_s_hrg_processors_init(error);
	if (S_CHK_ERR(error, S_CONTERR,
				  "_s_hrg_init",
				  "Failed to intialize HRG processors module"))
		local_err = *error;

	/* if there was an error local_err will have it */
	if ((local_err != S_SUCCESS) && (*error == S_SUCCESS))
		*error = local_err;
}


S_LOCAL void _s_hrg_quit(s_erc *error)
{
	S_CLR_ERR(error);

	_s_hrg_processors_quit(error);
	S_CHK_ERR(error, S_CONTERR,
			  "_s_hrg_quit",
			  "Failed to quit HRG processors module");
}

