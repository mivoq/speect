
.. index:: Generic Object System (C API), SObject

=======
SObject
=======

.. toctree::
   :hidden:

   SObject_detail
   

Definition
==========

.. doxybridge:: SObject
   :type: speect object
   :members: none


.. seealso:: :doc:`SObject_detail`.


Functions
=========

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
