/************************************************************************************/
/* Copyright (c) 2010 The Department of Arts and Culture,                           */
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
/* DATE    : March 2010                                                             */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* C convenience functions for extra plug-in relation Python functions.             */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Inline helper functions.                                                         */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

%inline
%{
	PyObject *_s_get_plugin_info(const char *path, s_erc *error)
	{
		SDso *pluginDso;
		char *new_path;
		PyObject *info;
		const s_plugin_init_fp plugin_initialize;
		const s_plugin_params *plugin_info;
		PyObject *tmp;


		S_CLR_ERR(error);
		if (path == NULL)
		{
			S_CTX_ERR(error, S_ARGERROR,
					  "_s_get_plugin_info",
					  "Argument \"path\" is NULL");
			return NULL;
		}

		pluginDso = (SDso*)S_NEW("SDso", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_get_plugin_info",
					  "Failed to create new dynamic shared object for plug-in"))
			return NULL;

		new_path = s_pm_get_plugin_path(path, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_get_plugin_info",
					  "Call to \"s_pm_get_plugin_path\" failed"))
		{
			S_DELETE(pluginDso, "_s_get_plugin_info", error);
			return NULL;
		}


		SDsoLoad(pluginDso, new_path, error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_get_plugin_info",
					  "Failed to load plug-in dynamic shared object at \"%s\"",
					  new_path))
		{
			S_DELETE(pluginDso, "_s_get_plugin_info", error);
			S_FREE(new_path);
			return NULL;
		}

		/*
		 * get the plugin initialization function
		 * done as described in dlsym man page.
		 */
		*(void**)(&plugin_initialize) = SDsoGetSymbol(pluginDso, "s_plugin_init", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_get_plugin_info",
					  "Failed to get \'s_plugin_init\' symbol from plug-in at \"%s\"",
					  new_path))
		{
			S_DELETE(pluginDso, "_s_get_plugin_info", error);
			S_FREE(new_path);
			return NULL;
		}

		if (plugin_initialize == NULL)
		{
			S_CTX_ERR(error, S_FAILURE,
					  "_s_get_plugin_info",
					  "Plug-in symbol \'s_plugin_init\' is NULL for plug-in at \"%s\"",
					  new_path);
			S_DELETE(pluginDso, "_s_get_plugin_info", error);
			S_FREE(new_path);
			return NULL;
		}

		plugin_info = (plugin_initialize)(error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_get_plugin_info",
					  "Call to \"s_plugin_init\" failed"))
		{
			S_DELETE(pluginDso, "_s_get_plugin_info", error);
			S_FREE(new_path);
			return NULL;
		}

		info = PyDict_New();
		if (info == NULL)
		{
			char *py_error = s_get_python_error_str();

			if (py_error)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "_s_get_plugin_info",
						  "Call to \"PyDict_New\" failed. Reported error: %s",
						  py_error);
				S_FREE(py_error);
			}
			else
			{
				S_CTX_ERR(error, S_FAILURE,
						  "_s_get_plugin_info",
						  "Call to \"PyDict_New\" failed");
			}

			S_DELETE(pluginDso, "_s_get_plugin_info", error);
			S_FREE(new_path);
			return NULL;
		}

		if (plugin_info->name != NULL)
			tmp = s_set_pyobject_str(plugin_info->name, error);
		else
			tmp = s_set_pyobject_str("unknown", error);

		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_get_plugin_info",
					  "Call to \"s_set_pyobject_str\" failed"))
		{
			S_DELETE(pluginDso, "_s_get_plugin_info", error);
			S_FREE(new_path);
			Py_DECREF(info);
			return NULL;
		}

		if (PyDict_SetItemString(info, "name", tmp) != 0)
		{
			char *py_error = s_get_python_error_str();

			if (py_error)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "_s_get_plugin_info",
						  "Call to \"PyDict_SetItemString\" failed. Reported error: %s",
						  py_error);
				S_FREE(py_error);
			}
			else
			{
				S_CTX_ERR(error, S_FAILURE,
						  "_s_get_plugin_info",
						  "Call to \"PyDict_SetItemString\" failed");
			}

			S_DELETE(pluginDso, "_s_get_plugin_info", error);
			S_FREE(new_path);
			Py_DECREF(info);
			Py_DECREF(tmp);
			return NULL;
		}

		Py_DECREF(tmp);
		if (plugin_info->description != NULL)
			tmp = s_set_pyobject_str(plugin_info->description, error);
		else
			tmp = s_set_pyobject_str("unknown", error);

		if (S_CHK_ERR(error, S_CONTERR,
					  "_s_get_plugin_info",
					  "Call to \"s_set_pyobject_str\" failed"))
		{
			S_DELETE(pluginDso, "_s_get_plugin_info", error);
			S_FREE(new_path);
			Py_DECREF(info);
			return NULL;
		}

		if (PyDict_SetItemString(info, "description", tmp) != 0)
		{
			char *py_error = s_get_python_error_str();

			if (py_error)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "_s_get_plugin_info",
						  "Call to \"PyDict_SetItemString\" failed. Reported error: %s",
						  py_error);
				S_FREE(py_error);
			}
			else
			{
				S_CTX_ERR(error, S_FAILURE,
						  "_s_get_plugin_info",
						  "Call to \"PyDict_SetItemString\" failed");
			}

			S_DELETE(pluginDso, "_s_get_plugin_info", error);
			S_FREE(new_path);
			Py_DECREF(info);
			Py_DECREF(tmp);
			return NULL;
		}

		Py_DECREF(tmp);
		{
			PyObject *versionMajor;
			PyObject *versionMinor;


			tmp = PyTuple_New(2);
			versionMajor = PyLong_FromLong((long)plugin_info->version.major);
			versionMinor = PyLong_FromLong((long)plugin_info->version.minor);

			PyTuple_SetItem(tmp, 0, versionMajor);
			PyTuple_SetItem(tmp, 1, versionMinor);
		}

		if (PyDict_SetItemString(info, "plug-in version", tmp) != 0)
		{
			char *py_error = s_get_python_error_str();

			if (py_error)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "_s_get_plugin_info",
						  "Call to \"PyDict_SetItemString\" failed. Reported error: %s",
						  py_error);
				S_FREE(py_error);
			}
			else
			{
				S_CTX_ERR(error, S_FAILURE,
						  "_s_get_plugin_info",
						  "Call to \"PyDict_SetItemString\" failed");
			}

			S_DELETE(pluginDso, "_s_get_plugin_info", error);
			S_FREE(new_path);
			Py_DECREF(info);
			Py_DECREF(tmp);
			return NULL;
		}

		Py_DECREF(tmp);
		{
			PyObject *versionMajor;
			PyObject *versionMinor;


			tmp = PyTuple_New(2);
			versionMajor = PyLong_FromLong((long)plugin_info->s_abi.major);
			versionMinor = PyLong_FromLong((long)plugin_info->s_abi.minor);

			PyTuple_SetItem(tmp, 0, versionMajor);
			PyTuple_SetItem(tmp, 1, versionMinor);
		}

		if (PyDict_SetItemString(info, "Speect ABI version", tmp) != 0)
		{
			char *py_error = s_get_python_error_str();

			if (py_error)
			{
				S_CTX_ERR(error, S_FAILURE,
						  "_s_get_plugin_info",
						  "Call to \"PyDict_SetItemString\" failed. Reported error: %s",
						  py_error);
				S_FREE(py_error);
			}
			else
			{
				S_CTX_ERR(error, S_FAILURE,
						  "_s_get_plugin_info",
						  "Call to \"PyDict_SetItemString\" failed");
			}

			S_DELETE(pluginDso, "_s_get_plugin_info", error);
			S_FREE(new_path);
			Py_DECREF(info);
			Py_DECREF(tmp);
			return NULL;
		}

		Py_DECREF(tmp);
		S_DELETE(pluginDso, "_s_get_plugin_info", error);
		S_FREE(new_path);

		return info;
	}
%}




%pythoncode
%{
def get_plugin_info(path):
    """
    Get information on the plug-in at the given path.
    If the given path does not include any path separators (just a file name)
    then the path is concatenated with the default plug-in path.

    :return: A dictionary with information on the given plug-in.
    :rtype: dict
    """
    if not isinstance(path, str):
        raise TypeError("Argument \"path\" must be a string")

    return _s_get_plugin_info(path)
%}


