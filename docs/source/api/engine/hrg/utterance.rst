
=========
Utterance
=========

An Utterance consists of a set of Relations which in turn consists of a set of Items.

.. toctree::
   :hidden:

   SUtterance_detail


Definitions
===========

.. index:: 
   single: Heterogeneous Utterance Graphs (C API); SUtterance

.. doxybridge:: SUtterance
   :type: speect object
   :members: none
   :inheritance: SObject.SUtterance

.. index:: 
   single: Heterogeneous Utterance Graphs (C API); SUtteranceClass

.. doxybridge:: SUtteranceClass
   :type: speect class
   :members: none
   :inheritance: SObjectClass.SUtteranceClass

.. seealso:: :doc:`SUtterance_detail`.


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


Macros
======

.. doxybridge:: S_UTTERANCE
   :type: macro
