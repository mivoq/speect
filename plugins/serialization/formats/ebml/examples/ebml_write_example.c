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
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/

#include <stdio.h>
#include "speect.h"
#include "ebml_write.h"


int main()
{
	s_erc error;
	SDatasource *ds = NULL;
	SPlugin *plugin = NULL;
	s_ebml_header *header = NULL;
	const char *doc_type = "spct_ebml_test";
	SEbmlWrite *ebmlWriter = NULL;


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
	 * load the ebml plug-in
	 */
	plugin = s_pm_load_plugin("ebml.spi", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to load plug-in at 'ebml.spi'"))
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
	ds = SFilesourceOpenFile("ebml_test.bin", "wb", &error);
	if (S_CHK_ERR(&error, S_CONTERR,
				  "main",
				  "Failed to create data source 'ebml_test.bin'"))
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
	 * Write a few unsigned int values
	 */
	if (S_EBMLWRITE_METH_VALID(ebmlWriter, write_uint))
	{
		S_EBMLWRITE_CALL(ebmlWriter, write_uint)(ebmlWriter, 130, 234, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to write uint [%d]", 234))
		{
			printf("failed to write uint [%d]\n", 234);
			goto quit;
		}
		else
		{
			printf("wrote uint [%d], id = %d\n", 234, 130);
		}

		S_EBMLWRITE_CALL(ebmlWriter, write_uint)(ebmlWriter, 130, 31234, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to write uint [%d]", 31234))
		{
			printf("Failed to write uint [%d]\n", 31234);
			goto quit;
		}
		else
		{
			printf("wrote uint [%d], id = %d\n", 31234, 130);
		}
	}

	/*
	 * Write a few signed int values
	 */
	if (S_EBMLWRITE_METH_VALID(ebmlWriter, write_sint))
	{
		S_EBMLWRITE_CALL(ebmlWriter, write_sint)(ebmlWriter, 131, -69, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to write sint [%d]", -69))
		{
			printf("Failed to write sint [%d]\n", -69);
			goto quit;
		}
		else
		{
			printf("wrote sint [%d], id = %d\n", -69, 131);
		}

		S_EBMLWRITE_CALL(ebmlWriter, write_sint)(ebmlWriter, 131, -132769, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to write sint [%d]", -132769))
		{
			printf("Failed to write sint [%d]\n", -132769);
			goto quit;
		}
		else
		{
			printf("wrote sint [%d], id = %d\n", -132769, 131);
		}
	}

	/*
	 * Start a container
	 */
	if (S_EBMLWRITE_METH_VALID(ebmlWriter, start_container))
	{
		S_EBMLWRITE_CALL(ebmlWriter, start_container)(ebmlWriter, 132, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to start ebml container"))
		{
			printf("failed to start container\n");
			goto quit;
		}
		else
		{
			printf("container started, id = %d\n", 132);
		}
	}

	/*
	 * Write some floats
	 */
	if (S_EBMLWRITE_METH_VALID(ebmlWriter, write_float))
	{
		S_EBMLWRITE_CALL(ebmlWriter, write_float)(ebmlWriter, 133, 20.343, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to write float [%f]", 20.343))
		{
			printf("Failed to write float [%f]\n", 20.343);
			goto quit;
		}
		else
		{
			printf("wrote float [%f], id = %d\n", 20.343, 133);
		}

		S_EBMLWRITE_CALL(ebmlWriter, write_float)(ebmlWriter, 133, -12320.7603, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to write float [%f]", -12320.7603))
		{
			printf("Failed to write float [%f]\n", -12320.7603);
			goto quit;
		}
		else
		{
			printf("wrote float [%f], id = %d\n", -12320.7603, 133);
		}
	}

	/*
	 * Write some doubles
	 */
	if (S_EBMLWRITE_METH_VALID(ebmlWriter, write_double))
	{
		S_EBMLWRITE_CALL(ebmlWriter, write_double)(ebmlWriter, 134, -3.145343234223321, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to write double [%f]", -3.145343234223321))
		{
			printf("Failed to write double [%f]\n", -3.145343234223321);
			goto quit;
		}
		else
		{
			printf("wrote double [%f], id = %d\n", -3.145343234223321, 134);
		}

		S_EBMLWRITE_CALL(ebmlWriter, write_double)(ebmlWriter, 134, 533.145343234223321145343234223321, &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to write double [%f]", 533.145343234223321145343234223321))
		{
			printf("Failed to write double [%f]\n", 533.145343234223321145343234223321);
			goto quit;
		}
		else
		{
			printf("wrote double [%f], id = %d\n", 533.145343234223321145343234223321, 134);
		}
	}

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


	/*
	 * Write some ascii
	 */
	if (S_EBMLWRITE_METH_VALID(ebmlWriter, write_ascii))
	{
		S_EBMLWRITE_CALL(ebmlWriter, write_ascii)(ebmlWriter, 135, "hello world", &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to write ascii [%s]", "hello world"))
		{
			printf("Failed to write ascii [%s]\n", "hello world");
			goto quit;
		}
		else
		{
			printf("wrote ascii [%s], id = %d\n", "hello world", 135);
		}
	}


	/*
	 * Write some utf8
	 */
	if (S_EBMLWRITE_METH_VALID(ebmlWriter, write_utf8))
	{
		S_EBMLWRITE_CALL(ebmlWriter, write_utf8)(ebmlWriter, 136, "hello world in Japanese: ハロー世界", &error);
		if (S_CHK_ERR(&error, S_CONTERR,
					  "main",
					  "Failed to write utf8 [%s]", "hello world in Japanese"))
		{
			printf("Failed to write utf8 [%s]\n", "hello world in Japanese: ハロー世界");
			goto quit;
		}
		else
		{
			printf("wrote utf8 [%s], id = %d\n", "hello world in Japanese: ハロー世界", 136);
		}
	}

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


	/*
	 * quit
	 */
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

	/* must be after ebmlWriter */
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
