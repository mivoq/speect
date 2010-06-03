.. _getting_started:

===============
Getting Started
===============

Text-to-speech synthesis can be viewed as the automated process of mapping
a textual representation of an utterance into a sequence of numbers representing
the samples of synthesized speech [#ref1]_.

.. index:: Natural Language Processing, Digital Signal Processing

This conversion is achieved in two stages as shown in :ref:`tts_conversion`:

    * Natural Language Processing (NLP): Converting the
      textual representation of an utterance into symbolic linguistic
      units.
    * Digital Signal Processing (DSP): Mapping the symbolic
      linguistic units into samples of synthesized speech.


|

.. _tts_conversion:


.. figure:: _static/tts_conversion.*
   :scale: 80 
   :alt: Functional blocks of a text-to-speech synthesizer
   :align: center

   Functional blocks of a text-to-speech synthesizer

|

.. index:: Utterance Processors

Utterance Processor
-------------------

In Speect these functional blocks, which do some or other conversion to their
respective inputs, are known as *utterance processors*. An utterance processor
receives an utterance as input and transforms the utterance in some or other
way based on knowledge of the:

    * input type: an email message requires some extra processing when compare
      to a single line of text,
    * language: phonetization will for example be different for English, when compared
      to isiZulu,
    * and the voice: different voices will have different speaking rates, pitch contours
      and so on.


.. index:: Utterance Type

Utterance Type
--------------

Therefore, we have a pipeline of utterance processors doing transformations on the utterance,
and producing the synthetic speech. Such a collection of utterance processors is known as
an *utterance type*. This enables us to define a different pipeline of utterance processors,
or order of utterance processors, for different input types.


.. index:: Utterance

Utterance
---------

In Speect the *utterance* is the input and output of all utterance processors, even
the DSP processors which generate the speech signal. Internally an utterance is represented
as a *heterogeneous relation graph* [#ref2]_, and all utterance processors just add
information to this structure.


.. index:: Feature Processors

Feature Processors
------------------

Utterance processors also make use of *feature processors*. A feature processor extracts
features from individual units in an utterance, these features can then be used by
the utterance processor. For example, we might have a feature processor that, when
given a word, syllabifies the word and returns the syllables. Feature processors are defined
in a key-value (name - processor implementation) mapping, and are called by utterance
processors by their names. The real power of feature processors becomes apparent when doing
multilingual TTS. We can, for example, reuse an utterance processor and just redefine the
key-value pair of a feature processor (same name, different implementation) to do
syllabification for a different language.


.. index:: Voice

Voice
-----

All of this comes together in the definition of a voice. In Speect, a voice defines
the *utterance types* that can be used for synthesis with the specific voice. Each of
these utterance types defines a pipeline of *utterance processors*. The voice also
defines the *feature processors* key-value mapping, connecting a named feature processor
to a specific implementation, which the utterance processors then can use. Finally,
the voice defines it's data, be that linguistic (phonesets, grapheme to phoneme rules, ...)
or acoustic (unit inventory, acoustic models, ...). Note that everything that a voice
defines can be shared between voices. Figure :ref:`voice_repr` shows a representation
of this voice definition.

|
|

.. _voice_repr:

.. figure:: _static/voice.*
   :scale: 80 
   :alt: Speect voice definition
   :align: center

   Speect voice definition

|

**References**

.. [#ref1] Traber, C., Huber, K., et al. "From multilingual to polyglot speech synthesis", In Proceedings of Eurospeech, pp. 835-838, Budapest, September, 1999.

.. [#ref2] Taylor, P., Black, A.W., and Caley, R. "Heterogeneous relation graphs as a mechanism for representing linguistic information", Speech Communication 33:153-174, 2001.
