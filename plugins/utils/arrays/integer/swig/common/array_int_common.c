/************************************************************************************/
/* Copyright (c) 2010-2011 The Department of Arts and Culture,                      */
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
/* DATE    : February 2010                                                          */
/*                                                                                  */
/************************************************************************************/
/*                                                                                  */
/* SWIG common C convenience functions for SArrayInt.                               */
/*                                                                                  */
/*                                                                                  */
/*                                                                                  */
/************************************************************************************/


/************************************************************************************/
/*                                                                                  */
/* Typedef used in typemaps                                                         */
/*                                                                                  */
/************************************************************************************/


%{
	typedef struct
	{
		int *ia_ip;
		uint32 ia_count;
	} int_array_t;
%}


/************************************************************************************/
/*                                                                                  */
/* Extend the SArrayInt class                                                       */
/*                                                                                  */
/************************************************************************************/
typedef struct
{
	%extend
	{
		const uint32 count;
	}
} SArrayInt;


%types(SArrayInt = SObject, SObject*);

%extend SArrayInt
{
	SArrayInt(const sint32 *array, uint32 len, s_erc *error)
	{
		SArrayInt *tmp;


		tmp = (SArrayInt*)S_NEW("SArrayInt", error);
		if (S_CHK_ERR(error, S_CONTERR,
					  "SArrayInt()",
					  "Failed to create new 'SArrayInt' object"))
			return NULL;

		tmp->count = len;
		tmp->i = (sint32*)array;
		return tmp;
	}

	~SArrayInt()
	{
		s_erc error;


		S_CLR_ERR(&error);
		S_DELETE($self, "~SArrayInt()", &error);
	}

	int_array_t get()
	{
		int_array_t tmp;


		tmp.ia_ip = $self->i;
		tmp.ia_count = $self->count;

		return tmp;
	}
}

%{

	const uint32 SArrayInt_count_get(SArrayInt *array)
	{
		return (const uint32)array->count;
	}
%}

