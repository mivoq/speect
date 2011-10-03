
=====
SList
=====

.. todo:: add examples
   

SList containers are data types that holds an ordered collection of
values (:c:type:`SObject`) , where the same value may occur more than
once. There is currently one :ref:`implementation
<implementations_list>` of the SList container.

The iterator (:c:type:`SIterator`) implementation returns the elements
(as :c:type:`SObject`) of the list for the :c:func:`SIteratorObject`,
and :c:func:`SIteratorUnlink` function calls. The
:c:func:`SIteratorKey` method is not implemented and will set an error
and return :c:type:`NULL` if called for SList type iterators.


Definitions
===========

.. index::
   single: Containers (C API); SList

.. doxybridge:: SList
   :type: speect object
   :members: none
   :inheritance: SObject.SContainer.SList

.. toctree::
   :hidden:

   SList_structure

:doc:`SList_structure`


.. index:: 
   single: Containers (C API); SListClass


.. doxybridge:: SListClass
   :type: speect object
   :members: none
   :inheritance: SObjectClass.SContainerClass.SListClass

.. toctree::
   :hidden:

   SListClass_structure

:doc:`SListClass_structure`.


Functions
=========

Query
~~~~~

.. doxybridge:: SListIsEmpty

.. doxybridge:: SListSize

.. doxybridge:: SListValPresent


Insertion
~~~~~~~~~

.. doxybridge:: SListAppend

.. doxybridge:: SListPrepend

.. doxybridge:: SListInsertBefore

.. doxybridge:: SListInsertAfter


Merge/Copy
~~~~~~~~~~

.. doxybridge:: SListMerge

.. doxybridge:: SListCopy


Queue
~~~~~

.. doxybridge:: SListPush

.. doxybridge:: SListPop

.. doxybridge:: SListReverse


Accessing
~~~~~~~~~

.. doxybridge:: SListNth


.. _implementations_list:

Implementations
===============

.. index::
   single: Containers (C API); SListList

SListList
~~~~~~~~~

SListList is an implementation of SList, where the underlying data structure
is a doubly-linked list.

.. doxybridge:: SListList
   :type: speect object
   :members: none
   :inheritance: SObject.SContainer.SList.SListList
