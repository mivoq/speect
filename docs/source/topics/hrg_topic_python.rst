.. _hrg_topic_python:

.. index:: 
   single: Heterogeneous Relation Graphs; Python example

=======================
HRG with the Python API
=======================

The code to recreate the HRG structure in :ref:`figure 4 <figure_hrg>`
can be found at ``speect/swig/python/examples/hrg_docs_example.py``,
or :doc:`HRG example <../examples/python/hrg_example>`. See
:doc:`HRG API <../api/python/engine/hrg/main>` for a detailed description
of the API. Here we show some snippets to explain the process of
building an utterance.

An utterance is created by a call to :meth:`speect.SUtterance()`:

.. code-block:: py

   import speect

   utt = speect.SUtterance()


A relation is created from the utterance with :meth:`speect.SUtterance.relation_new()`:

.. code-block:: py

   # create a new "Word" relation
   word_rel = utt.relation_new("Word")


and items can be created by appending them to a relation with :meth:`speect.SRelation.append()`:

.. code-block:: py

   # add word in word relation
   word_item = word_rel.append()

   word_item["name"] = "twenty"


No shared content argument was passed in :meth:`speect.SRelation.append()`.
Shared content is used when one wants to add an item to more than one relation, and share the content of the
items. For example, we might have a token relation, with the first
token being "21". In the word relation we will have two words,
"twenty" and "one", both of which can be daughters (as in :ref:`figure
4 <figure_hrg>`, but with the token relation one level higher than the
word relation) of the "21" token item. To create the shared items we
can code it as follows:


.. code-block:: py
   :linenos:

   item21 = token_rel.append()
   item21["name"] = "21"
   
   item_twenty = word_rel.append()
   item_twenty["name"] = "twenty"

   item_one = word_rel.append()
   item_one["name"] = "one"

   item21.add_daughter(item_twenty)
   item21.add_daughter(item_twenty)


:ref:`Figure 5 <figure_hrg_sharedp>` shows a representation of the HRG that
can be created by following the above code example. Note that ``item21`` is now
the parent item (gotten with :meth:`speec.SItem.parent()` of both ``itemTwenty`` and ``itemOne``).

|

.. _figure_hrg_sharedp:

.. figure:: ../../figures/hrg.shared.png
   :width: 55%
   :alt: An example of the HRG representation of parent/daughter items.
   :align: center

   Figure 5: An example of the HRG representation of parent/daughter items. 

|

