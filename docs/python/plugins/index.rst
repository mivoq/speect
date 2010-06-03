.. _python_plugins:

===============
Speect Plug-ins
===============

Modules provided by the Speect Plug-ins are:

.. toctree::
   :maxdepth: 1

   arrays
   matrices
   tracks
   audio
   riff
   ebml
   pyobject
   pyobject_ebml
   utt_ebml
   phoneset
   featproc_callback
   uttproc_callback


.. note::
   :mod:`speect.audio` adds an extra method to :class:`speect.SUtterance`, 
   see :meth:`speect.SUtterance.play`

.. note::
   :mod:`speect.riff` adds an extra method to :class:`speect.SUtterance`, 
   see :meth:`speect.SUtterance.save_audio_riff`

.. note::
   :mod:`speect.utt_ebml` adds extra methods to :class:`speect.SUtterance`, 
   see :meth:`speect.SUtterance.save_ebml` and :meth:`speect.SUtterance.load_ebml`

.. note::
   :mod:`speect.featproc_callback` adds an extra method to :class:`speect.SFeatProcessor`, 
   see :meth:`speect.SFeatProcessor.callback`

.. note::
   :mod:`speect.uttproc_callback` adds an extra method to :class:`speect.SUttProcessor`, 
   see :meth:`speect.SUttProcessor.callback`
