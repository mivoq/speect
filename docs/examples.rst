.. _examples:

========
Examples
========

Some example usage of the Speect Python interface with voices. 

Download Voices
---------------

There are voices that can be downloaded at the 
`Meraka HLT mirror <http://hlt.mirror.ac.za/TTS/Speect_HTS/>`_.


Unpack the voices in some directory::

    tar -xzf Lwazi.Eng-ZA.Voice.HTS.0.1.tar.gz

    cd eng-ZA

.. note::
   These voices do not have extensive NLP modules. Therefore,
   text to be synthesized must be plain text (normalized). For
   example:: "we want one goal" and *not* "we want 1 goal".


Using A Voice Interactively
---------------------------

Run the Python interpreter::

    $ python

Import the Speect module::

    >>> import speect

Import the Speect audio module for playing utterances::

    >>> import speect.audio

Load the voice config file::

    >>> voice = speect.SVoice("/path/to/eng-ZA/voice.txt")

.. note::
   The path to the voice configuration file must be an absolute path

Synthesize some text::

    >>> utt = voice.synth("this is speect from within python")

Play the synthesized audio::

    >>> utt.play()

View the utterance structure::

    >>> print utt


Miscellaneous
-------------

More examples can be found at :file:`speect/swig/python/examples`


.. index:: Client-Server

A Python client-server implementation can be found at :file:`speect/swig/python`

    





