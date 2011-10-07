.. _hrg_topic_c:

.. index:: 
   single: Heterogeneous Relation Graphs; C example

==================
HRG with the C API
==================

The code to recreate the HRG structure in :ref:`figure 4 <figure_hrg>`
can be found at ``speect/engine/examples/hrg/create_docs_example.c``,
or :doc:`HRG example <../examples/c/hrg_example>`. See
:doc:`HRG API <../api/c/engine/hrg/main>` for a detailed description
of the API. Here we show some snippets to explain the process of
building an utterance.

An utterance is created by a call to :c:macro:`S_NEW`, after which it must
be initialized with :c:func:`SUtteranceInit`. For example:

.. code-block:: c

   SUtterance *utt;
   
   
   utt = S_NEW(SUtterance, &error);
   SUtteranceInit(&utt, NULL, &error);


The :c:func:`SUtteranceInit` function takes the voice as an argument,
but in the above example we have used :c:type:`NULL`, which is fine if
one does not need the utterance's voice (:c:func:`SVoiceSynthUtt` is
used if one has a voice and wants to synthesize an utterance.)

A relation is created from the utterance with :c:func:`SUtteranceNewRelation`:

.. code-block:: c

   SRelation *wordRel;


   wordRel = SUtteranceNewRelation(utt, "Word", &error);


and items can be created by appending them to a relation with :c:func:`SRelationAppend`:

.. code-block:: c

   SItem *wordItem;


   wordItem = SRelationAppend(wordRel, NULL, &error);	
   SItemSetName(wordItem, "twenty", &error);


The :c:type:`NULL` argument passed in :c:func:`SRelationAppend` is for
an item's shared content (also used in :c:func:`SItemAppend`,
:c:func:`SItemPrepend`, :c:func:`SItemAddDaughter` and
:c:func:`SRelationPrepend`). Shared content is used when one wants to
add an item to more than one relation, and share the content of the
items. For example, we might have a token relation, with the first
token being "21". In the word relation we will have two words,
"twenty" and "one", both of which can be daughters (as in :ref:`figure
4 <figure_hrg>`, but with the token relation one level higher than the
word relation) of the "21" token item. To create the shared items we
can code it as follows:


.. code-block:: c
   :linenos:

   SItem *item21;
   SItem *itemTwenty;
   SItem *itemOne;


   item21 = SRelationAppend(tokenRel, NULL, &error);	
   SItemSetName(item21, "21", &error);

   itemTwenty = SRelationAppend(wordRel, NULL, &error);	
   SItemSetName(itemTwenty, "twenty", &error);

   itemOne = SRelationAppend(wordRel, NULL, &error);	
   SItemSetName(itemOne, "one", &error);

   SItemAddDaughter(item21, itemTwenty, &error);
   SItemAddDaughter(item21, itemOne, &error);


:ref:`Figure 5 <figure_hrg_shared>` shows a representation of the HRG that
can be created by following the above code example. Note that ``item21`` is now
the parent item (gotten with :c:func:`SItemParent` of both ``itemTwenty`` and ``itemOne``).

|

.. _figure_hrg_shared:

.. figure:: ../../figures/hrg.shared.png
   :width: 55%
   :alt: An example of the HRG representation of parent/daughter items. 
   :align: center

   Figure 5: An example of the HRG representation of parent/daughter items. 

|

