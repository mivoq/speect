.. index:: ! Generic Object System (C API)


.. _objsystem/main:

.. todo:: build time macros and definitions

=====================
Generic Object System
=====================

The object system provides basic object-oriented programming support (e.g., inheritance,
polymorphism, abstraction, etc.) to the Speect Engine by using **ISO C** structures 
as *objects* and *classes*.
 

Objects
=======

.. speect:object:: SObject

   The base object structure, it provides an interface to a generic data type. 
   It can encapsulate any other object, thereby providing abstraction.
   All objects that want to work with the Speect Generic Object System must inherit
   from this object.


Object Methods
==============

.. doxybridge:: SObjectSize

.. doxybridge:: SObjectIsType

.. doxybridge:: SObjectType

.. doxybridge:: SObjectInheritance

.. doxybridge:: SObjectCompare

.. doxybridge:: SObjectPrint

.. doxybridge:: SObjectCopy

.. doxybridge:: SObjectRead

.. doxybridge:: SObjectWrite

.. doxybridge:: SObjectIncRef

.. doxybridge:: SObjectDecRef

.. doxybridge:: S_NEW
   :type: macro

.. doxybridge:: S_DELETE
   :type: macro

.. doxybridge:: S_FORCE_DELETE
   :type: macro

.. doxybridge:: S_CAST_SAFE
   :type: macro

.. doxybridge:: S_CAST_UNSAFE
   :type: macro

.. doxybridge:: S_CAST
   :type: macro

.. doxybridge:: S_OBJECT_CALL
   :type: macro

.. doxybridge:: S_OBJECT_METH_VALID
   :type: macro

.. doxybridge:: S_OBJECT
   :type: macro

.. doxybridge:: S_OBJECT_REF
   :type: macro

.. doxybridge:: S_OBJECT_CLS
   :type: macro



Classes
=======
	
.. speect:class:: SObjectClass

   The base class structure. The class defines a constructor and destructor for :c:type:`SObject`,
   as well as class members, i.e. members that belong to all object instances of a specific class
   type. All classes that want to work with the Speect Generic Object System must inherit from this
   class structure.


Class Methods
=============

.. doxybridge:: s_class_add

.. doxybridge:: s_class_init

.. doxybridge:: s_class_reg

.. doxybridge:: s_class_free

.. doxybridge:: s_class_name

.. doxybridge:: s_class_inheritance

.. doxybridge:: s_class_is_reg

.. doxybridge:: s_class_find


