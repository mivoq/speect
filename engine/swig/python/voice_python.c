/************************************************************************************/
/* Copyright (c) 2009-2011 The Department of Arts and Culture,                      */
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
	PyObject *uttTypes;
	PyObject *uttProcessors;
	PyObject *dataObjects;
	PyObject *featProcessors;


	SVoice(const char *path, s_bool load_data=TRUE, s_erc *error)
	{
		SVoice *voice;
		PyObject *osPathModule;
		PyObject *abspathFunction;
		PyObject *splitextFunction;
		PyObject *splitFunction;
		PyObject *pyPath;
		PyObject *tmpObject1;
		PyObject *tmpObject2;
		const char *dir = NULL;
		const char *ext = NULL;
		const char *filename = NULL;
		char *full_path;
		int rv;


		if (path == NULL)
		{
			S_CTX_ERR(error, S_ARGERROR,
					  "SVoice",
					  "Argument \"path\" is NULL");
			return NULL;
		}

		osPathModule = PyImport_ImportModule("os.path");
		if (osPathModule == NULL)
		{
			char *py_error = s_get_python_error_str();

			if (py_error)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "SVoice",
						  "Call to \"PyImport_ImportModule\" failed. Reported error: %s",
						  py_error);
				S_FREE(py_error);
			}
			else
			{
				S_CTX_ERR(error, S_FAILURE,
						  "SVoice",
						  "Call to \"PyImport_ImportModule\" failed");
			}

			return NULL;
		}

		/* get abspath function */
		abspathFunction = PyObject_GetAttrString(osPathModule, "abspath");
		if (osPathModule == NULL)
		{
			char *py_error = s_get_python_error_str();

			if (py_error)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "SVoice",
						  "Call to \"PyObject_GetAttrString\" failed. Reported error: %s",
						  py_error);
				S_FREE(py_error);
			}
			else
			{
				S_CTX_ERR(error, S_FAILURE,
						  "SVoice",
						  "Call to \"PyObject_GetAttrString\" failed");
			}

			Py_DECREF(osPathModule);
			return NULL;
		}

		/* get splitext function */
		splitextFunction = PyObject_GetAttrString(osPathModule, "splitext");
		if (osPathModule == NULL)
		{
			char *py_error = s_get_python_error_str();

			if (py_error)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "SVoice",
						  "Call to \"PyObject_GetAttrString\" failed. Reported error: %s",
						  py_error);
				S_FREE(py_error);
			}
			else
			{
				S_CTX_ERR(error, S_FAILURE,
						  "SVoice",
						  "Call to \"PyObject_GetAttrString\" failed");
			}

			Py_DECREF(abspathFunction);
			Py_DECREF(osPathModule);
			return NULL;
		}

		/* get split function */
		splitFunction = PyObject_GetAttrString(osPathModule, "split");
		if (osPathModule == NULL)
		{
			char *py_error = s_get_python_error_str();

			if (py_error)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "SVoice",
						  "Call to \"PyObject_GetAttrString\" failed. Reported error: %s",
						  py_error);
				S_FREE(py_error);
			}
			else
			{
				S_CTX_ERR(error, S_FAILURE,
						  "SVoice",
						  "Call to \"PyObject_GetAttrString\" failed");
			}

			Py_DECREF(splitextFunction);
			Py_DECREF(abspathFunction);
			Py_DECREF(osPathModule);
			return NULL;
		}

		/* run abspath on the given path */
		pyPath = PyObject_CallFunction(abspathFunction, "s", path);
		if (pyPath == NULL)
		{
			char *py_error = s_get_python_error_str();

			if (py_error)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "SVoice",
						  "Call to \"PyObject_CallFunction\" failed. Reported error: %s",
						  py_error);
				S_FREE(py_error);
			}
			else
			{
				S_CTX_ERR(error, S_FAILURE,
						  "SVoice",
						  "Call to \"PyObject_CallFunction\" failed");
			}

			Py_DECREF(splitFunction);
			Py_DECREF(splitextFunction);
			Py_DECREF(abspathFunction);
			Py_DECREF(osPathModule);
			return NULL;
		}

		/* get the full path */
		full_path = s_get_pyobject_str(pyPath, error);
		Py_DECREF(pyPath);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SVoice",
					  "Call to \"s_get_pyobject_str\" failed"))
		{
			Py_DECREF(splitFunction);
			Py_DECREF(splitextFunction);
			Py_DECREF(abspathFunction);
			Py_DECREF(osPathModule);
			return NULL;
		}

		/* split the path into the directory name and file name */
		tmpObject1 = PyObject_CallFunction(splitFunction, "s", full_path);
		if (tmpObject1 == NULL)
		{
			char *py_error = s_get_python_error_str();

			if (py_error)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "SVoice",
						  "Call to \"PyObject_CallFunction\" failed. Reported error: %s",
						  py_error);
				S_FREE(py_error);
			}
			else
			{
				S_CTX_ERR(error, S_FAILURE,
						  "SVoice",
						  "Call to \"PyObject_CallFunction\" failed");
			}

			S_FREE(full_path);
			Py_DECREF(splitFunction);
			Py_DECREF(splitextFunction);
			Py_DECREF(abspathFunction);
			Py_DECREF(osPathModule);
			return NULL;
		}

		/* extract info from returned object */
		if (!PyArg_ParseTuple(tmpObject1, "ss", &dir, &filename))
		{
			char *py_error = s_get_python_error_str();

			if (py_error)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "SVoice",
						  "Call to \"PyArg_ParseTuple\" failed. Reported error: %s",
						  py_error);
				S_FREE(py_error);
			}
			else
			{
				S_CTX_ERR(error, S_FAILURE,
						  "SVoice",
						  "Call to \"PyArg_ParseTuple\" failed");
			}

			S_FREE(full_path);
			Py_DECREF(tmpObject1);
			Py_DECREF(splitFunction);
			Py_DECREF(splitextFunction);
			Py_DECREF(abspathFunction);
			Py_DECREF(osPathModule);
			return NULL;
		}

		/* now get file extension from file name */
		tmpObject2 = PyObject_CallFunction(splitextFunction, "s", filename);
		if (tmpObject2 == NULL)
		{
			char *py_error = s_get_python_error_str();

			if (py_error)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "SVoice",
						  "Call to \"PyObject_CallFunction\" failed. Reported error: %s",
						  py_error);
				S_FREE(py_error);
			}
			else
			{
				S_CTX_ERR(error, S_FAILURE,
						  "SVoice",
						  "Call to \"PyObject_CallFunction\" failed");
			}

			S_FREE(full_path);
			Py_DECREF(tmpObject1);
			Py_DECREF(splitFunction);
			Py_DECREF(splitextFunction);
			Py_DECREF(abspathFunction);
			Py_DECREF(osPathModule);
			return NULL;
		}

		/* extract info from returned object */
		if (!PyArg_ParseTuple(tmpObject2, "ss", &filename, &ext))
		{
			char *py_error = s_get_python_error_str();

			if (py_error)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "SVoice",
						  "Call to \"PyArg_ParseTuple\" failed. Reported error: %s",
						  py_error);
				S_FREE(py_error);
			}
			else
			{
				S_CTX_ERR(error, S_FAILURE,
						  "SVoice",
						  "Call to \"PyArg_ParseTuple\" failed");
			}

			S_FREE(full_path);
			Py_DECREF(tmpObject1);
			Py_DECREF(tmpObject2);
			Py_DECREF(splitFunction);
			Py_DECREF(splitextFunction);
			Py_DECREF(abspathFunction);
			Py_DECREF(osPathModule);
			return NULL;
		}

		/* Check if we were given a Python file */
		rv = s_strcmp(ext, ".py", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SVoice",
					  "Call to \"s_strscmp\" failed"))
		{
			S_FREE(full_path);
			Py_DECREF(tmpObject1);
			Py_DECREF(tmpObject2);
			Py_DECREF(splitFunction);
			Py_DECREF(splitextFunction);
			Py_DECREF(abspathFunction);
			Py_DECREF(osPathModule);
			return NULL;
		}

		if (rv == 0) /* it's a Python file */
		{
			PyObject   *sysModule;
			PyObject   *voiceModule;
			PyObject   *pathList;
			PyObject   *modulesDict;
			PyObject   *tmp;
			PyObject   *loadedVoice;
			Py_ssize_t  sp_size;


			/* load the sys module */
			sysModule = PyImport_ImportModule("sys");
			if (sysModule == NULL)
			{
				char *py_error = s_get_python_error_str();

				if (py_error)
				{
					S_CTX_ERR(error, S_FAILURE,
							  "SVoice",
							  "Call to \"PyImport_ImportModule\" failed. Reported error: %s",
							  py_error);
					S_FREE(py_error);
				}
				else
				{
					S_CTX_ERR(error, S_FAILURE,
							  "SVoice",
							  "Call to \"PyImport_ImportModule\" failed");
				}

				S_FREE(full_path);
				Py_DECREF(tmpObject1);
				Py_DECREF(tmpObject2);
				Py_DECREF(splitFunction);
				Py_DECREF(splitextFunction);
				Py_DECREF(abspathFunction);
				Py_DECREF(osPathModule);

				return NULL;
			}

			/* get the path list */
			pathList =  PyObject_GetAttrString(sysModule, "path");
			if (pathList == NULL)
			{
				char *py_error = s_get_python_error_str();

				if (py_error)
				{
					S_CTX_ERR(error, S_FAILURE,
							  "SVoice",
							  "Call to \"PyObject_GetAttrString\" failed. Reported error: %s",
							  py_error);
					S_FREE(py_error);
				}
				else
				{
					S_CTX_ERR(error, S_FAILURE,
							  "SVoice",
							  "Call to \"PyObject_GetAttrString\" failed");
				}

				S_FREE(full_path);
				Py_DECREF(tmpObject1);
				Py_DECREF(tmpObject2);
				Py_DECREF(splitFunction);
				Py_DECREF(splitextFunction);
				Py_DECREF(abspathFunction);
				Py_DECREF(osPathModule);
				Py_DECREF(sysModule);
				return NULL;
			}

			/* now add the dir to the system path */
			tmp = s_set_pyobject_str(dir, error);
			if (S_CHK_ERR(error, S_CONTERR,
						  "SVoice",
						  "Call to \"s_set_pyobject_str\" failed"))
			{

				S_FREE(full_path);
				Py_DECREF(pathList);
				Py_DECREF(tmpObject1);
				Py_DECREF(tmpObject2);
				Py_DECREF(splitFunction);
				Py_DECREF(splitextFunction);
				Py_DECREF(abspathFunction);
				Py_DECREF(osPathModule);
				Py_DECREF(sysModule);
				return NULL;
			}

			if (PyList_Append(pathList, tmp) == -1)
			{
				char *py_error = s_get_python_error_str();

				if (py_error)
				{
					S_CTX_ERR(error, S_FAILURE,
							  "SVoice",
							  "Call to \"PyList_Append\" failed. Reported error: %s",
							  py_error);
					S_FREE(py_error);
				}
				else
				{
					S_CTX_ERR(error, S_FAILURE,
							  "SVoice",
							  "Call to \"PyList_Append\" failed");
				}

				S_FREE(full_path);
				Py_DECREF(pathList);
				Py_DECREF(tmp);
				Py_DECREF(tmpObject1);
				Py_DECREF(tmpObject2);
				Py_DECREF(splitFunction);
				Py_DECREF(splitextFunction);
				Py_DECREF(abspathFunction);
				Py_DECREF(osPathModule);
				Py_DECREF(sysModule);
				return NULL;
			}

			/* done with tmp */
			Py_DECREF(tmp);

			/* now import the file */
			voiceModule = PyImport_ImportModule(filename);
			if (voiceModule == NULL)
			{
				char *py_error = s_get_python_error_str();

				if (py_error)
				{
					S_CTX_ERR(error, S_FAILURE,
							  "SVoice",
							  "Call to \"PyImport_ImportModule\" failed. Reported error: %s",
							  py_error);
					S_FREE(py_error);
				}
				else
				{
					S_CTX_ERR(error, S_FAILURE,
							  "SVoice",
							  "Call to \"PyImport_ImportModule\" failed");
				}

				S_FREE(full_path);
				Py_DECREF(pathList);
				Py_DECREF(tmpObject1);
				Py_DECREF(tmpObject2);
				Py_DECREF(splitFunction);
				Py_DECREF(splitextFunction);
				Py_DECREF(abspathFunction);
				Py_DECREF(osPathModule);
				Py_DECREF(sysModule);
				return NULL;
			}

			/* get voice loading function */
			tmp = PyObject_GetAttrString(voiceModule, "load_voice");
			if (tmp == NULL)
			{
				char *py_error = s_get_python_error_str();

				if (py_error)
				{
					S_CTX_ERR(error, S_FAILURE,
							  "SVoice",
							  "Call to \"PyObject_GetAttrString\" failed. Reported error: %s",
							  py_error);
					S_FREE(py_error);
				}
				else
				{
					S_CTX_ERR(error, S_FAILURE,
							  "SVoice",
							  "Call to \"PyObject_GetAttrString\" failed");
				}

				S_FREE(full_path);
				Py_DECREF(pathList);
				Py_DECREF(tmpObject1);
				Py_DECREF(tmpObject2);
				Py_DECREF(splitFunction);
				Py_DECREF(splitextFunction);
				Py_DECREF(abspathFunction);
				Py_DECREF(osPathModule);
				Py_DECREF(sysModule);
				Py_DECREF(voiceModule);
				return NULL;
			}

			/* and run it, no arguments */
			loadedVoice = PyObject_CallFunctionObjArgs(tmp, NULL, NULL);
			if (loadedVoice == NULL)
			{
				char *py_error = s_get_python_error_str();

				if (py_error)
				{
					S_CTX_ERR(error, S_FAILURE,
							  "SVoice",
							  "Call to \"PyObject_CallFunctionObjArgs\" failed. Reported error: %s",
							  py_error);
					S_FREE(py_error);
				}
				else
				{
					S_CTX_ERR(error, S_FAILURE,
							  "SVoice",
							  "Call to \"PyObject_CallFunctionObjArgs\" failed");
				}

				S_FREE(full_path);
				Py_DECREF(pathList);
				Py_DECREF(tmp);
				Py_DECREF(tmpObject1);
				Py_DECREF(tmpObject2);
				Py_DECREF(splitFunction);
				Py_DECREF(splitextFunction);
				Py_DECREF(abspathFunction);
				Py_DECREF(osPathModule);
				Py_DECREF(sysModule);
				Py_DECREF(voiceModule);
				return NULL;
			}

			/* get voice */
			voice = (SVoice*)s_pyobject_2_sobject(loadedVoice, error);
			Py_DECREF(loadedVoice);
			if (S_CHK_ERR(error, S_CONTERR,
						  "SVoice",
						  "Call to \"s_pyobject_2_sobject\" failed"))
			{
				S_FREE(full_path);
				Py_DECREF(pathList);
				Py_DECREF(tmp);
				Py_DECREF(tmpObject1);
				Py_DECREF(tmpObject2);
				Py_DECREF(splitFunction);
				Py_DECREF(splitextFunction);
				Py_DECREF(abspathFunction);
				Py_DECREF(osPathModule);
				Py_DECREF(sysModule);
				Py_DECREF(voiceModule);
				return NULL;
			}

			/* remove the dir from the system path */
			sp_size = PyList_Size(pathList);
			if (PySequence_DelItem(pathList, sp_size - 1) == -1)
			{
				char *py_error = s_get_python_error_str();

				if (py_error)
				{
					S_CTX_ERR(error, S_FAILURE,
							  "SVoice",
							  "Call to \"PySequence_DelItem\" failed. Reported error: %s",
							  py_error);
					S_FREE(py_error);
				}
				else
				{
					S_CTX_ERR(error, S_FAILURE,
							  "SVoice",
							  "Call to \"PySequence_DelItem\" failed");
				}

				S_FREE(full_path);
				Py_DECREF(pathList);
				Py_DECREF(tmp);
				Py_DECREF(tmpObject1);
				Py_DECREF(tmpObject2);
				Py_DECREF(splitFunction);
				Py_DECREF(splitextFunction);
				Py_DECREF(abspathFunction);
				Py_DECREF(osPathModule);
				Py_DECREF(sysModule);
				Py_DECREF(voiceModule);
				return NULL;
			}

			/* remove reference to system path */
			Py_DECREF(pathList);

			/*
			 * also remove the named module from the sys.modules dict
			 * get sys.modules dict
			 */
			modulesDict = PyObject_GetAttrString(sysModule, "modules");
			if (modulesDict == NULL)
			{
				char *py_error = s_get_python_error_str();

				if (py_error)
				{
					S_CTX_ERR(error, S_FAILURE,
							  "SVoice",
							  "Call to \"PyObject_GetAttrString\" failed. Reported error: %s",
							  py_error);
					S_FREE(py_error);
				}
				else
				{
					S_CTX_ERR(error, S_FAILURE,
							  "SVoice",
							  "Call to \"PyObject_GetAttrString\" failed");
				}

				S_FREE(full_path);
				Py_DECREF(tmp);
				Py_DECREF(tmpObject1);
				Py_DECREF(tmpObject2);
				Py_DECREF(splitFunction);
				Py_DECREF(splitextFunction);
				Py_DECREF(abspathFunction);
				Py_DECREF(osPathModule);
				Py_DECREF(sysModule);
				Py_DECREF(voiceModule);
				return NULL;
			}

			if (PyDict_DelItemString(modulesDict, filename) != 0)
			{
				char *py_error = s_get_python_error_str();

				if (py_error)
				{
					S_CTX_ERR(error, S_FAILURE,
							  "SVoice",
							  "Call to \"PyDict_DelItemString\" failed. Reported error: %s",
							  py_error);
					S_FREE(py_error);
				}
				else
				{
					S_CTX_ERR(error, S_FAILURE,
							  "SVoice",
							  "Call to \"PyDict_DelItemString\" failed");
				}

				S_FREE(full_path);
				Py_DECREF(tmp);
				Py_DECREF(tmpObject1);
				Py_DECREF(tmpObject2);
				Py_DECREF(splitFunction);
				Py_DECREF(splitextFunction);
				Py_DECREF(abspathFunction);
				Py_DECREF(osPathModule);
				Py_DECREF(sysModule);
				Py_DECREF(voiceModule);
				return NULL;
			}

			Py_DECREF(modulesDict);

			/* clean and return */
			S_FREE(full_path);
			Py_DECREF(tmp);
			Py_DECREF(tmpObject1);
			Py_DECREF(tmpObject2);
			Py_DECREF(splitFunction);
			Py_DECREF(splitextFunction);
			Py_DECREF(abspathFunction);
			Py_DECREF(osPathModule);
			Py_DECREF(sysModule);
			Py_DECREF(voiceModule);

			return voice;
		}

		/* it's not a Python file, do normal loading */
		voice = s_vm_load_voice(full_path, load_data, error);
		S_FREE(full_path);
		Py_DECREF(tmpObject1);
		Py_DECREF(tmpObject2);
		Py_DECREF(splitFunction);
		Py_DECREF(splitextFunction);
		Py_DECREF(abspathFunction);
		Py_DECREF(osPathModule);
		if (*error != S_SUCCESS)
			return NULL;

		return voice;
	}


	void uttType_set(const char *key, PyObject *uttType, s_erc *error)
	{
		SObject *object;


		object = s_pyobject_2_sobject(uttType, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SVoice::uttType_set",
					  "Call to \"s_pyobject_2_sobject\" failed"))
			return;

		SVoiceSetUttType($self, key, S_LIST(object), error);
		if (*error != S_SUCCESS)
			S_DELETE(object, "SVoice::uttType_set", error);
	}


	PyObject *uttType_get(const char *key, s_erc *error)
	{
		const SList *uttType;
		PyObject *plist;


		uttType = SVoiceGetUttType($self, key, error);
		if (*error != S_SUCCESS)
			return NULL;

		plist = s_sobject_2_pyobject(S_OBJECT(uttType), FALSE, error);
		if (*error != S_SUCCESS)
			return NULL;

		return plist;
	}


	void data_set(const char *key, PyObject *pobject, s_erc *error)
	{
		SObject *object;


		object = s_pyobject_2_sobject(pobject, error);
		if (*error != S_SUCCESS)
			return;

		SVoiceSetData($self, key, object, error);
		if (*error != S_SUCCESS)
			S_DELETE(object, "SVoice::data_set", error);
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



%{
	PyObject *SVoice_uttTypes_get(const SVoice *voice)
	{
		s_erc error;
		SList *uttTypes;
		PyObject *pobject;


		S_CLR_ERR(&error);
		uttTypes = SVoiceGetUttTypesKeys(voice, &error);
		if (error != S_SUCCESS)
			return NULL;

		pobject = s_sobject_2_pyobject(S_OBJECT(uttTypes), TRUE, &error);
		if (error != S_SUCCESS)
		{
			S_DELETE(uttTypes, "SVoice_uttTypes_get", &error);
			return NULL;
		}

		return pobject;
	}


	void SVoice_uttTypes_set(const SVoice *voice, PyObject *list)
	{
		s_erc error;


		S_CLR_ERR(&error);
	}


	PyObject *SVoice_uttProcessors_get(const SVoice *voice)
	{
		s_erc error;
		SList *uttProcessors;
		PyObject *pobject;


		S_CLR_ERR(&error);
		uttProcessors = SVoiceGetUttProcKeys(voice, &error);
		if (error != S_SUCCESS)
			return NULL;

		pobject = s_sobject_2_pyobject(S_OBJECT(uttProcessors), TRUE, &error);
		if (error != S_SUCCESS)
		{
			S_DELETE(uttProcessors, "SVoice_uttProcessors_get", &error);
			return NULL;
		}

		return pobject;
	}


	void SVoice_uttProcessors_set(const SVoice *voice, PyObject *list)
	{
		s_erc error;


		S_CLR_ERR(&error);
	}


	PyObject *SVoice_dataObjects_get(const SVoice *voice)
	{
		s_erc error;
		SList *data;
		PyObject *pobject;


		S_CLR_ERR(&error);
		data = SVoiceGetDataKeys(voice, &error);
		if (error != S_SUCCESS)
			return NULL;

		pobject = s_sobject_2_pyobject(S_OBJECT(data), TRUE, &error);
		if (error != S_SUCCESS)
		{
			S_DELETE(data, "SVoice_dataObjects_get", &error);
			return NULL;
		}

		return pobject;
	}


	void SVoice_dataObjects_set(const SVoice *voice, PyObject *list)
	{
		s_erc error;


		S_CLR_ERR(&error);
	}


	PyObject *SVoice_featProcessors_get(const SVoice *voice)
	{
		s_erc error;
		SList *featProcessors;
		PyObject *pobject;


		S_CLR_ERR(&error);
		featProcessors = SVoiceGetFeatProcKeys(voice, &error);
		if (error != S_SUCCESS)
			return NULL;

		pobject = s_sobject_2_pyobject(S_OBJECT(featProcessors), TRUE, &error);
		if (error != S_SUCCESS)
		{
			S_DELETE(featProcessors, "SVoice_featProcessors_get", &error);
			return NULL;
		}

		return pobject;
	}


	void SVoice_featProcessors_set(const SVoice *voice, PyObject *list)
	{
		s_erc error;


		S_CLR_ERR(&error);
	}
%}
