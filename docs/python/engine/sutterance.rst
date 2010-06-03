.. _python_engine_sutterance:

==========
SUtterance
==========

This section describes the :class:`SUtterance` class.


.. currentmodule:: speect


.. autoclass:: SUtterance
 
.. autosummary::
   
   SUtterance.features
   SUtterance.voice
   SUtterance.relation_new
   SUtterance.relation_get
   SUtterance.relation_set
   SUtterance.relation_del
   SUtterance.__contains__
   SUtterance.__iter__
   SUtterance.__str__

.. note::

   The utterance is deleted when the ``SUtterance`` object is deleted.

.. note::

   The ``SUtterance`` object inherits from ``SObject`` in the Speect Engine
   and can therefore be used in functions/methods that require parameters
   of type ``SObject``.


Attributes
----------
Utterance attributes.

.. autoattribute:: SUtterance.features

.. seealso::
   :class:`SMap`

.. autoattribute:: SUtterance.voice

.. seealso::
   :class:`SVoice`


Relations
---------
Methods relating to the utterance's relations.

.. automethod:: SUtterance.relation_new()

.. automethod:: SUtterance.relation_get()

.. automethod:: SUtterance.relation_set()

.. automethod:: SUtterance.relation_del()

.. seealso::
   :class:`SRelation`

Membership
^^^^^^^^^^

.. automethod:: SUtterance.__contains__()

Called to implement relation membership tests, for example::
    
    relation_name = 'Syllable'
    if relation_name in utterance:
        print 'true'
    else:
	print 'false'

.. seealso::
   :class:`SRelation`


Iteration
---------

.. automethod:: SUtterance.__iter__()

Called for iteration over relation names in an utterance, for example::
    
    for relations_names in utterance:
    	print relation_names

.. seealso::
   :class:`SRelation`

Printing
--------

.. automethod:: SUtterance.__str__()

for example::
    
    print utterance



