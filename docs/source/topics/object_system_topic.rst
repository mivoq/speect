.. _object_system_topic/main:

.. index:: 
   single: Topic Guides (C API); Generic Object System

=====================
Generic Object System
=====================

The Speect object system provides basic object-oriented programming
support by using **ISO C** structures as *classes* and *objects*. The
following object-oriented programming concepts are supported:

	  * **Class**: Defines the characteristics of an object's
	    behavior, or *methods*.
	  * **Object**: A specific instance of a class, created at
	    runtime. Defines the specific characteristics, or
	    *members*, of an instance of the class, which may differ
	    between different instances of the same class.
	  * **Single Inheritance**: "Subclasses" are more specialized
	    versions of a class, which inherit methods and members
	    from their parent classes, and can introduce their
	    own. The Speect object system supports only single
	    inheritance, therefore a class can only inherit from one
	    parent class. For subclasses the parent class methods are
	    *pure virtual functions* and must be implemented by the
	    subclass, if they are required.
	  * **Abstraction**: Modelling classes appropriate to a specific
	    problem, and working at the most appropriate level of
	    inheritance for a given aspect of the problem. For
	    example, an :c:type:`SList` may be treated as a
	    :c:type:`SContainer` when necessary to access container
	    specific members or methods.
	  * **Polymorphism**: The ability of objects belonging to
	    different data types to respond to method calls of methods
	    of the same name, each one according to an appropriate
	    type-specific behavior. For example, the function
	    :c:func:`SObjectPrint` can be implemented to elicit a
	    different behaviour from objects belonging to different
	    classes.
     

Classes are statically allocated and are used to create new instances
of objects and to define the methods of the object. The methods are
defined with function pointers in the class structure. Classes also
contain information about the class hierarchy, the size of the objects
instantiated by the class, and version information.  Objects are
dynamically allocated by the object's class. The first member of an
object structure definition is always an object of the object's
superclass type. Objects keep a reference count.


SObject
=======

The Speect object system provides a base object, :c:type:`SObject`,
and it's class, :c:type:`SObjectClass`, from which all other objects
and classes must inherit, directly or indirectly. The *SObject*
structure definition is as follows:

.. code-block:: c

   typedef struct
   {
	const SObjectClass *cls;
	uint32              ref;
   } SObject;
   
The *SObject* structure is very simple, with a pointer to it's class
and a reference counter. 


SObjectClass
============

The *SObjectClass* structure definition is as follows:

.. code-block:: c

   typedef struct 
   {
	const char *name;
	size_t      size;
	s_version   ver;

	void     (* const init)    (void *obj, s_erc *error);
	void     (* const destroy) (void *obj, s_erc *error);
	void     (* const dispose) (void *obj, s_erc *error);
	s_bool   (* const compare) (const SObject *first, const SObject *second, s_erc *error);
	char    *(* const print)   (const SObject *self, s_erc *error);
	SObject *(* const copy)    (const SObject *self, s_erc *error);
   } SObjectClass;

The first three entries define information relating to the class instance objects, and the rest define
the class methods that are common for all classes.

Creating and Destroying Objects
===============================

All objects can be created with a call to the :c:macro:`S_NEW` macro, which takes as arguments a string name
of the desired object to be created and an error code pointer. The returned value is always of type :c:type:`SObject`
and can be safely casted




The first entry is the class name. The name is a colon (":") separated
list of names which reflect the inheritance hierarchy of the class,
excluding *SObject*. For example, :c:type:`SMapHashTable` inherits from
:c:type:`SMap`, :c:type:`SContainer`, and :c:type:`SObject`, so the name
would be "SContainer:SMap:SMapHashTable".

The next entry is the size in bytes of the class instance objects. For example::
    
    sizeof(SMapHashTable)


Let us illustrate the object system with an example. Lets say we have a **Shape** class for 2 dimensional
shapes. Shapes can be moved in the x and y coordinates and one can calculate a shape's area. The following
code snippet defines the interface for the shape class:

.. literalinclude:: ../../../engine/examples/base/objsystem/shape.h
   :language: c
   :linenos:
   :lines: 127-158

The class is just a normal C structure, with the class inheritance on line 7, and two class methods, *move*
and *area* on lines 19 and 30. The class methods are defined as function pointers in the class structure. The
class methods take as first argument an *SShape* object, which by convention is called *self*. The *SShape* object
is also a normal C structure, and is defined as follows:

.. literalinclude:: ../../../engine/examples/base/objsystem/shape.h
   :language: c
   :linenos:
   :lines: 108-114

