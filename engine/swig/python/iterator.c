
%inline
{
	typedef struct
	{
		SIterator *itr;
	} PIterator;


	PIterator *make_PIterator(SIterator *itr, s_erc *error)
	{
		PIterator *pitr;


		S_CLR_ERR(error);
		pitr = S_MALLOC(PIterator, 1);
		if (pitr == NULL)
		{
			S_FTL_ERR(error, S_MEMERROR,
					  "make_PIterator",
					  "Failed to allocate memory for 'PIterator' object");
			return NULL;
		}

		pitr->itr = itr;
		return pitr;
	}
}

%nodefaultctor PIterator;


%extend PIterator
{
	PIterator *__iter__()
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

