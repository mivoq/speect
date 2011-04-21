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
/* DATE    : 18 November 2009                                                       */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* EBML example of reading Speect primitive data types (SSint, SFloat and           */
/* SString) as SObjects.                                                            */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#include <stdio.h>
#include "speect.h"
#include "ebml_read.h"


int main()
{
	s_erc error;
	SPlugin *plugin = NULL;
	const char *doc_type = "spct_ebml_test";
	SDatasource *ds = NULL;
	SEbmlRead *ebmlReader = NULL;
	SObject *object = NULL;
	int rv;


	S_CLR_ERR(&error);

	/*
	 * initialize speect
	 */
	error = speect_init(NULL);
	if (error != S_SUCCESS)
	{
		printf("Failed to initialize Speect\n");
		return 1;
	}

	/*
	 * load the prim_ebml plug-in
	 */
	plugin = s_pm_load_plugin("prim_ebml.spi", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to load plug-in at 'prim_ebml.spi'"))
	{
		printf("failed to load plug-in\n");
		goto quit;
	}
	else
	{
		printf("plug-in loaded\n");
	}

	/*
	 * create a data source, "rb" = read, binary
	 */
	ds = SFilesourceOpenFile("prim_ebml_test.bin", "rb", &error);
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
	 * create ebml reader object
	 */
	ebmlReader = (SEbmlRead*)S_NEW("SEbmlRead", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create new SEbmlRead object"))
	{
		printf("failed to create SEbmlRead object\n");
		goto quit;
	}
	else
	{
		printf("created SEbmlRead object\n");
	}

	/*
	 * initialize ebml reader object
	 */
	if (S_EBMLREAD_METH_VALID(ebmlReader, read_init))
	{
		S_EBMLREAD_CALL(ebmlReader, read_init)(&ebmlReader, ds, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to initialize SEbmlRead object"))
		{
			printf("failed to initialize SEbmlRead object\n");
			goto quit;
		}
		else
		{
			printf("initialized SEbmlRead object\n");

			/*
			 * ebml reader takes hold of the data source, we dont want
			 * a reference to it anymore (for quit to work).
			 */
			ds = NULL;
		}
	}
	else
	{
		S_CTX_ERR(&error, S_METHINVLD,
				  "main",
				  "EbmlRead method \"read_init\" not implemented");
		printf("cannot do read_init\n");
		goto quit;
	}

	/*
	 * verify doc_type
	 */
	rv = s_strcmp(ebmlReader->header->doctype, doc_type, &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to compare doc_type's"))
	{
		printf("failed to compare document types\n");
		goto quit;
	}

	if (rv != 0)
	{
		printf("document types differ\n");
		goto quit;
	}
	else
	{
		printf("document types are good\n");
	}

	/*
	 * Open container
	 */
	if (S_EBMLREAD_METH_VALID(ebmlReader, container))
	{
		uint32 id;

		id = S_EBMLREAD_CALL(ebmlReader, container)(ebmlReader, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to open ebml container"))
		{
			printf("failed to open container\n");
			goto quit;
		}
		else
		{
			printf("container opened with id %d, should be %d\n", id, 129);
		}
	}

	/*
	 * Read int object
	 */
	if (S_EBMLREAD_METH_VALID(ebmlReader, read_object))
	{
		uint32 id;
		sint32 val;


		object = S_EBMLREAD_CALL(ebmlReader, read_object)(ebmlReader, &id, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to read int object"))
		{
			printf("failed to read int object\n");
			goto quit;
		}
		else
		{
			/* get val */
			val = SObjectGetInt(object, &error);
			if (S_CHK_ERR(&error, S_CONTERR,
						  "main",
						  "Failed to get int from object"))
			{
				printf("failed to get int from object\n");
				goto quit;
			}
			else
			{
				printf("read int object [%d] with id %d, should be %d and id %d\n", val, id, 21, 129);
			}
		}

		/* delete object */
		S_DELETE(object, "main", &error);
	}


	/*
	 * Read float object
	 */
	if (S_EBMLREAD_METH_VALID(ebmlReader, read_object))
	{
		uint32 id;
		float val;


		object = S_EBMLREAD_CALL(ebmlReader, read_object)(ebmlReader, &id, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to read float object"))
		{
			printf("failed to read float object\n");
			goto quit;
		}
		else
		{
			/* get val */
			val = SObjectGetFloat(object, &error);
			if (S_CHK_ERR(&error, S_CONTERR,
						  "main",
						  "Failed to get float from object"))
			{
				printf("failed to get float from object\n");
				goto quit;
			}
			else
			{
				printf("read float object [%f] with id %d, should be %f and id %d\n", val, id, -3.187, 130);
			}
		}

		/* delete object */
		S_DELETE(object, "main", &error);
	}


	/*
	 * Read string object
	 */
	if (S_EBMLREAD_METH_VALID(ebmlReader, read_object))
	{
		uint32 id;
		const char *val;


		object = S_EBMLREAD_CALL(ebmlReader, read_object)(ebmlReader, &id, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to read string object"))
		{
			printf("failed to read string object\n");
			goto quit;
		}
		else
		{
			/* get val */
			val = SObjectGetString(object, &error);
			if (S_CHK_ERR(&error, S_CONTERR,
						  "main",
						  "Failed to get string from object"))
			{
				printf("failed to get string from object\n");
				goto quit;
			}
			else
			{
				printf("read string object [%s] with id %d, should be %s and id %d\n", val, id, "hello world", 131);
			}
		}

		/* delete object */
		S_DELETE(object, "main", &error);
	}

	/*
	 * Check that container is at end
	 */
	if (S_EBMLREAD_METH_VALID(ebmlReader, container_at_end))
	{
		s_bool at_end;

		at_end = S_EBMLREAD_CALL(ebmlReader, container_at_end)(ebmlReader, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to check if container is at end"))
		{
			printf("failed to check if container is at end\n");
			goto quit;
		}

		if (at_end)
		{
			printf("found container at end (correct)\n");
		}
		else
		{
			printf("container not at end, error\n");
			goto quit;
		}
	}


	/*
	 * quit
	 */
quit:
	if (ds != NULL)
		S_DELETE(ds, "main", &error);

	if (ebmlReader != NULL)
		S_DELETE(ebmlReader, "main", &error);

	/* must be after ebmlReader */
	if (plugin != NULL)
		S_DELETE(plugin, "main", &error);

	if (object != NULL)
		S_DELETE(object, "main", &error);


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
