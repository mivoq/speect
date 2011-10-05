.. _python/engine/relation:

.. index:: 
   single: Heterogeneous Relation Graphs (Python API); SRelation

=========
SRelation
=========

.. currentmodule:: speect

.. autoclass:: SRelation

   .. note::

      The ``SRelation`` object inherits from ``SObject`` in the Speect Engine
      and can therefore be used in functions/methods that require parameters
      of type ``SObject``.


Summary
=======

.. autosummary::
   
   SRelation.append
   SRelation.prepend

   SRelation.head
   SRelation.tail

   SRelation.name
   SRelation.utterance


Item Creation
=============

Methods relating to the creation of items in a relation:

.. automethod:: SRelation.append()

.. automethod:: SRelation.prepend()


Traversing
==========

Methods relating to the traversing of items in a relation:

.. automethod:: SRelation.head()

.. automethod:: SRelation.tail()


Iteration
=========

**for item in relation**

        SRelation supports the Python iterator protocol and therefore one can iterate over
	the :class:`items <SItem>` in a relation.

Information
===========

Methods relating to relation information.

.. automethod:: SRelation.name()

.. automethod:: SRelation.utterance()


Printing
========

**str(relation)**
	
	SRelation supports the Python method ``__str__()`` and can therefore be
	used in the ``str()`` built-in function and by the ``print`` statement
	to compute the "informal" string representation of the relation.

