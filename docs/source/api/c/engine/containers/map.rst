
====
SMap
====

SMap containers are data types composed of a paired collection of
unique keys (strings) and values (:c:type:`SObject`), where
each key is associated with one value. There are currently
two different :ref:`implementations <implementations_map>` of the SMap
container.

The iterator (:c:type:`SIterator`) implementation returns an element
(as :c:type:`SObject`) of the map for the :c:func:`SIteratorObject`
function call. When a key-value pair is unlinked from the map with
:c:func:`SIteratorUnlink`, the key is freed, and the value is returned
(as :c:type:`SObject`). The SMap iterator implements the
:c:func:`SIteratorKey` call and returns the key of a key-value pair.


Definitions
===========

.. index::
   single: Containers (C API); SMap

.. doxybridge:: SMap
   :type: speect object
   :members: none
   :inheritance: SObject.SContainer.SMap

.. toctree::
   :hidden:

   SMap_structure

:doc:`SMap_structure`


.. index:: 
   single: Containers (C API); SMapClass


.. doxybridge:: SMapClass
   :type: speect object
   :members: none
   :inheritance: SObjectClass.SContainerClass.SMapClass

.. toctree::
   :hidden:

   SMapClass_structure

:doc:`SMapClass_structure`.


Functions
=========

Accessing
~~~~~~~~~

.. doxybridge:: SMapGetInt

.. doxybridge:: SMapGetFloat

.. doxybridge:: SMapGetString

.. doxybridge:: SMapGetObject

.. doxybridge:: SMapGetKeys


Accessing (with defaults)
~~~~~~~~~~~~~~~~~~~~~~~~~

.. doxybridge:: SMapGetIntDef

.. doxybridge:: SMapGetFloatDef

.. doxybridge:: SMapGetStringDef

.. doxybridge:: SMapGetObjectDef


Setting
~~~~~~~

.. doxybridge:: SMapSetInt

.. doxybridge:: SMapSetFloat

.. doxybridge:: SMapSetString

.. doxybridge:: SMapSetObject


Delete/Unlink
~~~~~~~~~~~~~

.. doxybridge:: SMapObjectDelete

.. doxybridge:: SMapObjectUnlink


Query
~~~~~

.. doxybridge:: SMapObjectPresent

.. doxybridge:: SMapSize


Copy
~~~~

.. doxybridge:: SMapCopy


.. _implementations_map:

Implementations
===============

Two different implementations of the SMap container:

.. index::
   single: Containers (C API); SMapHashTable

SMapHashTable
~~~~~~~~~~~~~


SMapHashTable is an implementation of SMap, where the underlying data structure
is a hash table.

.. doxybridge:: SMapHashTable
   :type: speect object
   :members: none
   :inheritance: SObject.SContainer.SMap.SMapHashTable


SMapHashTable Specific Functions
--------------------------------

.. doxybridge:: SMapHashTableResize


.. index::
   single: Containers (C API); SMapList

SMapList
~~~~~~~~

SMapList is an implementation of SMap, where the underlying data structure
is a doubly-linked list.

.. doxybridge:: SMapList
   :type: speect object
   :members: none
   :inheritance: SObject.SContainer.SMap.SMapList
