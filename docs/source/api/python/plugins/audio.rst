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

   .. note::

      This function first tries to import :mod:`pyaudio`
      (`<http://people.csail.mit.edu/hubert/pyaudio/>`_), and use pyaudio
      for playback. If pyaudio is not available then on ``posix``
      systems, it tries to open an audio device (environment variable
      :envvar:`AUDIODEV`, or fallback ``/dev/dsp``) and write 16 bit
      *little endian* values to it.
   
