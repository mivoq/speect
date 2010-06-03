.. _python_plugins_riff:

.. index:: RIFF, Resource Interchange File Format

The `Resource Interchange File Format (RIFF) <http://en.wikipedia.org/wiki/Resource_Interchange_File_Format>`_
is a generic meta-format for storing data in tagged chunks.

:mod:`speect.riff` -- RIFF Audio Serialization
==============================================

.. automodule:: speect.riff
   :synopsis: RIFF audio serialization support (wav)


:mod:`speect.riff` adds a method to :class:`speect.SUtterance`, to save audio
objects to the RIFF format (commonly known as WAV format).

.. automethod:: speect.SUtterance.save_audio_riff()
