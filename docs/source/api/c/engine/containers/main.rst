.. index:: ! Containers (C API)

.. _containers_main_c:

==========
Containers
==========

Container data types to store a collection of other objects.


Container Definitions
=====================

.. index::
   single: Containers (C API); SContainer

SContainer is an abstract container object from which all other container
objects must inherit. 

.. doxybridge:: SContainer
   :type: speect object
   :members: none
   :inheritance: SObject.SContainer

.. toctree::
   :hidden:

   SContainer_structure

:doc:`SContainer_structure`

.. index:: 
   single: Containers (C API); SContainerClass

.. doxybridge:: SContainerClass
   :type: speect class
   :members: none
   :inheritance: SObjectClass.SContainerClass

.. toctree::
   :hidden:

   SContainerClass_structure

:doc:`SContainerClass_structure`


.. _containers_iterators_c:

Iterator Definitions
====================

.. index::
   single: Containers (C API); SIterator

SIterator provides an interface for iteration over the objects stored in a container.
Specific container implementations must implement the SIteratorClass functions.

Also see :ref:`examples/c/list_iteration_example`.

.. doxybridge:: SIterator
   :type: speect object
   :members: none
   :inheritance: SObject.SIterator

.. toctree::
   :hidden:

   SIterator_structure

:doc:`SIterator_structure`

.. index:: 
   single: Containers (C API); SIteratorClass

.. doxybridge:: SIteratorClass
   :type: speect class
   :members: none
   :inheritance: SObjectClass.SIteratorClass	

.. toctree::
   :hidden:

   SIteratorClass_structure

:doc:`SIteratorClass_structure`



Macros
======

.. doxybridge:: S_CONTAINER
  :type: macro

.. doxybridge:: S_CONTAINER_CALL
  :type: macro

.. doxybridge:: S_ITERATOR
  :type: macro

.. doxybridge:: S_ITERATOR_GET
  :type: macro

See also :c:macro:`SPCT_DO_SAFE_CAST`


Functions
=========

.. doxybridge:: SContainerGetIterator

.. doxybridge:: SIteratorNext

.. doxybridge:: SIteratorKey

.. doxybridge:: SIteratorObject

.. doxybridge:: SIteratorUnlink


Implementations
===============

.. toctree::
   :maxdepth: 2

   map
   list 
