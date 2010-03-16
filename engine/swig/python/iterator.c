
%inline
{
	typedef struct
	{
		SIterator *itr;
	} PMapIterator;


	PMapIterator *make_PMapIterator(SIterator *itr, s_erc *error)
	{
		PMapIterator *pitr;


		S_CLR_ERR(error);
		pitr = S_MALLOC(PMapIterator, 1);
		if (pitr == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "make_PMapIterator",
					  "Failed to allocate memory for 'PMapIterator' object");
			return NULL;
		}

		pitr->itr = itr;
		return pitr;
	}

	typedef struct
	{
		SIterator *itr;
	} PListIterator;


	PListIterator *make_PListIterator(SIterator *itr, s_erc *error)
	{
		PListIterator *pitr;


		S_CLR_ERR(error);
		pitr = S_MALLOC(PListIterator, 1);
		if (pitr == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "make_PListIterator",
					  "Failed to allocate memory for 'PListIterator' object");
			return NULL;
		}

		pitr->itr = itr;
		return pitr;
	}
}

%nodefaultctor PListIterator;
%nodefaultctor PMapIterator;


%extend PMapIterator
{
	PMapIterator *__iter__()
	{
		return $self;
	}

	PyObject *next()
	{
		const char *key;
		size_t slen;
		s_erc error;
		PyObject *pobject;


		S_CLR_ERR(&error);
		if ($self->itr == NULL)
		{
			PyErr_SetNone(PyExc_StopIteration);
			return NULL;
		}

		key = SMapIteratorKey($self->itr, &error);
		if (error != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Call to \"SMapIteratorKey\" failed");
			return NULL;
		}

		slen = s_strsize(key, &error);
		if (error != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Call to \"s_strsize\" failed");
			return NULL;
		}

		$self->itr = SIteratorNext($self->itr);
		pobject = PyString_FromStringAndSize(key, slen);
		return pobject;
	}
};


%extend PListIterator
{
	PListIterator *__iter__()
	{
		return $self;
	}

	PyObject *next()
	{
		const SObject *iObject;
		s_erc error;
		PyObject *pobject;


		S_CLR_ERR(&error);
		if ($self->itr == NULL)
		{
			PyErr_SetNone(PyExc_StopIteration);
			return NULL;
		}

		iObject = SListIteratorValue($self->itr, &error);
		if (error != S_SUCCESS)
		{
			PyErr_SetString(PyExc_RuntimeError, "Call to \"SListIteratorValue\" failed");
			return NULL;
		}

		$self->itr = SIteratorNext($self->itr);
		pobject = sobject_2_pyobject(iObject, &error, FALSE);
		if (error != S_SUCCESS)
			return NULL;

		return pobject;
	}
};

