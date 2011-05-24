
====
Item
====


.. toctree::
   :hidden:

   SItem_detail


Definitions
===========

.. index:: 
   single: Heterogeneous Relation Graphs (C API); SItem

.. doxybridge:: SItem
   :type: speect object
   :members: none
   :inheritance: SObject.SItem

.. index:: 
   single: Heterogeneous Relation Graphs (C API); SItemClass

.. doxybridge:: SItemClass
   :type: speect class
   :members: none
   :inheritance: SObjectClass.SItemClass

.. seealso:: :doc:`SItem_detail`.


Functions
=========

Initialization
--------------

.. doxybridge:: SItemInit


As/In Relation
--------------

.. doxybridge:: SItemAs

.. doxybridge:: SItemIn


List Traversal
--------------

.. doxybridge:: SItemNext

.. doxybridge:: SItemPrev

.. doxybridge:: SItemAppend

.. doxybridge:: SItemPrepend


Tree Traversal
--------------

.. doxybridge:: SItemParent

.. doxybridge:: SItemDaughter

.. doxybridge:: SItemNthDaughter

.. doxybridge:: SItemLastDaughter

.. doxybridge:: SItemAddDaughter


Feature Functions
-----------------

.. doxybridge:: SItemGetName

.. doxybridge:: SItemSetName

.. doxybridge:: SItemFeatureIsPresent

.. doxybridge:: SItemDelFeature 


.. doxybridge:: SItemFeatKeys


Get key-value feature pairs
---------------------------

.. doxybridge:: SItemGetObject

.. doxybridge:: SItemGetInt

.. doxybridge:: SItemGetFloat

.. doxybridge:: SItemGetString



Set key-value feature pairs
---------------------------

.. doxybridge:: SItemSetObject

.. doxybridge:: SItemSetInt

.. doxybridge:: SItemSetFloat

.. doxybridge:: SItemSetString


Paths
-----

.. doxybridge:: s_path_to_item

.. doxybridge:: s_path_to_feature

.. doxybridge:: s_path_to_featproc



Miscellaneous
-------------

.. doxybridge:: SItemRelation

.. doxybridge:: SItemUtterance 

.. doxybridge:: SItemVoice

.. doxybridge:: SItemEqual


Macros
======

.. doxybridge:: S_ITEM
   :type: macro
