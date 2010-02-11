/* SRelation iterator */

%inline{

	typedef struct
	{
		const SItem *item;
	} SRelationItr;

	SRelationItr *make_SRelationItr(const SRelation *rel, s_erc *error)
	{
		SRelationItr *ritr;


		S_CLR_ERR(error);
		ritr = S_MALLOC(SRelationItr, 1);
		if (ritr == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "make_SRelationItr",
					  "Failed to allocate memory for 'SRelationItr' object");
			return NULL;
		}

		ritr->item = SRelationHead(rel, error);
		if (*error != S_SUCCESS)
		{
			S_FREE(ritr);
			return NULL;
		}

		return ritr;
	}
 }


%nodefaultctor SRelationItr;

%extend SRelationItr
{
	SRelationItr *__iter__()
	{
		return $self;
	}

	PyObject *next()
	{
		s_erc error;
		PyObject *pobject;


		S_CLR_ERR(&error);
		if ($self->item == NULL)
		{
			PyErr_SetNone(PyExc_StopIteration);
			return NULL;
		}

		pobject = SWIG_NewPointerObj(SWIG_as_voidptr($self->item), SWIGTYPE_p_SItem, 0 |  0 );

		$self->item = SItemNext($self->item, &error);
		if (error != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Call to \"SItemNext\" failed");
			return NULL;
		}

		return pobject;
	}
}




