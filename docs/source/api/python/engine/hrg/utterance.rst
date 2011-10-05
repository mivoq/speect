.. _python/engine/utterance:

.. index:: 
   single: Heterogeneous Relation Graphs (Python API); SUtterance

==========
SUtterance
==========

.. currentmodule:: speect

.. autoclass:: SUtterance

   .. note::

      The utterance is deleted when the ``SUtterance`` object is deleted.

   .. note::

      The ``SUtterance`` object inherits from ``SObject`` in the Speect Engine
      and can therefore be used in functions/methods that require parameters
      of type ``SObject``
 

Summary
=======

.. autosummary::

   SUtterance.relation_new
   SUtterance.relation_get
   SUtterance.relation_set
   SUtterance.relation_del

   
   SUtterance.features

   SUtterance.voice



Relations
=========

Methods relating to the utterance's relations:

.. automethod:: SUtterance.relation_new()

.. automethod:: SUtterance.relation_get()

.. automethod:: SUtterance.relation_set()

.. automethod:: SUtterance.relation_del()


Membership
----------

SUtterance supports the Python method ``__contains__()`` , therefore
membership type tests for relations can be done:


**key in utterance**

        Return ``True`` if *utterance* has a relation named *key*, else ``False``.


**key not in utterance**

        Equivalent to ``not key in utterance``.


Iteration
=========

**for key in utterance**

        SUtterance supports the Python iterator protocol and therefore one can iterate over
	a relation names (*key*) in the utterance.

Features
========

An utterance has features which can be accessed by:

.. py:attribute:: SUtterance.features

   Get the features that are defined for the utterance.

   :return: A map of the utterance features.
   :rtype: :class:`SMap`

For example::

   input_type = utterance.features["input"]

    
See :class:`SMap` for the implemented operations.


Voice
=====

.. autoattribute:: SUtterance.voice


Printing
========

**str(utterance)**
	
	SUtterance supports the Python method ``__str__()`` and can therefore be
	used in the ``str()`` built-in function and by the ``print`` statement
	to compute the "informal" string representation of the utterance.

