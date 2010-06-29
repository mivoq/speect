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
/* DATE    : November 2009                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* C convenience functions for SVoice Python wrapper.                               */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


%extend SVoice
{
	void uttType_set(const char *key, PyObject *uttType, s_erc *error)
	{
		SList *pyList;


		S_CLR_ERR(error);
		pyList = (SList*)s_pyobject_2_sobject(uttType, error);
		Py_DECREF(uttType);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SVoice::uttType_set",
					  "Call to \"s_pyobject_2_sobject\" failed"))
			return;

		SVoiceSetUttType($self, key, pyList, error);
		S_DELETE(pyList, "SVoice::uttType_set", error);
	}


	PyObject *data_get(const char *key, s_erc *error)
	{
		const SObject *sdata;
		PyObject *pdata;



		sdata = SVoiceGetData($self, key, error);
		if (*error != S_SUCCESS)
			return NULL;

		pdata = s_sobject_2_pyobject(sdata, FALSE, error);
		if (*error != S_SUCCESS)
			return NULL;

		return pdata;
	}
};
