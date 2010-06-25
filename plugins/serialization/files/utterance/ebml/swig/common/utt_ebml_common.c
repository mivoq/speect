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
/* SWIG common C convenience functions for SUtterance EBML.                         */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Inline helper functions                                                          */
/*                                                                                  */
/************************************************************************************/

%inline
%{
	void _save_utterance_ebml(SUtterance *utt, const char *path, s_erc *error)
	{
		S_CLR_ERR(error);
		if (utt == NULL)
		{
			S_CTX_ERR(error, S_ARGERROR,
					  "_save_utterance_ebml",
					  "Argument \"utt\" is NULL");
			return;
		}

		if (path == NULL)
		{
			S_CTX_ERR(error, S_ARGERROR,
					  "_save_utterance_ebml",
					  "Argument \"path\" is NULL");
			return;
		}

		SObjectSave(S_OBJECT(utt), path, "spct_utt", error);
		S_CHK_ERR(error, S_CONTERR,
				  "_save_utterance_ebml",
				  "Call to \"SObjectSave\" failed");
	}


	SUtterance *_load_utterance_ebml(const char *path, s_erc *error)
	{
		SObject *loadedUtt;


		S_CLR_ERR(error);
		if (path == NULL)
		{
			S_CTX_ERR(error, S_ARGERROR,
					  "_load_utterance_ebml",
					  "Argument \"path\" is NULL");
			return NULL;
		}

		loadedUtt = SObjectLoad(path, "spct_utt", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_load_utterance_ebml",
					  "Call to \"SObjectLoad\" failed"))
			return NULL;

		return S_UTTERANCE(loadedUtt);
	}
%}

