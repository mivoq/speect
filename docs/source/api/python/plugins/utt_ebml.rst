.. _python_plugins_utt_ebml:


:mod:`speect.utt_ebml` -- Utterance EBML Serialization
======================================================

.. automodule:: speect.utt_ebml
   :synopsis: EBML utterance serialization support


:mod:`speect.utt_ebml` adds methods to :class:`speect.SUtterance`, to save/load 
utterances to/from files in the EBML format (see :doc:`ebml`).

.. automethod:: speect.SUtterance.save_ebml()

.. automethod:: speect.SUtterance.load_ebml()
