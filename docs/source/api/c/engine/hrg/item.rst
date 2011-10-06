.. _hrg/item:

=====
SItem
=====

.. todo:: feature processor link in paths, examples, explain ph phone constant features.

A class for containing linguistic items (arbitrary objects). Items are
nodes in a relation which is again linked to an utterance. Items
contain a key-value (string/:c:type:`SObject`) set of features. An item
can have a list of daughter items as well. Items can also be shared
between relations.


Definitions
===========

.. index:: 
   single: Heterogeneous Relation Graphs (C API); SItem

.. doxybridge:: SItem
   :type: speect object
   :members: none
   :inheritance: SObject.SItem

.. toctree::
   :hidden:

   SItem_structure

:doc:`SItem_structure`

.. index:: 
   single: Heterogeneous Relation Graphs (C API); SItemClass

.. doxybridge:: SItemClass
   :type: speect class
   :members: none
   :inheritance: SObjectClass.SItemClass

.. toctree::
   :hidden:

   SItemClass_structure

:doc:`SItemClass_structure`


Macros
======

.. doxybridge:: S_ITEM
   :type: macro


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



Path Features
-------------

Extract items, item features and item functions by navigating given
paths on the HRG structure (see :ref:`HRG Path topic
<hrg_paths>`). Paths are strings that are composed of period (".")
separated tokens describing the path of the requested item/feature,
relative to a given item. The possible tokens are:

=========   ========
Token       Meaning
=========   ========
p 	    previous item
n 	    next item
daughter    first daughter item
daughtern   last daughter item
parent 	    parent item
R:relname   item *as* it is in the given relation ``relname``
=========   ========


.. doxybridge:: s_path_to_item

.. doxybridge:: s_path_to_feature

.. doxybridge:: s_path_to_featproc

.. doxybridge:: SItemPath


Miscellaneous
-------------

.. doxybridge:: SItemRelation

.. doxybridge:: SItemUtterance 

.. doxybridge:: SItemVoice

.. doxybridge:: SItemEqual


