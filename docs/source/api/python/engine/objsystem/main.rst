
=======
SObject
=======

.. currentmodule:: speect

.. py:class:: SObject

   SObject is a proxy of the C :c:type:`SObject` structure, and is not
   directly used. It can represent any C Speect type object that has
   inherited from :c:type:`SObject` (see
   :ref:`object_system_topic`). It can therefore be
   used by plug-ins to return objects, for which no Python wrapper
   classes exists, which can then be used by other functions or
   plug-ins. For example :meth:`SList.value_get` returns an
   :class:`SObject` if no wrapper exists, although this object can be
   used by other functions.
