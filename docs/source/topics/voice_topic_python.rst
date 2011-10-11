.. index:: 
   single: Topic Guides; Python Voice Definition

.. _voice_topic_python:

======
Voices
======

The :meth:`speect.SVoice()` method can load a voice that has been defined either
as a JSON_ voice :ref:`definition <voice_topic>`, or as a Python function in a file. The advantage
of loading a voice through a Python function is that one can define the voice's utterance
and feature :ref:`processors <processors_topic_python>` with the voice. Lets create a file
called :file:`my_voice.py` and define a function called ``load_voice`` as follows:

.. code-block:: python
   :linenos:

   def load_voice():
       
       import speect
       import speect.uttproc_cb   # utterance callback interface

       # load voice json file
       voice = speect.SVoice("voice.hts.json") # load voice definition

       # load any required data

       ...

       # create utterance processors 
       
       ...

       # set utterance processors in voice
       
       ...

       return voice

The function loading the voice *must* be named ``load_voice``. On line
7 a JSON voice :ref:`definition <voice_topic>` is loaded, this defines
the voice processors and utterance types. Now other processors can be
created and set in the voice, and the voice is returned. This file,
``my_voice.py``, can now be used to load the voice from within Python
as follows (this example shows how to do it with the Python
interpreter):

.. code-block:: pycon

    $ python
    >>> import speect
    >>> voice = speect.SVoice("my_voice.py")




Example
=======

A complete example can be found at :ref:`examples/python/voice_function`.


.. _JSON: http://www.json.org/
