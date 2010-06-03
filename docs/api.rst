.. _api:

====================
Architecture and API
====================

The Speect source code is organized into two major sections:

    * **Engine**: which drives the synthesis process. The engine does not contain any
      language or signal processing code so as to make it independent of any specific
      implementations and thereby providing flexibilty to use the engine with any TTS
      methods.
    * **Plug-ins**: implements the language and signal processing code, which is then
      used by the engine for synthesis. This architecture allows any voice type (i.e.
      *unit selection*, *statistical parametric synthesis*), and miltilingual voices to 
      use the engine by just loading the appropriate plug-in.

.. note::
   Speect is still under constant and active development, and therefore the API
   may change. We consider the Speect Engine API to be more stable than the 
   Speect Python interface API, plug-ins and applications.

|
|

.. _speect_arch:


.. figure:: _static/speect_arch.*
   :scale: 80 
   :alt: Speect architecture
   :align: center

   Speect architecture

|

Speect Engine
-------------

.. htmlonly::

   :Release: |version|
   :Date: |today|

The Speect Engine API is documented in the source code header files in the doxygen format, 
and can be accessed from the :addlink:`link` 


Speect Python Interface
-----------------------

The Speect Python interface is automatically generated from `SWIG <http://www.swig.org>`_ 
interface files.


.. htmlonly::

   :Release: |version|
   :Date: |today|

.. toctree::
   :maxdepth: 2

   python/engine/index
   python/plugins/index
