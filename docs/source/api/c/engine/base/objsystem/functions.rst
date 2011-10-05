
.. index:: 
   single: Generic Object System (C API); SObject Functions

=================
SObject Functions
=================

Provides object type function.


Summary
=======

.. doxybridge-autosummary::
   :nosignatures:
   
   SObjectSize
   SObjectIsType
   SObjectType
   SObjectInheritance
   SObjectCompare
   SObjectRead
   SObjectWrite
   SObjectTypeIsReadable
   SObjectIsWritable
   SObjectSave
   SObjectLoad
   SObjectIncRef
   SObjectDecRef
   SObjectPrint
   SObjectCopy


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
