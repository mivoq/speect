.. _python_engine_svoice:

======
SVoice
======

This section describes the :class:`SVoice` class.


.. currentmodule:: speect


.. autoclass:: SVoice
 

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


.. note::

   The plug-in is unloaded when the ``SPlugin`` object is deleted.

.. note::

   The ``SVoice`` object inherits from ``SObject`` in the Speect Engine
   and can therefore be used in functions/methods that require parameters
   of type ``SObject``.


Synthesis
-----------
Methods relating to voice synthesis. 

.. automethod:: SVoice.synth()

.. seealso::
   :class:`SUtterance`


.. automethod:: SVoice.resynth()

.. seealso::
   :class:`SUtterance`


Information
-----------
Methods relating to voice information.

.. automethod:: SVoice.name()

.. automethod:: SVoice.description()

.. automethod:: SVoice.gender()

.. automethod:: SVoice.language()

.. automethod:: SVoice.lang_code()

.. automethod:: SVoice.version()


Features
--------
Voice features is a voice attributes.

.. autoattribute:: SVoice.features

.. seealso::
   :class:`SMap`


Data
----
Methods relating to the voice's data.

.. autoattribute:: SVoice.dataObjects

.. automethod:: SVoice.data_get()

.. automethod:: SVoice.data_set()

.. automethod:: SVoice.data_del()



.. index::
   single: Utterance Types; SVoice

.. _python_engine_voice_utttypes:

Utterance Types
---------------
Methods relating to the voice's utterance types.

.. autoattribute:: SVoice.uttTypes

.. automethod:: SVoice.uttType_get()

.. automethod:: SVoice.uttType_set()

.. automethod:: SVoice.uttType_del()


.. index::
   single: Utterance Processors; SVoice

.. _python_engine_voice_uttprocs:

Utterance Processors
--------------------
Methods relating to the voice's utterance processors.

.. autoattribute:: SVoice.uttProcessors

.. automethod:: SVoice.uttProcessor_get()

.. automethod:: SVoice.uttProcessor_set()

.. automethod:: SVoice.uttProcessor_del()

.. seealso::
   :class:`SUttProcessor`


.. index::
   single: Feature Processors; SVoice

.. _python_engine_voice_featprocs:

Feature Processors
--------------------
Methods relating to the voice's feature processors.

.. autoattribute:: SVoice.featProcessors

.. automethod:: SVoice.featProcessor_get()

.. automethod:: SVoice.featProcessor_set()

.. automethod:: SVoice.featProcessor_del()

.. seealso::
   :class:`SFeatProcessor`

