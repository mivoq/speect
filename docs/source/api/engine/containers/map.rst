
.. index::
   single: Containers (C API); SMap

====
SMap
====

.. todo:: check if normal accessing will fail if key not in map

.. toctree::
   :hidden:

   SMap_detail


Definitions
===========

.. doxybridge:: SMap
   :type: speect object
   :members: none
   :inheritance: SObject.SContainer.SMap

.. doxybridge:: SMapClass
   :type: speect object
   :members: none
   :inheritance: SObjectClass.SContainerClass.SMapClass

.. seealso:: :doc:`SMap_detail`.


Accessing
=========

.. doxybridge:: SMapGetInt

.. doxybridge:: SMapGetFloat

.. doxybridge:: SMapGetString

.. doxybridge:: SMapGetObject

.. doxybridge:: SMapGetKeys


Accessing (with defaults)
=========================

.. doxybridge:: SMapGetIntDef

.. doxybridge:: SMapGetFloatDef

.. doxybridge:: SMapGetStringDef

.. doxybridge:: SMapGetObjectDef


Setting
=======

.. doxybridge:: SMapSetInt

.. doxybridge:: SMapSetFloat

.. doxybridge:: SMapSetString

.. doxybridge:: SMapSetObject


Delete/Unlink
=============

.. doxybridge:: SMapObjectDelete

.. doxybridge:: SMapObjectUnlink


Query
=====

.. doxybridge:: SMapObjectPresent

.. doxybridge:: SMapSize


Copy
====

.. doxybridge:: SMapCopy



