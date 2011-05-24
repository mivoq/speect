
.. index:: 
   single: Generic Object System (C API); Object Functions

================
Object Functions
================


Query
=====

.. doxybridge:: SObjectSize

.. doxybridge:: SObjectIsType

.. doxybridge:: SObjectType

.. doxybridge:: SObjectInheritance

.. doxybridge:: SObjectCompare


Read/Write
==========

.. doxybridge:: SObjectRead

.. doxybridge:: SObjectWrite

.. doxybridge:: SObjectTypeIsReadable 

.. doxybridge:: SObjectIsWritable


Save/Load
=========

.. doxybridge:: SObjectSave

.. doxybridge:: SObjectLoad



Reference Counting
==================

.. doxybridge:: SObjectIncRef

.. doxybridge:: SObjectDecRef

.. warning::
   Reference counting functions should be used with utmost care. 
   They are normally not required, and are here just for expert use.


Miscellaneous
=============

.. doxybridge:: SObjectPrint


Copy
====

.. doxybridge:: SObjectCopy
