.. _python_engine_sitem:

=====
SItem
=====

This section describes the :class:`SItem` class.

.. currentmodule:: speect

.. autoclass:: SItem


.. autosummary::
   
   SItem.relation
   SItem.utterance
   SItem.voice
   SItem.append
   SItem.prepend
   SItem.add_daughter
   SItem.next
   SItem.prev
   SItem.parent
   SItem.daughter
   SItem.as_relation
   SItem.in_relation
   SItem.__eq__
   SItem.path_to_item
   SItem.path_to_feature
   SItem.path_to_derived_feature
   SItem.__len__
   SItem.__contains__
   SItem.__setitem__
   SItem.__getitem__
   SItem.__delitem__
   SItem.__iter__
   SItem.__str__
   SItem.feature_get


.. note::

   The ``SItem`` object inherits from ``SObject`` in the Speect Engine
   and can therefore be used in functions/methods that require parameters
   of type ``SObject``.


Information
-----------
Methods relating to item information.

.. automethod:: SItem.relation()

.. seealso::
   :class:`SRelation`


.. automethod:: SItem.utterance()

.. seealso::
   :class:`SUtterance`

.. automethod:: SItem.voice()

.. seealso::
   :class:`SVoice`


Item Creation
-------------
Methods relating to the creation of items.

.. automethod:: SItem.append()

.. automethod:: SItem.prepend()

.. automethod:: SItem.add_daughter()


Item Traversing
---------------
Methods relating to the traversing of items.


List Traversal
^^^^^^^^^^^^^^

.. automethod:: SItem.next()

.. automethod:: SItem.prev()


Tree Traversal
^^^^^^^^^^^^^^

.. automethod:: SItem.parent()

.. automethod:: SItem.daughter()


Queries
-------
Methods relating to items queries.


.. automethod:: SItem.as_relation()

.. automethod:: SItem.in_relation()

.. seealso::
   :class:`SRelation`

.. automethod:: SItem.__eq__()

for example::
    
    if (item_a == item_b):
        print "true"
    else:
	print "false"


Paths
-----
Methods relating to items paths.


.. automethod:: SItem.path_to_item()

.. automethod:: SItem.path_to_feature()

.. automethod:: SItem.path_to_derived_feature()


Features
--------
Methods relating to items features.

.. automethod:: SItem.__len__()

Get the number of features in an item, for example::

    len(item)


.. automethod:: SItem.__contains__()

Query if item contains a feature, for example::

    feature_name = "part_of_speech"
    if feature_name in item:
        print "true"
    else:
	print "false"

    
.. automethod:: SItem.__setitem__()

Set an item feature, for example::

    item["name"] = "pau"

.. automethod:: SItem.__getitem__()

Get an item feature, for example::

    name = item["name"]

.. note::
   
    The Speect SWIG/Python implementation tries to convert the returned Speect object
    to a Python object through the use of ``typemaps``. These ``typemaps`` can also be
    extended for new object types through plug-ins. Sometimes we want access to the
    pure Speect object, for example in methods which require an ``SObject`` type
    such as :meth:`SVoice.data_set`, to get the ``SObject`` value we can call 
    :meth:`SItem.feature_get`.

.. seealso::
    :meth:`SMap.value_get`

.. automethod:: SItem.__delitem__()

Delete an item feature, for example::

    del item["name"]

.. automethod:: SItem.feature_get()

Get the raw Speect ``SObject`` item feature::

    sobject = item.feature_get("name")

.. seealso::
   :meth:`SItem.__getitem__`


Iteration
^^^^^^^^^
.. automethod:: SItem.__iter__()

Iterating over an item's feature keys, for example::

    for feat_names in item:
    	print item[feat_names]



Printing
--------

.. automethod:: SItem.__str__()

for example::
    
    print item



