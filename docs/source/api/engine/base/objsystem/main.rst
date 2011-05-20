.. index:: ! Generic Object System (C API)

.. _objsystem/main:

.. todo:: build time macros and definitions

=====================
Generic Object System
=====================

.. toctree::
   :hidden:

   SObject_detail
   SObjectClass_detail


SObject Definition
==================

.. doxybridge:: SObject
   :type: speect object
   :members: none


.. seealso:: :doc:`SObject_detail`.


SObject Functions
=================

Query
-----

.. doxybridge:: SObjectSize

.. doxybridge:: SObjectIsType

.. doxybridge:: SObjectType

.. doxybridge:: SObjectInheritance


Read/Write
----------

.. doxybridge:: SObjectRead


.. doxybridge:: SObjectWrite


Reference Counting
------------------

.. doxybridge:: SObjectIncRef

.. doxybridge:: SObjectDecRef

.. warning::
   Reference counting functions should be used with utmost care. 
   They are normally not required, and are here just for expert use.



SObjectClass Definition
=======================

.. doxybridge:: SObjectClass
   :type: speect class
   :members: none

.. seealso:: :doc:`SObjectClass_detail`.


SObjectClass Methods
====================

Query
-----

.. doxybridge:: SObjectCompare


Miscellaneous
-------------

.. doxybridge:: SObjectPrint


Copy
----

.. doxybridge:: SObjectCopy


