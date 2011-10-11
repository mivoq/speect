.. _c/engine/main:

===================
Speect Engine C API
===================

The engine is completely independent of any language or waveform
generation modules, and is solely responsible for the loading of
voices and their associated data and plug-ins, and controlling the
synthesis process. The engine is not dependent on any external
libraries, and provides lower level functionality to the plug-ins.
The engine consists of the base system and the object framework.
The :ref:`base <base/main>` system provides a low level library to the
following modules:

      * :ref:`standard defines/macros as well as build time
        definitions <include/main>`,
      * :ref:`abstract data types <base_containers/main>` (lists,
        buffers, hash tables),
      * :ref:`utilities <utils/main>` (memory allocation,
        byte-swapping, timing, fundamental types, versioning, math and
        system path functions),
      * :ref:`error handling, debugging <errdbg_main_c>` and
        :ref:`logging <log_main_c>`,
      * :ref:`platform independent concurrency abstraction
        <threads/main>`,
      * :ref:`UTF-8 string handling <strings/main>` (character and
        string level functions, printing functions and regular
        expressions),
      * and an :ref:`object system <objsystem_main_c>`.


The object system allows an object-oriented programming approach to
the higher level libraries implemented in the object framework. These
higher level libraries provide the following modules:

	* :ref:`containers <containers_main_c>` (map, list),
	* :ref:`data sources <datasources/main>` and :ref:`data serialization <serialization/main>`,
	* :ref:`heterogeneous relation graphs <hrg_main_c>` (HRGs) (for internal utterance representation),
	* :ref:`utterance and feature processors <processors_c_api>`,
	* :ref:`plug-in manager <pluginmanager_main_c>`,
	* and a :ref:`voice manager <voicemanager/main>`.


.. toctree::
   :hidden: 

   include/main
   base/main
   containers/main
   datasources/main
   hrg/main
   main/main
   pluginmanager/main
   serialization/main
   voicemanager/main
   processors/main
