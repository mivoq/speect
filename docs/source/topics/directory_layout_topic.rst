.. _style_guide_topic:

.. index:: 
   single: Topic Guides; Directory Layout

.. index:: ! Directory Layout

================
Directory Layout
================

The top level directory layout is as follows::

    speect
    ├── build            -- where the build is placed
    ├── cmake            -- top level project specific CMake files
    ├── copying          -- licensing information
    ├── docs             -- this documentation 
    ├── engine           -- engine 
    ├── plugins          -- plug-ins
    └── swig             -- SWIG output


.. index::
   single: Directory Layout; Engine

Engine
======

The ``speect/engine/`` directory layout is as follows::

    speect/engine/
    ├── cmake            -- engine specific CMake files
    ├── config           -- files that will be configured during build time 
    ├── examples         -- some examples
    ├── src              -- engine source files
    ├── swig             -- engine SWIG files
    └── tests            -- some tests

The directory layout of Speect Engine, ``speect/engine/src/source``,
follows the :ref:`system architecture <system_arch_engine>`::

    speect/engine/src/
    ├── base             -- base system provides low level modules (all non object-oriented)
    │   ├── containers      -- abstract data types (lists, buffers, hash tables)
    │   ├── errdbg          -- error handling and debugging 
    │   ├── log             -- logging system
    │   ├── objsystem       -- generic object system
    │   ├── strings         -- UTF-8 string handling (character, string, printing and regular expressions)
    │   ├── threads         -- platform independent concurrency abstraction
    │   └── utils           -- utilities (memory, byte-swapping, time, types, versioning, math and path)
    ├── containers       -- containers (map, list)
    ├── datasources      -- data sources 
    ├── hrg              -- heterogeneous relation graphs (HRGs)
    ├── include          -- system definitions and macros
    ├── main             -- main entry point for library
    ├── pluginmanager    -- plug-in manager
    ├── serialization    -- data serialization
    └── voicemanager     -- voice manager and voice interface


.. index::
   single: Directory Layout; Plug-ins

Plug-ins
========

The ``speect/plugins`` directory layout is as follows::


    speect/plugins/
    ├── acoustic              -- acoustic specific plug-ins
    │   ├── audio                -- low-level audio access abstraction
    │   ├── cost_functions       -- unit selection acoustic cost functions
    │   ├── unit_db              -- unit selection database abstraction (interface)
    │   ├── wave_generation      -- waveform generation algorithms
    │   └── window_functions     -- DSP window functions
    ├── cmake                 -- plug-ins specific CMake files
    ├── config                -- files that will be configured during build time 
    ├── languages             -- language specific implementations (syllabification, ...)
    ├── linguistic            -- linguistic specific plug-ins
    │   ├── addendum          -- addendum abstraction (interface)
    │   ├── cost_functions    -- unit selection linguistic cost functions
    │   ├── g2p               -- grapheme-to-phoneme abstraction (interface)
    │   ├── lexicon           -- lexicon abstraction (interface)
    │   ├── phoneset          -- phoneset abstraction (interface)
    │   └── syllabification   -- syllabification abstraction (interface)
    ├── processors            -- processor implementations
    │   ├── features             -- feature processor implementations
    │   └── utterances           -- utterance processor implementations
    ├── serialization         -- data serialization
    │   ├── files                -- file serialization (utterance, g2p, audio, ...)
    │   ├── formats              -- format implementations (EBML, ... )
    │   └── objects              -- object serialization (arrays, Speect objects, ...)
    ├── template              -- plug-in template
    └── utils                 -- utilities 
        ├── arrays               -- array implementations (float, int)
        ├── cost_function        -- unit selection cost function abstraction (interface)
        ├── matrices             -- matrices implementation (float, int)
        ├── tokenizer            -- simple string tokenizer
        ├── tracks               -- track implementation
        └── viterbi              -- viterbi implementation


Each plug-in implementation is in its own directory. There is always
an abstract class that defines the class methods or interface and then
implementations of that class or interface.  For example, the g2p
interface is in located in ``speect/plugins/linguistic/g2p``, and then
there is also a g2p implementation located at
``speect/plugins/serialization/files/g2p_rewrites`` which implements
the g2p interface as a g2p rewrites type where the rules are read from
a file. Another example is where the unit selection cost functions
class (which defines the class and the methods or interface for cost
functions) is defined in ``speect/plugins/utils/cost_function`` while
the cost function implementation to calculate the cost of the number
of syllables of a unit is in
``speect/plugins/linguistic/cost_functions/num_syls``.

The Speect Engine defines classes for the utterance and feature
processors, therefore no plug-ins are required to define these and
there are only implementations defined, see for example the
``speect/plugins/processors/features/nlp`` directory which is given
below::

    speect/plugins/processors/features/nlp/
    ├── hts_labels
    ├── phrase_num_syls
    ├── phrase_num_words
    ├── phrase_pos_utt
    ├── phrase_pos_utt_rev
    ├── segment_duration
    ├── segment_end
    ├── segment_mid
    ├── segment_name_multilingual
    ├── segment_phoneset_feature
    ├── segment_pos_syl
    ├── segment_pos_syl_rev
    ├── segment_start
    ...

Most of these directories contain *one* implementation of a feature
processor (NLP processors in this case), except where it might be
natural to group them together, for example the
``speect/plugins/processors/features/nlp/hts_labels`` directory which
contains directories of different *HTS label* feature processor
implementations.
