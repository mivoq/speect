
.. index:: RIFF, Resource Interchange File Format

The `Resource Interchange File Format (RIFF) <http://en.wikipedia.org/wiki/Resource_Interchange_File_Format>`_
is a generic meta-format for storing data in tagged chunks.


.. _python_plugins_riff:

:mod:`speect.audio_riff` -- RIFF Audio Serialization
====================================================

.. automodule:: speect.audio_riff
   :synopsis: RIFF audio serialization support (wav)


:mod:`speect.audio_riff` adds a method to :class:`speect.audio.SAudio`, to save audio
objects to the RIFF format (commonly known as WAV format).

.. automethod:: speect.audio.SAudio.save_riff()
