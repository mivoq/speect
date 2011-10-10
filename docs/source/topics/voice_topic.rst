.. index:: ! Voice Definition

.. index:: 
   single: Topic Guides; Voice Definition

.. _voice_topic:

======
Voices
======

As explained in the :ref:`introduction <intro_voice>` a voice defines
the utterance types that can be used for synthesis with the specific
voice. Each of these utterance types defines a pipeline of utterance
processors. The voice also defines the feature processors key-value
mapping, connecting a named feature processor to a specific
implementation, which the utterance processors then can use. Finally,
the voice defines it’s data, be that linguistic (phonesets, grapheme
to phoneme rules, ...) or acoustic (unit inventory, acoustic models,
...). 

In Speect voices are defined in JSON_ format files. Here we will
discuss the layout and meaning of a voice file.


Information
===========

The first section of a voice file contains the *voice definition*, in
which basic information of the voice is defined. This section is
required in all voice files. Below is an example:

.. code-block:: javascript

    "voice-definition" :
    {
      "name" : "Afrikaans ZA",
      "description" : "South African Afrikaans HTS voice.",
      "gender" : "female",
      "language" : "afrikaans",
      "lang-code" : "afr",
      "version" :
      {
	"major" : 0,
	"minor" : 0
      }
    }


Plug-ins
========

The next section defines plug-ins which Speect must load. These are
plug-ins that are not critical to the working of the voice, but may be
necessary for other types of operations. In the example below the
``utt_ebml.spi`` :ref:`plug-in <plugins_topic>` will be loaded, which
provides methods for saving an utterance to an EBML_ formatted file.

.. code-block:: javascript

    "plug-ins" : 
    [
      "utt_ebml.spi" /* for loading/saving utterances */
    ]


Data
====

The data section of the voice defines linguistic and acoustic data that is required during
synthesis. Each entry *must* define the following three variables:

	   * *format* : The format of the data.
	   * *plug-in* : The plug-in that is required to load the above mentioned format of the data.
	   * *path* : The location of the data, this may be relative to the voice file.

Below is an extract of the data section of a voice file:

.. code-block:: javascript

    "data" :
    {
      "phoneset" :
      {
	"format" : "spct_phoneset",
	"plug-in" : "phoneset_json.spi",
	"path" : "etc/phoneset.json"
      },
      "lexicon" :
      {
	"format" : "spct_lexicon",
	"plug-in" : "lexicon_json.spi",
	"path" : "etc/lexicon.json"
      }
    }

The plug-ins will load the data in the described format from the given
path and set it in the voice with the associated data name (for
example "phoneset"). The C and Python API functions,
:c:func:`SVoiceGetData` and :meth:`speect.SVoice.data_get`, can then
used to retrieve this data from the voice.


Features
========

The voice features is defined as a dictionary in the voice file. There
is no restriction on the dictionary, as long as it confines to the
JSON_ format, and sub-elements of the dictionary may themselves
contain dictionaries or lists. During the voice loading this
dictionary will be read and set as the voice's features, no
interpretation of this information is done by the voice manager.

The C and Python API, :c:func:`SVoiceGetFeature` and
:attr:`speect.SVoice.features`, can be used to access this
information.

Below is an example of a voice features definition:

.. code-block:: javascript

    "features" : 
    {
      "hts engine data" :
      {
	"duration" :
	{
	  "trees" : [ "hts/tree-dur.inf" ],
	  "pdfs" : [ "hts/dur.pdf" ]
	},
	"log F0" :
	{
	  "trees" : [ "hts/tree-lf0.inf" ],
	  "pdfs" : [ "hts/lf0.pdf" ],
	  "windows" : 
	  [
	    "hts/lf0.win1",
	    "hts/lf0.win2",
	  ]
	}
      }
    }  


Feature Processors
==================

The feature processors section defines a list of *named* feature processors, which can be requested
by there names and executed on items. Each entry *must* define the following two variables:

	   * *class* : The class of the feature processor (here the term class is used for the object type).
	   * *plug-in* : The plug-in that defines the above mentioned class implementation.

Below is an extract of the feature processors section of a voice file:

.. code-block:: javascript

   "feature-processors" : 
    {
      "segment_duration" :
      {
 	"class" : "SSegDurFeatProc",
	"plug-in" : "seg_dur_featproc.spi"
      },
      "segment_end" :
      {
 	"class" : "SSegEndFeatProc",
	"plug-in" : "seg_end_featproc.spi"
      },
      "segment_mid" :
      {
 	"class" : "SSegMidFeatProc",
	"plug-in" : "seg_mid_featproc.spi"
      }
    }

The voice manager will load the plug-in and instantiate an object of the type named in the
"class" variable. This object is then saved in the voice's feature processors dictionary with
it's associated name. 


.. _voice_utt_proc:

Utterance Processors
====================

The utterance processors section is almost the same as the features processors, except that an optional
variable, "features", can be added:

	   * *class* (required) : The class of the utterance processor (here the term class is used for the object type).
	   * *plug-in* (required) : The plug-in that defines the above mentioned class implementation.
	   * *features* (optional) : A JSON_ dictionary format of features that are available for the utterance processor.

Below is an extract of the utterance processors section of a voice file:

.. code-block:: javascript

    "utterance-processors" :
    {
      "Tokenize" :
      {
	"class" : "STokenizationUttProc",
	"plug-in" : "tokenization_uttproc.spi",
	"features" : /* features of the utterance processor */
	{
	  "tokenizer symbols" :
	  {
	    "whitespace" : " \t\n\r",
	    "single-char" : "(){}[]",
	    "pre-punctuation" : "\"'`({[",
	    "post-punctuation" : "\"'`.,:;!?(){}[]"
	  }
	}
      },
      "Normalize" :
      {
	"class" : "SNormalizationUttProc",
	"plug-in" : "normalization_uttproc.spi"
      }
    }

The voice manager will load the plug-in and instantiate an object of
the type named in the "class" variable. The features dictionary, if
defined, will then be set as the utterance processor object's
features. The :c:func:`SUttProcessorInit` function is also called so
that the utterance processor can do any initialization (for example
load files defined in it's features) if required. This object is then
saved in the voice's utterance processors dictionary with it's
associated name.

.. _voice_utt_types:

Utterance Types
===============

Utterance types define a pipeline of utterance processors, doing
transformations on the utterance, and producing the synthetic speech.
The utterance types are defined as a dictionary of utterance type
names with an associated list of utterance processor names.

Below is an extract of the utterance types section of a voice file:

.. code-block:: javascript

    "utterance-types" : 
    {
      "text" :
      [
	"Tokenize",
	"Normalize",
	"Phrasify",
	"LexLookup",
	"Pauses",
	"HTS Engine Synthesizer"
      ],

      "text-to-segments" :
      [
	"Tokenize",
	"Normalize",
	"Phrasify",
	"LexLookup",
	"Pauses"
      ]
    }

During synthesis a specific utterance type is requested (see
:c:func:`SVoiceSynthUtt` and :meth:`speect.SVoice.synth`), and for this
type the named utterance processors are called in the defined sequence
and executed on the utterance. 


Example
=======

A complete example of a voice file can be found at :ref:`examples/misc/voice_def`.



.. _JSON: http://www.json.org/

.. _EBML: http://en.wikipedia.org/wiki/Extensible_Binary_Meta_Language
