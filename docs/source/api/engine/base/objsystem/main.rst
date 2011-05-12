.. _objsystem/main:

=====================
Generic Object System
=====================

The object system provides basic object-oriented programming support (e.g., inheritance,
polymorphism, encapsulation, abstraction, decoupling etc.) to the Speect Engine by
using **ISO C** structures as objects and classes. 
 
Class
"""""
Classes are statically allocated and are used to create new instances of objects and to
define the interface of the object. The interface is defined with function pointers in
the class structure. Classes also contain information about the class hierarchy, the size
of the objects instantiated by the class, and version information.
 
Object
""""""
Objects are dynamically allocated by the object's class. The first member of an object
structure definition is always an object of the object's superclass type. Objects keep
a reference count.


.. c:doxygen-struct:: SDatasource

.. c:doxygen-struct:: SDatasourceClass
