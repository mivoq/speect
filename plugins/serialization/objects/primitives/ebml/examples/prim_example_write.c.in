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
/* DATE    : 17 November 2009                                                       */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* Simple EBML writing example.                                                     */
/* example_write.c is automatically created from example_write.c.in                 */
/*                                                                                  */
/************************************************************************************/

#include <stdio.h>
#include "speect.h"
#include "ebml_write.h"


static const char *plugin_path = "prim-ebml.spi";


int main()
{
	s_erc error;
	SPlugin *plugin = NULL;
	const char *doc_type = "spct_ebml_test";
	SDatasource *ds = NULL;
	s_ebml_header *header = NULL;
	SEbmlWrite *ebmlWriter = NULL;
	SObject *object;


	S_CLR_ERR(&error);

	/*
	 * initialize speect
	 */
	error = speect_init();
	if (error != S_SUCCESS)
	{
		printf("Failed to initialize Speect\n");
		return 1;
	}

	/*
	 * load the prim-ebml plug-in
	 */
	plugin = s_pm_load_plugin(plugin_path, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to load plug-in at '%s'", plugin_path))
	{
		printf("failed to load plug-in\n");
		goto quit;
	}
	else
	{
		printf("plug-in loaded\n");
	}

	/*
	 * create a data source, "wb" = write, binary
	 */
	ds = SFilesourceOpenFile("prim_ebml_test.bin", "wb", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create data source 'prim_ebml_test.bin'"))
	{
		printf("failed to create data source\n");
		goto quit;
	}
	else
	{
		printf("data source created\n");
	}

	/*
	 * create ebml header
	 */
	header = S_MALLOC(s_ebml_header, 1);
	if (header == NULL)
	{
		S_FTL_ERR(&error, S_MEMERROR,
				  "main",
				  "Failed to allocate memory for s_ebml_header object");
		goto quit;
	}
	header->ebml_version = 1;
	header->ebml_read_version = 1;
	header->max_id_width = 4;
	header->max_size_width = 4;
	header->doctype_version = 1;
	header->doctype_read_version = 1;
	header->doctype = s_strdup(doc_type, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to duplicate 'doc_type' for ebml header"))
		goto quit;


	/*
	 * create ebml writer object
	 */
	ebmlWriter = (SEbmlWrite*)S_NEW("SEbmlWrite", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create new SEbmlWrite object"))
	{
		printf("failed to create SEbmlWrite object\n");
		goto quit;
	}
	else
	{
		printf("created SEbmlWrite object\n");
	}

	/*
	 * initialize ebml writer object
	 */
	if (S_EBMLWRITE_METH_VALID(ebmlWriter, write_init))
	{
		S_EBMLWRITE_CALL(ebmlWriter, write_init)(&ebmlWriter, ds, header, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to initialize SEbmlWrite object"))
		{
			printf("failed to initialize SEbmlWrite object");
			goto quit;
		}
		else
		{
			printf("initialized SEbmlWrite object\n");

			/*
			 * ebml writer takes hold of the header and the data
			 * source, we dont want a reference to it anymore (for
			 * quit to work).
			 */
			header = NULL;
			ds = NULL;
		}
	}
	else
	{
		S_CTX_ERR(&error, S_METHINVLD,
				  "main",
				  "EbmlWrite method \"write_init\" not implemented");
		printf("cannot do write_init");
		goto quit;
	}

	/*
	 * Start a container
	 */
	if (S_EBMLWRITE_METH_VALID(ebmlWriter, start_container))
	{
		S_EBMLWRITE_CALL(ebmlWriter, start_container)(ebmlWriter, 129, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to start ebml container"))
		{
			printf("failed to start container\n");
			goto quit;
		}
		else
		{
			printf("container started, id = %d\n", 129);
		}
	}

	/*
	 * Create a int object
	 */
	object = SObjectSetInt(21, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create int object"))
	{
		printf("failed to create int object\n");
		goto quit;
	}
	else
	{
		printf("created int object (21)\n");
	}

	/*
	 * Write int object
	 */
	if (S_EBMLWRITE_METH_VALID(ebmlWriter, write_object))
	{
		S_EBMLWRITE_CALL(ebmlWriter, write_object)(ebmlWriter, 129, object, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to write int object"))
		{
			printf("failed to write int object\n");
			goto quit;
		}
		else
		{
			printf("wrote int object, id 129\n");
		}
	}

	/*
	 * Delete int object
	 */
	S_DELETE(object, "main", &error);

	/*
	 * Create a float object
	 */
	object = SObjectSetFloat(-3.187, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create float object"))
	{
		printf("failed to create float object\n");
		goto quit;
	}
	else
	{
		printf("created float object (-3.187)\n");
	}

	/*
	 * Write float object
	 */
	if (S_EBMLWRITE_METH_VALID(ebmlWriter, write_object))
	{
		S_EBMLWRITE_CALL(ebmlWriter, write_object)(ebmlWriter, 130, object, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to write float object"))
		{
			printf("failed to write float object\n");
			goto quit;
		}
		else
		{
			printf("wrote float object, id 130\n");
		}
	}

	/*
	 * Delete float object
	 */
	S_DELETE(object, "main", &error);

	/*
	 * Create a string object
	 */
	object = SObjectSetString("hello world", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create string object"))
	{
		printf("failed to create string object\n");
		goto quit;
	}
	else
	{
		printf("created string object (\"hello world\")\n");
	}

	/*
	 * Write string object
	 */
	if (S_EBMLWRITE_METH_VALID(ebmlWriter, write_object))
	{
		S_EBMLWRITE_CALL(ebmlWriter, write_object)(ebmlWriter, 131, object, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to write string object"))
		{
			printf("failed to write string object\n");
			goto quit;
		}
		else
		{
			printf("wrote string object, id 131\n");
		}
	}

	/*
	 * Delete string object
	 */
	S_DELETE(object, "main", &error);


	/*
	 * Close container
	 */
	if (S_EBMLWRITE_METH_VALID(ebmlWriter, stop_container))
	{
		S_EBMLWRITE_CALL(ebmlWriter, stop_container)(ebmlWriter, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to stop ebml container"))
		{
			printf("failed to stop container\n");
			goto quit;
		}
		else
		{
			printf("container stop\n");
		}
	}


quit:
	if (ds != NULL)
		S_DELETE(ds, "main", &error);

	if (header != NULL)
	{
		if (header->doctype != NULL)
			S_FREE(header->doctype);
		S_FREE(header);
	}

	if (ebmlWriter != NULL)
		S_DELETE(ebmlWriter, "main", &error);

	if (object != NULL)
		S_DELETE(object, "main", &error);

	if (plugin != NULL)
		S_DELETE(plugin, "main", &error);

	/*
	 * quit speect
	 */
	error = speect_quit();
	if (error != S_SUCCESS)
	{
		printf("Call to 'speect_quit' failed\n");
		return 1;
	}


	return 0;
}
