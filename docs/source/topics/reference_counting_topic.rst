
.. index:: 
   single: Topic Guides; Reference Counting

.. _reference_counting_topic/main:

==================
Reference Counting
==================

The :doc:`object_system_topic` makes use of reference counting to know
when to delete an object.  The rule is that any object that is created
with the :c:macro:`S_NEW` and :c:macro:`S_NEW_FROM_NAME` related
macros must be deleted with a call to :c:macro:`S_DELETE`. There are a
few exceptions to this rule:

       * Containers - when a created object is added to a
       	 :doc:`container <../api/c/engine/containers/main>`, the container takes
       	 hold of the object, and the object should not be deleted. For
       	 example:

	 .. code-block:: c
	 
	    SList *list = NULL;
	    SObject *a = NULL;

	    /* Create a new list */
	    list = S_LIST(S_NEW(SListList, error));
	 
	    /* Create some object and put the into the list */
	    a = SObjectSetInt(10, error);

	    SListPush(list, a, error);

	The call to :c:func:`SListPush` adds the created object to the
	list, and the list is now responsible for the memory of the
	object. When the list is deleted with :c:macro:`S_DELETE`,
	then it will delete the object.

       * Iterators - :doc:`Container <../api/c/engine/containers/main>` iterators are created
         with calls to either :c:func:`SContainerGetIterator` or :c:func:`S_ITERATOR_GET`.
	 Iterators need not be deleted with :c:macro:`S_DELETE`, except if the iteration over
	 the container (:c:func:`SIteratorNext`) did not reach the end of the container. For
	 example:

	 .. code-block:: c
	    :linenos:
	 
	    SIterator *itr = NULL;
	    int counter = 0;
	    int loop_fin = 1;

	    itr = S_ITERATOR_GET(list, error);
	    
	    /* iterate through list objects and print them to stdout */
	    for (/* NOP */; itr != NULL; itr = SIteratorNext(itr))
	    {
	        char *buf;
		const SObject *tmp;

		tmp = SIteratorObject(itr, error);
		buf = SObjectPrint(tmp, error);
		printf("list object = %s\n", buf);
		S_FREE(buf);

		counter++;
		if (counter == 2)
		{
		    loop_fin = 0;
		    break;
		}
	    }

	    if (!loop_fin)
	        S_DELETE(itr, "main", error);

	 In this example the iteration over the container is stopped if the container has more than
	 2 objects. Therefore the ``itr`` object still has memory associated with it and must be deleted.
	
       * Heterogeneous Relation Graphs - Items are created by either adding them as daughters
         of already existing items, or appending them to already existing relations. Relations
	 are created by adding them to already existing utterances. The items and relations make
	 up the :doc:`HRG <../api/c/engine/hrg/main>` structure, and a call to the :c:macro:`S_DELETE`
	 macro on these objects will *not* delete them. A relation can be deleted with a call to
	 :c:func:`SUtteranceDelRelation`, which will delete all of the relation's items.


const-Correctness
=================

The `const-correctness
<http://en.wikipedia.org/wiki/Const-correctness/>`_ principles are
followed as far as possible. When a function's argument has a
``const`` keyword, then the function will treat the argument as
immutable. When a function's return type has a ``const`` keyword, then
the caller must treat the returned object as immutable. This is of
course a compile-time construct that indicates what a programmer
should do, not necessarily what they can do.
