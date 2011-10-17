.. _python/engine/voice:

.. index:: Voices (Python API)

.. index::
   single: Voices (Python API); SVoice


======
SVoice
======

.. currentmodule:: speect

.. autoclass:: SVoice

   .. note::

      The ``SVoice`` object inherits from ``SObject`` in the Speect Engine
      and can therefore be used in functions/methods that require parameters
      of type ``SObject``.

   .. seealso::

      :ref:`voice_topic_python`


Summary
=======
 
.. autosummary::

   SVoice.synth
   SVoice.resynth
   SVoice.name
   SVoice.description
   SVoice.gender
   SVoice.language
   SVoice.lang_code
   SVoice.version
   SVoice.features
   SVoice.dataObjects
   SVoice.data_get
   SVoice.data_set
   SVoice.data_del
   SVoice.uttTypes
   SVoice.uttType_get
   SVoice.uttType_set
   SVoice.uttType_del
   SVoice.uttProcessors
   SVoice.uttProcessor_get
   SVoice.uttProcessor_set
   SVoice.uttProcessor_del
   SVoice.featProcessors
   SVoice.featProcessor_get
   SVoice.featProcessor_set
   SVoice.featProcessor_del


Synthesis
=========

Methods relating to voice synthesis:

.. automethod:: SVoice.synth()

.. automethod:: SVoice.resynth()


Description
===========

Methods relating to a description of the voice:

.. automethod:: SVoice.name()

.. automethod:: SVoice.description()

.. automethod:: SVoice.gender()

.. automethod:: SVoice.language()

.. automethod:: SVoice.lang_code()

.. automethod:: SVoice.version()


Features
========

An voice has features which can be accessed by:


.. py:attribute:: SVoice.features

   Get the features that are defined for the voice.

   :return: A map of the voice features.
   :rtype: :class:`SMap`

For example::

   config_file = voice.features["config_file"]

    
See :class:`SMap` for the implemented operations.


Data
====

Methods relating to the voice's data:

.. autoattribute:: SVoice.dataObjects

.. automethod:: SVoice.data_get()

.. automethod:: SVoice.data_set()

.. automethod:: SVoice.data_del()


Utterance Types
===============

Methods relating to the voice's utterance types:

.. autoattribute:: SVoice.uttTypes

.. automethod:: SVoice.uttType_get()

.. automethod:: SVoice.uttType_set()

.. automethod:: SVoice.uttType_del()


Utterance Processors
====================

Methods relating to the voice's utterance processors:

.. autoattribute:: SVoice.uttProcessors

.. automethod:: SVoice.uttProcessor_get()

.. automethod:: SVoice.uttProcessor_set()

.. automethod:: SVoice.uttProcessor_del()


Feature Processors
==================

Methods relating to the voice's feature processors:

.. autoattribute:: SVoice.featProcessors

.. automethod:: SVoice.featProcessor_get()

.. automethod:: SVoice.featProcessor_set()

.. automethod:: SVoice.featProcessor_del()

