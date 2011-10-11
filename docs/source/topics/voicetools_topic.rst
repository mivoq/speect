.. index:: 
   single: Topic Guides; Creating New Voices

.. index:: ! Voicetools


.. _creating_new_voices:

===================
Creating New Voices
===================

Introduction
============

This document describes the basic process in order to compile a new
voice for Speect with :ref:`Voicetools <voicetools>`, given a number
of basic resources. Due to the flexibility of Speect, this is not a
comprehensive text on the topic, but rather a starting point
describing the generic process and giving an example of how the
accompanying scripts may be used to this end.

A :ref:`voice <voice_topic>` in Speect consists of a collection of
``data resources``, code modules or ``plugins`` and a single
``definition`` file describing how these are used during synthesis.

In this document we will assume pre-existing ``plugins`` and ``voice
definition`` files and describe the creation of basic data resources.



Setting up basic resources
==========================

It is convention to store the ``voice definition`` and other resource
files as a collection in a single directory. Thus, the following steps
should be carried out:

1. Create a directory where voice definition and resource files will
be stored (``$VOICEDIR``).

2. Create a ``voice definition`` file (``$VOICEDEF``) in
``$VOICEDIR``. Examples of such a file can be found in
``./examples/voice.*.json``. See the :ref:`voice definition example
<examples/misc/voice_def>`.

3. Define a phoneset (e.g. ``./examples/phoneset.json``), place this
file in ``$VOICEDIR`` and ensure that it is specified in ``$VOICEDEF``

4. Define a pronunciation lexicon and/or addendum
(e.g. ``./examples/lexicon.json`` and ``./examples/addendum.json``) by
placing these files in ``$VOICEDIR`` and specifying them in
``$VOICEDEF``



Compiling grapheme-to-phoneme rewrite rules
===========================================

A simple utility (``./scripts/compile_g2p.py``) is provided to compile
rewrite rules defined in semicolon delimited text files (e.g. as can
be exported by DictionaryMaker -
http://dictionarymaker.sourceforge.net/) to Speect format.

The following is an example of fields in such a semicolon delimited
rules file::

 a;;;a;0;5599
 a;a;;0;1;2364
 a;;a;A;2;2252
 a;;0sie;A;3;174
 a;;re;A;4;103

where a single field per line has the form::

 grapheme;leftcontext;rightcontext;phoneme;ordinal;statistic

Empty fields denote any context and rules are ordered from most to
least general. The ``statistic`` field is not important here. Other
simple semicolon delimited mapping files can also be loaded to perform
grapheme or phoneme representation mappings as well as specify rules
for the insertion of graphemic nulls. These files all have fields of
the form: ``inputmapping;outputmapping`` with one rule per line
similar to the rules file example above.

The exact syntax and additional options can be obtained by running
with the ``--help`` flag and an example of a metadata configuration
file for use with this utility can be found in ``./examples/g2p.conf``



Performing phonetic alignments
==============================

Given a set of audio files in RIFF Wave format and a text file
containing corresponding orthographic transcriptions
(e.g. ``./examples/utts.data``), the TTS frontend can be used to
perform automatic phonetic alignment in order to derive acoustic
models for synthesis. For this purpose, the scripts
``./scripts/do_build_setup.py`` and ``./scripts/do_alignments.py`` can
be used as described below.

Running::

 do_build_setup.py -w $SOURCE_WAVDIR -u $SOURCE_TRANSCRFILE -o $BUILD_ROOTDIR

should set up a directory structure at ``$BUILD_ROOTDIR`` to contain a
default configuration and downsampled, power normalised audio files
which looks like this::


 .
 |-- etc
 |   |-- feats.conf
 |   |-- halign.conf
 |   |-- halign_feats.conf
 |   |-- halign_hmmproto
 |   `-- utts.data
 `-- wavs
     |-- ...
     |-- ...


The configuration files in ``$BUILD_ROOTDIR/etc`` can be edited if
desired (an important speaker-specific parameter is the voice pitch
range which can be set in ``feats.conf``).

Once the configuration is satisfactory, the alignment process can be
done by running the following in ``$BUILD_ROOTDIR``::

 do_alignments.py -v $VOICEDEF to_textgrid

This outputs alignments in Praat's (http://www.fon.hum.uva.nl/praat/)
TextGrid format. These files can be edited (as long as the word
sequences stay intact) and then incorporated in aligned Speect format
utterance files with the command::

 do_alignments.py -v $VOICEDEF from_textgrid

If manual correction is not required, the above two commands can be
replaced with the following::

 do_alignments.py -v $VOICEDEF auto



Compiling acoustic representations
==================================

Unit selection catalogue
------------------------

To compile a halfphone database given phonetic alignments, run the
following in ``$BUILD_ROOTDIR``::

 do_halfphone_catalogue.py -v $VOICEDEF auto

This performs the relevant feature extraction and compiles a halfphone
database file in Speect format. For more options run with ``--help``


Hidden Markov Models (HTS)
--------------------------

A script (``scripts/utt2lab.py``) is provided to generate HTS labels
from aligned Speect utterances. To train speech models, the HTS-demo
scripts (available at http://hts.sp.nitech.ac.jp/?Download) can be
used in conjunction with ``utt2lab.py``
