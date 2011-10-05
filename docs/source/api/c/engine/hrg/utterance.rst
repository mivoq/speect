.. _hrg/utterance:

==========
SUtterance
==========

An Utterance consists of a set of relations which in turn consists of a set of items.


Definitions
===========

.. index:: 
   single: Heterogeneous Relation Graphs (C API); SUtterance

.. doxybridge:: SUtterance
   :type: speect object
   :members: none
   :inheritance: SObject.SUtterance

.. toctree::
   :hidden:

   SUtterance_structure

:doc:`SUtterance_structure`

.. index:: 
   single: Heterogeneous Relation Graphs (C API); SUtteranceClass

.. doxybridge:: SUtteranceClass
   :type: speect class
   :members: none
   :inheritance: SObjectClass.SUtteranceClass

.. toctree::
   :hidden:

   SUtteranceClass_structure

:doc:`SUtteranceClass_structure`


Macros
======

.. doxybridge:: S_UTTERANCE
   :type: macro


Functions
=========

Initialization
--------------

.. doxybridge:: SUtteranceInit


Voice
-----

.. doxybridge:: SUtteranceVoice


Relations
---------

.. doxybridge:: SUtteranceNewRelation

.. doxybridge:: SUtteranceGetRelation

.. doxybridge:: SUtteranceSetRelation

.. doxybridge:: SUtteranceDelRelation

.. doxybridge:: SUtteranceRelationIsPresent

.. doxybridge:: SUtteranceRelationsKeys


Features
--------

.. doxybridge:: SUtteranceGetFeature

.. doxybridge:: SUtteranceSetFeature

.. doxybridge:: SUtteranceDelFeature

.. doxybridge:: SUtteranceFeatureIsPresent

.. doxybridge:: SUtteranceFeatKeys

