.. index:: Quick Start (Python)

.. _python_quick_start:

==================
Python Quick Start
==================

:doc:`download` Speect and look at :ref:`build_install` or get the
Debian :ref:`packages <debian_packages>`.  Download a voice from
`<http://hlt.mirror.ac.za/TTS/Speect/>`_.

Unpack the voice in some directory::

    tar -xzf cmu_arctic_slt-1.0.tar.gz

    cd cmu_arctic_slt

.. note::
   This voice does not have extensive NLP modules. Therefore,
   text to be synthesized must be plain text (normalized). For
   example:: "we want one goal" and *not* "we want 1 goal".


Run the Python interpreter::

    $ python

Import the Speect module::

    >>> import speect

Import the Speect audio module for playing utterances::

    >>> import speect.audio

Load the voice config file::

    >>> voice = speect.SVoice("voice.json")

.. seealso::

   :py:class:`speect.SVoice`


Synthesize some text::

    >>> utt = voice.synth("this is speect from within python")

.. seealso::

   :py:class:`speect.SUtterance`


Play the synthesized audio::

    >>> utt.play()

.. note::

   See :py:meth:`speect.SUtterance.play()` for more information if
   playback fails.

View the utterance structure::

    >>> print utt

Save the utterance by first importing the audio riff module::

    >>> import speect.audio_riff

Get the utterance's audio feature::

    >>> audio = utt.features["audio"]

.. seealso::

   :py:class:`speect.SUtterance.features`


and save it to a ``WAV`` format file::

    >>> audio.save_riff("temp.wav")

