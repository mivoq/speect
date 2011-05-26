.. index:: 
   single: Plug-ins (C API); Dynamic Shared Objects

======================
Dynamic Shared Objects
======================

Provides support for the use of *dynamic shared objects* (dso).
With :c:type:`SDso` one can open/close a dso and get symbols from a dso. SDso will close
the associated dso when it is deleted with :c:type:`S_DELETE`. Implementation specific dynamic
linking is abstracted.


.. toctree::
   :hidden:

   SDso_detail


Summary
=======

.. doxybridge-autosummary::
   :nosignatures:
   
   SDsoLoad
   SDsoGetSymbol


Definition
==========

.. doxybridge:: SDso
   :type: speect object
   :members: none
   :inheritance: SObject.SDso

.. doxybridge:: SDsoClass
   :type: speect class
   :members: none
   :inheritance: SObjectClass.SDsoClass
 

.. seealso:: :doc:`SDso_detail`.


Functions
=========

.. doxybridge:: SDsoLoad

.. doxybridge:: SDsoGetSymbol
