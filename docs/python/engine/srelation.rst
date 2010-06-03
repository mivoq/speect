.. _python_engine_srelation:

=========
SRelation
=========

This section describes the :class:`SRelation` class.


.. currentmodule:: speect


.. autoclass:: SRelation

.. autosummary::
   
   SRelation.name
   SRelation.utterance
   SRelation.append
   SRelation.prepend
   SRelation.head
   SRelation.tail
   SRelation.__iter__
   SRelation.__str__


.. note::

   The ``SRelation`` object inherits from ``SObject`` in the Speect Engine
   and can therefore be used in functions/methods that require parameters
   of type ``SObject``.


Information
-----------
Methods relating to relation information.

.. automethod:: SRelation.name()

.. automethod:: SRelation.utterance()

.. seealso::
   :class:`SUtterance`


Item Creation
-------------
Methods relating to the creation of items in a relation.

.. automethod:: SRelation.append()

.. automethod:: SRelation.prepend()


.. seealso::
   :class:`SItem`


Item Traversing
---------------
Methods relating to the traversing of items in a relation.

.. automethod:: SRelation.head()

.. automethod:: SRelation.tail()

.. seealso::
   :class:`SItem`


Iteration
^^^^^^^^^

.. automethod:: SRelation.__iter__()

Called for iteration over items in a relation, for example::
    
    for items in relation:
    	print items

.. seealso::
   :class:`SItem`


Printing
--------

.. automethod:: SRelation.__str__()

for example::
    
    print relation



