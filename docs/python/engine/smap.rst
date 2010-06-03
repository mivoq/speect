.. _python_engine_smap:

====
SMap
====

This section describes the :class:`SMap` class, which is an interface to
the Speect Engine map container (similar to a Python dictionary).

|


.. currentmodule:: speect

.. autoclass:: SMap

.. autosummary::

   SMap.__setitem__
   SMap.__getitem__
   SMap.__delitem__
   SMap.__len__
   SMap.__str__
   SMap.__iter__
   SMap.value_get


.. note::

    The ``SMap`` object inherits from ``SObject`` in the Speect Engine
    and can therefore be used in functions/methods that require parameters
    of type ``SObject``, e.g. :meth:`SVoice.data_set`.


.. automethod:: SMap.__setitem__()
   
An ``SMap`` has the same functionality as a Python dictionary. To add a key-value pair
to an ``SMap``::

    mySMap["library name"] = "test library"


.. automethod:: SMap.__getitem__()

To get a key-value pair from an ``SMap``::

    libname = mySMap["library name"]

.. note::
   
    The Speect SWIG/Python implementation tries to convert the returned Speect object
    to a Python object through the use of ``typemaps``. These ``typemaps`` can also be
    extended for new object types through plug-ins. Sometimes we want access to the
    pure Speect object, for example in methods which require an ``SObject`` type
    such as :meth:`SVoice.data_set`, to get the ``SObject`` value we can call 
    :meth:`SMap.value_get`.


.. automethod:: SMap.__delitem__()

Delete a key-value pair from an ``SMap``::

    del mySMap["library name"]


.. automethod:: SMap.__len__()

Get the number of key-value pairs::
   
    len(mySMap)

.. automethod:: SMap.__str__()

Print a string version of the key-value pairs of an ``SMap``::
    
    print mySMap

.. automethod:: SMap.__iter__()

One can iterate over the keys of an ``SMap``::
    
    for keys in mySMap:
    	print keys


.. automethod:: SMap.value_get()

Get the raw Speect ``SObject`` value for a given key::

    sobject = mySMap.value_get("library name")

.. seealso::
   :meth:`SMap.__getitem__`
