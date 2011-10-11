.. index::
   single: Containers (Python API); SMap

====
SMap
====

.. currentmodule:: speect

SMap containers are the Speect Engine equivalent of the Python
``dict`` type.  The Python SMap class is just an interface to the
Speect Engine C :c:type:`SMap` type.  Python SMap objects can not be
instantiated, however, it can be a return value of certain functions
or class attributes (for example :attr:`SVoice.features`).

.. py:class:: SMap

   A map class, an abstract data type composed of a collection of
   unique keys (strings) and a collection of values, where
   each key is associated with one value.

   .. note::

      The ``SMap`` object inherits from ``SObject`` in the Speect Engine
      and can therefore be used in functions/methods that require parameters
      of type ``SObject``.


Operations
==========

SMap supports the following Python ``dict`` operations:

**len(m)**

	Return the number of items in the map m.


**m[key]**

	Return the item of m with key *key*. Raises a ``KeyError`` if key is not in the map.


**m[key] = value**

	Set ``m[key]`` to *value*.


**del m[key]**

        Remove ``m[key]`` from *m*. Raises a ``KeyError`` if *key* is not in the map.


**key in m**

        Return ``True`` if *m* has a key *key*, else ``False``.


**key not in m**

        Equivalent to ``not key in m``.


**for key in m**

        SMap also supports the Python iterator protocol and therefore one can iterate over
	a map's keys.


**str(m)**
	
	SMap supports the Python method ``__str__()`` and can therefore be
	used in the ``str()`` built-in function and by the ``print`` statement
	to compute the "informal" string representation of the map.


Functions
=========

.. automethod:: speect.SMap.value_get()
