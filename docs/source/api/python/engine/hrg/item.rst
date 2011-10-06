.. _python/engine/item:

.. index:: 
   single: Heterogeneous Relation Graphs (Python API); SItem

=====
SItem
=====

.. currentmodule:: speect

.. py:class:: SItem

   A class for containing the items (arbitrary objects). Items are
   nodes in a relation which is again linked to an utterance. Items
   contain a key-value (string/object) set of features. An Item can
   have a list of daughter items as well. Items can be shared between
   relations.

   .. note::

      The ``SItem`` object inherits from ``SObject`` in the Speect Engine
      and can therefore be used in functions/methods that require parameters
      of type ``SObject``.


Summary
=======

.. autosummary::

   SItem.append
   SItem.prepend
   SItem.add_daughter
   SItem.delete

   SItem.next
   SItem.prev
   SItem.parent
   SItem.daughter

   SItem.feature_get

   SItem.path
   SItem.path_to_derived_feature
   SItem.path_to_feature
   SItem.path_to_item

   SItem.relation
   SItem.as_relation
   SItem.in_relation
   SItem.utterance
   SItem.voice


Creation/Deletion
=================

Methods relating to the creation of items:

.. automethod:: SItem.append()

.. automethod:: SItem.prepend()

.. automethod:: SItem.add_daughter()

.. seealso::   
   :meth:`SRelation.append`, :meth:`SRelation.prepend`


Deleting an item:

.. automethod:: SItem.delete()


Traversing
==========

Methods relating to the traversing of items:


List Traversal
--------------

.. automethod:: SItem.next()

.. automethod:: SItem.prev()


Tree Traversal
--------------

.. automethod:: SItem.parent()

.. automethod:: SItem.daughter()


Features
========

An item's features are accessible through the standard Python
container functions.  SItem supports the following container
functions:


**len(item)**

	Return the number of features in the item.


**key in item**

        Return ``True`` if *item* has a feature named *key*, else ``False``.


**key not in item**

        Equivalent to ``not key in item``.


**item_a == item_b**

	Comparison operator. Return ``True`` if *item_a* equals *item_b*, else ``False``.
	Equal meaning that the items share the same contents.


**item[key]**

	Return the feature named *key* of item. Note that ``None`` is 
	returned if no feature named *key*.


**item[key] = value**

	Set ``item[key]`` to *value*. If a feature named *key* already exists in item,
	it will be deleted and replaced with *value*.


**del item[key]**

        Remove ``item[key]`` from *item*. Nothing will be done if no feature named *key* is
	in item.


**for key in item**

        SItem also supports the Python iterator protocol and therefore one can iterate over
	an item's feature keys.


**str(item)**
	
	SItem supports the Python method ``__str__()`` and can therefore be
	used in the ``str()`` built-in function and by the ``print`` statement
	to compute the "informal" string representation of the item.


SObject Features
================

.. automethod:: SItem.feature_get()


Paths Features
==============

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

Methods relating to items paths:

.. automethod:: SItem.path()

.. automethod:: SItem.path_to_item()

.. automethod:: SItem.path_to_feature()

.. automethod:: SItem.path_to_derived_feature()


HRG
===

Methods relating to the item's heterogeneous relation graph
information:

.. automethod:: SItem.relation()

.. automethod:: SItem.as_relation()

.. automethod:: SItem.in_relation()

.. automethod:: SItem.utterance()

.. automethod:: SItem.voice()

