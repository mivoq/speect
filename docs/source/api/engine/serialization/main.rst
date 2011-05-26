.. _serialization/main:

.. index:: ! Serialization (C API)

=============
Serialization
=============

Reading/Writing and loading/saving of serialized objects.

.. toctree::
   :hidden:
	
   SSerializedFile_detail
   SSerializedObject_detail

	
Summary
=======

.. doxybridge-autosummary::
   :nosignatures:
   
   SSerializedObjectRegister
   SSerializedObjectFree
   SSerializedObjectGet
   SSerializedFileRegister
   SSerializedFileFree
   SSerializedObjectRead
   SSerializedObjectWrite


Definitions
===========

.. doxybridge:: SSerializedFile
   :type: speect object
   :members: none
   :inheritance: SObject.SSerializedFile

.. doxybridge:: SSerializedFileClass
   :type: speect class
   :members: none
   :inheritance: SObjectClass.SSerializedFileClass
 

.. seealso:: :doc:`SSerializedFile_detail`.


.. doxybridge:: SSerializedObject
   :type: speect object
   :members: none
   :inheritance: SObject.SSerializedObject

.. doxybridge:: SSerializedObjectClass
   :type: speect class
   :members: none
   :inheritance: SObjectClass.SSerializedObjectClass
 
.. seealso:: :doc:`SSerializedObject_detail`.


Functions
=========

.. doxybridge:: SSerializedObjectRegister

.. doxybridge:: SSerializedObjectFree

.. doxybridge:: SSerializedObjectGet

.. doxybridge:: SSerializedFileRegister

.. doxybridge:: SSerializedFileFree

.. doxybridge:: SSerializedObjectRead

.. doxybridge:: SSerializedObjectWrite

