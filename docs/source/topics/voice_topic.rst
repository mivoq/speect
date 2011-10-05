.. _voice_topic/main:

.. index:: 
   single: Topic Guides (C API); Voices

======
Voices
======

Text-to-speech (TTS) synthesis is the automated process of mapping a
textual representation of an utterance into a sequence of numbers
representing the samples of synthesized speech.  This conversion is
achieved in two stages as depicted in :ref:`figure 1
<figure_tts_con>`:

     * Natural Language Processing (NLP): Converting the textual
       representation of an utterance into symbolic linguistic units.
     * Digital Signal Processing (DSP): Mapping the symbolic
       linguistic units into samples of synthesized speech.


|

.. _figure_tts_con:

.. figure:: ../../figures/tts_con.png
   :scale: 75%
   :alt: 
   :align: center

   Figure 1: Functional blocks of a text-to-speech synthesizer.

|


Utterance Processor
===================

In Speect these functional blocks, which do some or other conversion
to their respective inputs, are known as :c:type:`utterance processors
<SUttProcessor>`. An utterance processor receives an utterance as
input and transforms the utterance in some or other way based on
knowledge of the:

	  * input type: an email message requires some extra
	    processing when compare to a single line of text,
	  * language: phonetization will for example be different for
	    English, when compared to isiZulu,
	  * and the voice: different voices will have different
            speaking rates, pitch contours and so on.


Utterance Type
==============

Therefore, we have a pipeline of utterance processors doing
transformations on the utterance, and producing the synthetic
speech. Such a collection of utterance processors is known as an
*utterance type*. This enables us to define a different pipeline of
utterance processors, or order of utterance processors, for different
input types.


Utterance
=========

In Speect the :c:type:`utterance <SUtterance>` is the input and output
of all utterance processors, even the DSP processors which generate
the speech signal. Internally an utterance is represented as a
:doc:`heterogeneous relation graph <hrg_topic>`, and all utterance
processors just add information to this structure.


Feature Processors
==================

Utterance processors also make use of :c:type:`feature processors
<SFeatProcessor>`. A feature processor extracts features from
individual units in an utterance, these features can then be used by
the utterance processor. For example, we might have a feature
processor that, when given a word, syllabifies the word and returns
the syllables. Feature processors are defined in a key-value (name -
processor implementation) mapping, and are called by utterance
processors by their names. The real power of feature processors
becomes apparent when doing multilingual TTS. We can, for example,
reuse an utterance processor and just redefine the key-value pair of a
feature processor (same name, different implementation) to do
syllabification for a different language.


Voice
=====

All of this comes together in the definition of a :c:type:`voice
<SVoice>`. In Speect, a voice defines the utterance types that can be
used for synthesis with the specific voice. Each of these utterance
types defines a pipeline of utterance processors. The voice also
defines the feature processors key-value mapping, connecting a named
feature processor to a specific implementation, which the utterance
processors then can use. Finally, the voice defines it’s data, be that
linguistic (phonesets, grapheme to phoneme rules, ...) or acoustic
(unit inventory, acoustic models, ...). Note that everything that a
voice defines can be shared between voices. :ref:`Figure 2
<figure_voice>` shows a representation of this voice definition.


|

.. _figure_voice:

.. figure:: ../../figures/voice.png
   :scale: 75%
   :alt: 
   :align: center

   Figure 2: Speect voice definition.

|


.. toctree::
   :hidden:

   voice_def

Voices are primarily defined in `JSON`_ formatted text files, :doc:`voice_def`.







.. _JSON: http://www.json.org/
