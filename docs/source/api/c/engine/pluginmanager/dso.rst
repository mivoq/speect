.. index:: 
   single: Plug-ins (C API); Dynamic Shared Objects

======================
Dynamic Shared Objects
======================

Provides support for the use of *dynamic shared objects* (dso).  With
:c:type:`SDso` one can open/close a dso and get symbols from a
dso. SDso will close the associated dso when it is deleted with
:c:type:`S_DELETE`. Implementation specific dynamic linking is
abstracted.


Summary
=======

.. doxybridge-autosummary::
   :nosignatures:
   
   SDsoLoad
   SDsoGetSymbol


Definition
==========

.. index:: 
   single: Plug-ins (C API); SDso

.. doxybridge:: SDso
   :type: speect object
   :members: none
   :inheritance: SObject.SDso

.. toctree::
   :hidden:

   SDso_structure

:doc:`SDso_structure`


.. index:: 
   single: Plug-ins (C API); SDsoClass


.. doxybridge:: SDsoClass
   :type: speect class
   :members: none
   :inheritance: SObjectClass.SDsoClass
 

.. toctree::
   :hidden:

   SDsoClass_structure

:doc:`SDsoClass_structure`



Functions
=========

.. doxybridge:: SDsoLoad

.. doxybridge:: SDsoGetSymbol
