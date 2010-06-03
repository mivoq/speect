.. _python_plugins_audio:


:mod:`speect.audio` -- Waveform Container 
=========================================

.. module:: speect.audio
   :synopsis: Waveform container class

.. autoclass:: SAudio

.. automethod:: SAudio.num_samples()

.. automethod:: SAudio.sample_rate()

.. automethod:: SAudio.get_audio_waveform()


:mod:`speect.audio` also adds a method to play audio objects through
``dev/dsp`` to the :class:`speect.SUtterance` class.

.. automethod:: speect.SUtterance.play()
