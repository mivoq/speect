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
/* C convenience functions for SUttProcessor Python wrapper.                        */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/*
 * Do not delete these delimiters, required for SWIG
 */
%inline
%{
	SObject *py_suttproc_create(const char *class_name)
	{
		s_erc rv = S_SUCCESS;
		s_bool is_good;
		SObject *uttproc;

		/* check that the class is registered */
		is_good = s_class_is_reg(class_name, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_Format(PyExc_RuntimeError, "Failed to query if class is registered");
			return NULL;
		}

		if (!is_good)
		{
			PyErr_Format(PyExc_RuntimeError, "Given class name is not registered");
			return NULL;
		}

		/* create the utterance processor */
		uttproc = S_NEW(class_name, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_Format(PyExc_RuntimeError, "Failed to create utterance processor");
			return NULL;
		}

		/* check that it inherits from SUttProcessor */
		is_good = SObjectIsType(uttproc, "SUttProcessor", &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_Format(PyExc_RuntimeError, "Failed to query object type");
			S_DELETE(uttproc, "py_suttproc_create", &rv);
			return NULL;
		}

		if (!is_good)
		{
			PyErr_Format(PyExc_RuntimeError, "Given class name does not inherit from 'SUttProcessor'");
			S_DELETE(uttproc, "py_suttproc_create", &rv);
			return NULL;
		}

		return uttproc;
	}


	const SObject *py_suttproc_get_feature(const SObject *self, const char *featname)
	{
		s_erc rv = S_SUCCESS;
		const SObject *feature;


		feature = SUttProcessorGetFeature(S_UTTPROCESSOR(self), featname, &rv);
		if (rv != S_SUCCESS)
		{
			PyErr_Format(PyExc_RuntimeError, "Failed to get utterance processor feature");
			return NULL;
		}

		return S_OBJECT(feature);
	}


	void py_suttproc_set_feature(SObject *self, const char *featname, const SObject *feat)
	{
		s_erc rv = S_SUCCESS;


		SUttProcessorSetFeature(S_UTTPROCESSOR(self), featname, feat, &rv);
		if (rv != S_SUCCESS)
			PyErr_Format(PyExc_RuntimeError, "Failed to set utterance processor feature");
	}


	void py_suttproc_del_feature(SObject *self, const char *feat_name)
	{
		s_erc rv = S_SUCCESS;


		SUttProcessorDelFeature(S_UTTPROCESSOR(self), feat_name, &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Failed to delete feature");
	}


	const SObject *py_suttproc_features(const SObject *self)
	{
		SUttProcessor *uttProc = S_UTTPROCESSOR(self);


		if (uttProc == NULL)
			return NULL;

		return (const SObject*)uttProc->features;
	}


	void py_suttproc_run(const SObject *self, SObject *utt)
	{
		s_erc rv = S_SUCCESS;


		SUttProcessorRun(S_UTTPROCESSOR(self), S_UTTERANCE(utt), &rv);
		if (rv != S_SUCCESS)
			PyErr_SetString(PyExc_RuntimeError, "Utterance processor execution failed");
	}

/*
 * Do not delete this delimiter, required for SWIG
 */
%}
