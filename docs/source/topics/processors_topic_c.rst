
.. index:: 
   single: Topic Guides; C Processors

.. _processors_topic_c:

==========
Processors
==========

Processors modify or extract features from utterances
(:c:type:`SUtterance`) or items (:c:type:`SItem`). See
:ref:`Processors C API <processors_c_api>` for a detailed description
of the API.


Utterance Processor
===================

An utterance processor (:c:type:`SUttProcessor`) receives an utterance
as input and transforms the utterance in some or other way based on
knowledge of the:

	  * input type: an email message requires some extra
	    processing when compare to a single line of text,
	  * language: phonetization will for example be different for
	    English, when compared to isiZulu,
	  * and the voice: different voices will have different
            speaking rates, pitch contours and so on.

When a voice is loaded the :c:func:`SUttProcessorInit` function is
called on each of the utterance processors of the voice, where the
utterance processor can do any initialization required (see
:ref:`utterance processors <voice_utt_proc>` for more details of the
loading of a utterance processor plug-in by the *voice manager*).
During the synthesis process each utterance processor defined for the
:ref:`utterance type <voice_utt_types>` requested in synthesis (see
:c:func:`SVoiceSynthUtt`) is called with the
:c:func:`SUttProcessorRun` function. Therefore, implementations of
utterance processors normally do not add any members or methods to the
:c:type:`SUttProcessor` object or :c:type:`SUttProcessorClass` class,
as only these two functions are called.

As an example we will implement an utterance processor that adds pauses
to the beginning and end of the ``Segment`` relation of an
utterance. Since we are not adding any methods or members we can
simply define the utterance processor as:

.. code-block:: c
   
   typedef SUttProcessor SExampleUttProc;

   typedef SUttProcessorClass SExampleUttProcClass;


The ``run`` function of the processor (see
:ref:`SUttProcessorClass structure <SUttProcessorClass_Structure>` ``run``) can be implemented as
follows:

.. code-block:: c
   :linenos:

   static void Run(const SUttProcessor *self, SUtterance *utt, s_erc *error)
   {
       const SRelation *segmentRel;
       SItem *segmentItem;


       S_CLR_ERR(error);
       
       segmentRel = SUtteranceGetRelation(utt, "Segment", error);

       /* prepend an item onto the segment relation, no shared content */
       segmentItem = SRelationPrepend(segmentRel, NULL, error);
	
       /* set it's name to "pau" */
       SItemSetName(segmentItem, "pau", error);

       /* append an item to the segment relation, no shared content */
       segmentItem = SRelationAppend(segmentRel, NULL, error);
       	
       /* set it's name to "pau" */
       SItemSetName(segmentItem, "pau", error);

       S_UNUSED(self);
   }

This implementation is quite simple, and does not do normal error
checking. Note the :c:macro:`S_UNUSED` macro on line 23, used to tell
the compiler that we know that the ``self`` argument is not used in
this function. We also need to implement a *dispose* function so that
the processor can be normally deleted when the voice is deleted (see
:doc:`object_system_topic`).

.. code-block:: c
   
   static void Dispose(void *obj, s_erc *error)
   {
       S_CLR_ERR(error);
       SObjectDecRef(obj);
   }

Now the class initialization can be done:

.. code-block:: c

   /* SExampleUttProc class declaration. */
   static SExampleUttProcClass ExampleUttProcClass;

   static SExampleUttProcClass ExampleUttProcClass =
   {
       /* SObjectClass */
       {
           "SUttProcessor:SExampleUttProc",
	   sizeof(SExampleUttProc),
	   { 0, 1},
	   NULL,            /* init    */
	   NULL,            /* destroy */
	   Dispose,         /* dispose */
	   NULL,            /* compare */
	   NULL,            /* print   */
	   NULL,            /* copy    */
       },
       /* SUttProcessorClass */
       NULL,                   /* initialize */
       Run                     /* run        */
   }; 

and of course ``ExampleUttProcClass`` must be registered with the
object system (see :ref:`object registering <reg_free_classes>`). Now
``SExampleUttProcClass`` can be used to instantiate an utterance
processor for a voice, which when run will add pauses to the beginning
and end of an utterance.


Feature Processors
==================

Utterance processors also make use of :c:type:`feature processors
<SFeatProcessor>`. A feature processor extracts features from
individual units (items, :c:type:`SItem`) in an utterance, these
features can then be used by the utterance processor. Feature
processors are defined in a key-value (name - processor
implementation) mapping in a voice, and are called by their names.

Implementations of feature processors normally do not add any members
or methods to the :c:type:`SFeatProcessor` object or
:c:type:`SFeatProcessorClass` class, as only the
:c:func:`SFeatProcessorRun` function is called on a feature processor.

As an example we will implement an feature processor that extracts the
end time values of segments.  Since we are not adding any methods or
members we can simply define the feature processor as:

.. code-block:: c
   
   typedef SFeatProcessor SSegEndFeatProc;

   typedef SFeatProcessorClass SSegEndFeatProcClass;


The ``run`` function of the processor (see
:c:type:`SFeatProcessorClass` structure ``run``) can be implemented as
follows:

.. code-block:: c
   :linenos:

   static SObject *Run(const SFeatProcessor *self, const SItem *item,
		       s_erc *error)
   {
	SObject *extractedFeat = NULL;
	float end = 0.0;


	S_CLR_ERR(error);

	if (item == NULL)
		return NULL;

	end = SItemGetFloat(item, "end", error);
	extractedFeat = SObjectSetFloat(end, error);

	return extractedFeat;

	S_UNUSED(self);
   }

This implementation does not include all error checking for the sake
of brevity.  We also need to implement a *dispose* function so that
the processor can be normally deleted when the voice is deleted (see
:doc:`object_system_topic`).

.. code-block:: c
   
   static void Dispose(void *obj, s_erc *error)
   {
	S_CLR_ERR(error);
	SObjectDecRef(obj);
   }


Now the class initialization can be done:

.. code-block:: c

   /* SSegEndFeatProc class declaration. */
   static SSegEndFeatProcClass SegEndFeatProcClass; 

   static SSegEndFeatProcClass SegEndFeatProcClass =
   {
	/* SObjectClass */
	{
		"SFeatProcessor:SSegEndFeatProc",
		sizeof(SSegEndFeatProc),
		{ 0, 1},
		NULL,            /* init    */
		NULL,            /* destroy */
		Dispose,         /* dispose */
		NULL,            /* compare */
		NULL,            /* print   */
		NULL,            /* copy    */
	},
	/* SFeatProcessorClass */
	Run                      /* run     */
   };

``SegEndFeatProcClass`` must be registered with the object system (see
see :ref:`object registering <reg_free_classes>`). Now
``SSegEndFeatProcClass`` can be used to instantiate a feature
processor for a voice, which when run will extract end time values
from items.
