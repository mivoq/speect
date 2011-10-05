
===================
Utterance Processor
===================

An Utterance Processor processes an utterance by extracting information from it
and then modifying it in some way.


.. index:: 
   single: Processors (Python API); SUttProcessor

.. currentmodule:: speect


.. autoclass:: SUttProcessor
 
   .. note::

      The ``SUttProcessor`` object inherits from ``SObject`` in the Speect Engine
      and can therefore be used in functions/methods that require parameters
      of type ``SObject``.


Functions
=========

.. automethod:: SUttProcessor.run()


Features
========

An utterance processor has features which can be accessed by:

.. py:attribute:: SUttProcessor.features

   Get the features that are defined for the utterance processor.

   :return: A map of the utterance processor features.
   :rtype: :class:`SMap`

For example::

   sr = uttproc.features["sampling_rate"]

    
See :class:`SMap` for the implemented operations.

