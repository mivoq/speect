
.. index:: 
   single: Topic Guides; Python Processors

.. _processors_topic_python:

==========
Processors
==========

Processors modify or extract features from utterances
(:class:`speect.SUtterance`) or items (:class:`speect.SItem`). See
:ref:`Processors Python API <processors_python_api>` for a detailed description
of the API.


Utterance Processor
===================

An utterance processor (:class:`speect.SUttProcessor`) receives an utterance
as input and transforms the utterance in some or other way based on
knowledge of the:

	  * input type: an email message requires some extra
	    processing when compare to a single line of text,
	  * language: phonetization will for example be different for
	    English, when compared to isiZulu,
	  * and the voice: different voices will have different
            speaking rates, pitch contours and so on.

During the synthesis process each utterance processor defined for the
:ref:`utterance type <voice_utt_types>` requested in synthesis (see
:meth:`speect.SVoice.synth`) is called with the
:meth:`speect.SUttProcessor.run` function. Python implemented
utterance processors can replace an existing utterance processors of
an :ref:`utterance type <voice_utt_types>`, or implement a new
utterance processor that can be used in some or other utterance type.
Utterance processors in Python are implemented as *callback* functions
with the :mod:`speect.uttproc_cb` plug-in and set with
:meth:`speect.SVoice.uttProcessor_set` in the voice. 

As an example we will implement an utterance processor that adds pauses
to the beginning and end of the ``Segment`` relation of an
utterance. First we implement the function:

.. code-block:: python

   def add_segment_pauses(utt):

       # get the Segment relation
       segment_rel = utt.relation_get("Segment")

       # prepend an item onto the segment relation, no shared content
       segment_item = segment_rel.prepend()

       # set it's name to "pau"
       segment_item["name"] = "pau"

       # append an item onto the segment relation, no shared content
       segment_item = segment_rel.append()

       # set it's name to "pau"
       segment_item["name"] = "pau"

Now an utterance processor based on the above function can be created
with the :mod:`speect.uttproc_cb` plug-in and set in the voice:

.. code-block:: python

   import speect
   import speect.uttproc_cb

   
   # load voice ...
   # ...

   # create utterance processor   
   utt_processor = speect.SUttProcessor.callback(add_segment_pauses)

   # add utterance processor to voice
   voice.uttProcessor_set("SegmentPauser", utt_processor)


Our utterance processor is called "SegmentPauser" in the voice. 
During the synthesis process when the Speect Engine executes the **C**
``run`` function of the processor (see :ref:`SUttProcessorClass
structure <SUttProcessorClass_Structure>` ``run``), the
``add_segment_pauses`` **Python** function will be called when the "SegmentPauser"
utterance processor is executed.


Feature Processors
==================

Utterance processors also make use of :class:`feature processors
<speect.SFeatProcessor>`. A feature processor extracts features from
individual units (items, :class:`speect.SItem`) in an utterance, these
features can then be used by the utterance processor. Feature
processors are defined in a key-value (name - processor
implementation) mapping in a voice, and are called by their names.
Feature processors in Python are implemented as *callback* functions
with the :mod:`speect.featproc_cb` plug-in and set with
:meth:`speect.SVoice.featProcessor_set` in the voice.

As an example we will implement an feature processor that extracts the
end time values of segments. First we implement the function:

.. code-block:: python

   def get_segment_end_time(item):

       # get the segment item's end time feature
       if "end" in item:
           end_time = item["end"]
       else:
           end_time = 0.0

       return end_time

Now a feature processor based on the above function can be created
with the :mod:`speect.featproc_cb` plug-in and set in the voice:

.. code-block:: python

   import speect
   import speect.featproc_cb

   
   # load voice ...
   # ...

   # create feature processor   
   feat_processor = speect.SFeatProcessor.callback(get_segment_end_time)

   # add feature processor to voice
   voice.featProcessor_set("seg_end", feat_processor)

Our feature processor is called "seg_end" in the voice, and for
example can now be used in :ref:`item paths <hrg_paths>` or in
utterance processors or any place where necessary as it is defined in
the voice.
