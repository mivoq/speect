.. index:: Generic Object System (C API)

.. _objsystem_main_c:

=====================
Generic Object System
=====================

Provides an object-oriented approach for data types. See the :ref:`object_system_topic` topic
guide for more details. 


Definitions
===========

.. index:: 
   single: Generic Object System (C API); SObject

SObject provides an interface to a generic data container.
It is a simple object which can encapsulate any other objects and
allows entities to store *any* type of data. Any data type
can inherit from the SObjectClass, and implement the basic
methods, or not. An object can also extend these as required.

.. doxybridge:: SObject
   :type: speect object
   :members: none

.. toctree::
   :hidden:

   SObject_structure

:doc:`SObject_structure`

.. index:: 
   single: Generic Object System (C API); SObjectClass

.. doxybridge:: SObjectClass
   :type: speect class
   :members: none

.. toctree::
   :hidden:

   SObjectClass_structure

:doc:`SObjectClass_structure`


Interface
=========

.. toctree::
   :maxdepth: 2

   functions
   class_functions
   primitives
   macros






