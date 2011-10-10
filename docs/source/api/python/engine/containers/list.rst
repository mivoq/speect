.. index::
   single: Containers (Python API); SList

=====
SList
=====

.. currentmodule:: speect

SList containers are the Speect Engine equivalent of the Python
``list`` type.  The Python SList class is just an interface to the
Speect Engine C :c:type:`SList` type.  Python SList objects can not be
instantiated, however, it can be a return value of certain functions
or class attributes (for example :attr:`SVoice.dataObjects`).

.. py:class:: SList

   A list class, an abstract data structure that implements an ordered collection
   of value, where the same value may occur more than once.

   .. note::

      The ``SList`` object inherits from ``SObject`` in the Speect Engine
      and can therefore be used in functions/methods that require parameters
      of type ``SObject``.


.. _slist_python_operations:

Operations
==========

SList supports the following Python ``list`` operations:

**len(l)**

	Return the number of items in the list l.


.. _slist_python_get_item:

**l[index]**

	Return the item in the list that is associated with the given index, where *index*
	is the index of the item in the list. Indexing starts from 0.
	Raises a ``IndexError`` if index is out of bounds.


**l[index] = value**

	Set ``l[index]`` to *value*, where *index* is an index to the list. Indexing starts from 0.
	Raises a ``IndexError`` if index is out of bounds.


**del l[index]**

        Remove ``l[index]`` from *l*. Raises a ``IndexError`` if index is out of bounds.
	Indexing starts from 0.


**for item in l**

        SList also supports the Python iterator protocol and therefore one can iterate over
	a list's items.


**str(l)**
	
	SList supports the Python method ``__str__()`` and can therefore be
	used in the ``str()`` built-in function and by the ``print`` statement
	to compute the "informal" string representation of the list.


Functions
=========

.. automethod:: speect.SList.value_get()
