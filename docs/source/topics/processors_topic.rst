
.. index:: 
   single: Topic Guides; Processors

.. _processors_topic:

==========
Processors
==========

In Speect the utterance and feature processors (as explained in the
:ref:`introduction <how_is_it_implemented>`) are the most important
aspect of TTS. The Speect Engine provides the basic data structures
and controls the utterance processor pipeline defined by the voice
:ref:`utterance types <voice_utt_types>`. Utterance processors modify
the utterance using the :ref:`HRG structures <hrg_topic>`, extracting
features from items with feature processor :ref:`paths <hrg_paths>`.

Utterance and feature processors are defined as :ref:`plug-ins
<plugins_topic>` and are described in more detail in the respective
API's:

    	 * :doc:`Processors C API <processors_topic_c>`
	 * :doc:`Processors Python API <processors_topic_python>`
