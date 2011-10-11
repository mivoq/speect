.. index:: 
   single: Topic Guides (C API); Generic Object System

.. _object_system_topic:

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
superclass type. Objects keep a reference count (see :doc:`reference_counting_topic`).

See :doc:`../api/c/engine/base/objsystem/main` for a detailed description
of the API.


Base object and class
=====================

The Speect object system provides a base object, :c:type:`SObject`,
and it's class, :c:type:`SObjectClass`, from which all other objects
and classes must inherit, directly or indirectly. The structure
definitions for *SObject* and *SObjectClass* can be found in
``speect/engine/src/base/objsystem/object_def.h``. For convenience of
reference, this repeats the definitions found there. First *SObject*:

.. code-block:: c

   typedef struct
   {
	const SObjectClass *cls;
	uint32              ref;
   } SObject;
   
**cls**
	A pointer to the object's class.	

**ref**
	Reference counter.


.. _sobjectclass:

and *SObjectClass*:

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


**name**
	The name of the class. The class name is the same as the
	object name by convention, for example the *SMapClass* name is
	``SMap`` and not ``SMapClass``. The name also includes
	information about the inheritance hierarchy of the class as a
	colon (":") separated list of names of the parent classes of
	the class, excluding *SObject*, which is assumed to be the
	base class of all classes.  For example,
	:c:type:`SMapHashTable` inherits from :c:type:`SMap`,
	:c:type:`SContainer`, and :c:type:`SObject`, so the name would
	be ``SContainer:SMap:SMapHashTable``.

**size**
	The size in bytes of the of the class instance objects. The size can
	be obtained with the ``sizeof`` compile-time operator.

**ver**
	Version information of the implementation of the class, as 
	an :c:type:`s_version` type.


.. _sobjectclass_init:

**init**
	The initialization function should initialize all the object's
	members and allocate any dynamic memory resources required by
	the object's members. It can safely be assumed that any
	inherited members have already been initialized and dynamic
	memory allocated as required. The *obj* argument can be casted
	to the class object type.

.. _sobjectclass_destroy:

**destroy**
	The destroy function must free up any dynamic memory resources
	allocated by the ``init`` function. The *obj* argument can be casted
	to the class object type.

**dispose**
	Decrement the object's reference counter if the object can be
	deleted with the :c:macro:`S_DELETE` macro.The *obj* argument
	can be casted to the class object type. See
	:c:macro:`S_DELETE` for more details.

**compare**
	Compare two given objects with each other and return, either
	:c:type:`TRUE` or :c:type:`FALSE`, if the objects are equal or
	not. The given objects *will* always be the same type and
	valid. The two given objects reference must not be factored
	into the comparison. The *first* and *second* arguments can be
	casted to the class object type. The ``compare`` function
	pointer is used in the :c:func:`SObjectCompare` function.

**print**
	Print the given object to a character string buffer. The
	*self* argument can be casted to the class object type.  The
	``print`` function pointer is used in the
	:c:func:`SObjectPrint` function.

**copy**
	Copy (deep) the given object. The *self* argument can be
	casted to the class object type. Create a new object and deep
	copy all the given object's members to the new object. If the
	given object has other objects as data members they must in
	turn be copied with a call to :c:func:`SObjectCopy`. The new
	object's reference count must be 0.


.. _objsystem_example:

Defining new objects and classes
================================

To put this all into perspective we will go through an example of
defining new objects and classes and their usage. We will define a
*shape* class, with two subclasses, *rectangle* and *circle*. The
example can be found in ``speect/engine/examples/base/objsystem`` and
will be handled in detail here. Class and object definitions and
methods are defined in ".h" files and their implementations in ".c"
files, one class and related object per file, by convention.

Shape
-----

First the definition of the *shape* object:

.. code-block:: c

   typedef struct
   {  
       SObject  obj;

       int      x;
       int      y;
   } SShape;


The *shape* object inherits from :c:type:`SObject`, and *must always* have the 
object it inherits from as its first member and named *obj*. A *shape* also has
an x and y coordinate on a 2d space.

The *shape* class can be defined as follows:

.. code-block:: c

   typedef struct
   {
       SObjectClass  _inherit;

       void  (* const move)   (SShape *self, int newx, int newy, s_erc *error);
       float (* const area)   (const SShape *self, s_erc *error);
   } SShapeClass;

The *shape* class inherits from :c:type:`SObjectClass`, and *must always* have the 
class it inherits from as its first member and named *_inherit*. The *shape* class
has two methods, ``move`` and ``area``, which moves the shape in it's 2d space and
calculates the shape's area.

We also define four function prototypes:

.. code-block:: c

   void SShapeMove(SShape *self, int newx, int newy, s_erc *error);
   float SShapeArea(const SShape *self, s_erc *error);
   void _s_shape_class_reg(s_erc *error);
   void _s_shape_class_free(s_erc *error);


``SShapeMove`` and ``SShapeArea`` will handle the calling of the given *shape* object's methods
in a clean way, and ``_s_shape_class_reg`` and ``_s_shape_class_free`` will register and free the
shape object from the Speect object system. A helper macro

.. code-block:: c

   S_SHAPE(SELF)    ((SShape *)(SELF))

is defined to cast a given object to the ``SShape`` object type.


For the implementation we declare a *static* ``SShapeClass`` variable, which
will hold the shape class definition for all instances of the class:

.. code-block:: c

   static SShapeClass ShapeClass; 


and two helper macros:

.. code-block:: c

   #define S_SHAPE_CALL(SELF, FUNC) ((SShapeClass *)S_OBJECT_CLS(SELF))->FUNC

   #define S_SHAPE_METH_VALID(SELF, FUNC) S_SHAPE_CALL(SELF, FUNC) ? TRUE : FALSE


The first macro ``S_SHAPE_CALL`` is used to call a function pointer
method of the ``SShapeClass``, and the second macro
``S_SHAPE_METH_VALID`` is used to check if a desired function pointer
method has been implemented. ``SShapeMove`` can be implemented as:

.. code-block:: c
   :linenos:
   
   void SShapeMove(SShape *self, int newx, int newy, s_erc *error)
   {
       S_CLR_ERR(error);

       if (self == NULL)
       {
           S_CTX_ERR(error, S_ARGERROR,
	   	     "SShapeMove",
		     "Argument \"self\" is NULL");
           return;
       }

       if (!S_SHAPE_METH_VALID(self, move))
       {
           S_CTX_ERR(error, S_METHINVLD,
	             "SShapeMove",
		     "Shape method \"move\" not implemented");
           return;
       }


       S_SHAPE_CALL(self, move)(self, newx, newy, error);
       S_CHK_ERR(error, S_CONTERR,
       		 "SShapeMove",
		 "Call to class method \"move\" failed");
   }

Notice that there is a lot of error checking being done, which is
discussed in detail in :ref:`errdbg_topic_c`. Lines 13 and 22 contain
the interesting bits, firstly a check is done on the given ``self``
shape to see if it has implemented the ``move`` function pointer, and
if so, then the function is called with the correct signature as
defined in the ``SShapeClass`` for the ``move`` function pointer. The
``SShapeArea`` function can be implemented in the same fashion:

.. code-block:: c
   :linenos:

   float SShapeArea(const SShape *self, s_erc *error)
   {
       float area;


       S_CLR_ERR(error);

       if (self == NULL)
       {
           S_CTX_ERR(error, S_ARGERROR,
	   	     "SShapeArea",
		     "Argument \"self\" is NULL");
	   return 0.0;
       }

       if (!S_SHAPE_METH_VALID(self, area))
       {
           S_CTX_ERR(error, S_METHINVLD,
	    	     "SShapeArea",
		     "Shape method \"area\" not implemented");
	   return 0.0;
       }

       area = S_SHAPE_CALL(self, area)(self, error);
       if (S_CHK_ERR(error, S_CONTERR,
		     "SShapeArea",
		     "Call to class method \"area\" failed"))
           return 0.0;

       return area;
   }

.. _reg_free_classes:

Two functions are defined to register and free the shape class with the Speect
object system:

.. code-block:: c
   :linenos:

   void _s_shape_class_reg(s_erc *error)
   {
       S_CLR_ERR(error);
       s_class_reg(S_OBJECTCLASS(&ShapeClass), error);
       S_CHK_ERR(error, S_CONTERR,
       		 "_s_shape_class_reg",
		 "Failed to register SShapeClass");
   }


   void _s_shape_class_free(s_erc *error)
   {
       S_CLR_ERR(error);
       s_class_free(S_OBJECTCLASS(&ShapeClass), error);
       S_CHK_ERR(error, S_CONTERR,
       		 "_s_shape_class_free",
		 "Failed to free SShapeClass");
   }

with the actual registering and freeing calls on lines 4 and 14. These functions are required
because the static ``ShapeClass`` declaration has no scope outside of the implementation.
The class methods can now be defined as:

.. code-block:: c

   static void InitShape(void *obj, s_erc *error)
   {
       SShape *self = obj;


       S_CLR_ERR(error);
       self->x = 0;
       self->y = 0;
   }


   static void DisposeShape(void *obj, s_erc *error)
   {
       S_CLR_ERR(error);
       SObjectDecRef(obj);
   }


.. _shape_move_method:

.. code-block:: c

   static void MoveShape(SShape *self, int newx, int newy, s_erc *error)
   {
       S_CLR_ERR(error);
       self->x = newx;
       self->y = newy;
   }

Note that the class methods must always be declared as static. Finally we can initialize the
``ShapeClass`` class declaration:

.. code-block:: c
   :linenos:

   static SShapeClass ShapeClass =
   {
       /* SObjectClass */
       {
           "SShape",
	   sizeof(SShape),
	   { 0, 1},
	   InitShape,         /* init    */
	   NULL,              /* destroy */
	   DisposeShape,      /* dispose */
	   NULL,              /* compare */
	   NULL,              /* print   */
	   NULL,              /* copy    */
       },
       /* SShapeClass */
       MoveShape,             /* move    */
       NULL                   /* area    */
   };

Notice that the first part initializes the :ref:`SObjectClass definition <sobjectclass>`
as discussed previously, while the second part initializes the ``SShapeClass`` class definition. 
Function pointers may be defined as :c:type:`NULL`, which necessitates the use of the helper macros.

Rectangle
---------

The rectangle object is defined as:

.. code-block:: c

   typedef struct
   {
       SShape  obj;

       int     width;
       int     height;
   } SRectangle;


The rectangle object inherits from the *shape* object, and therefore also inherits the x and y coordinate
members of the shape object.

The definition of the rectangle class is:

.. code-block:: c

   typedef struct
   {
       SShapeClass  _inherit;

       void  (* const set_width)   (SRectangle *self, int new_width, s_erc *error);
       void  (* const set_height)  (SRectangle *self, int new_height, s_erc *error);
   } SRectangleClass;

The rectangle class inherits from the *shape* class, and therefore
also inherits the ``move`` and ``area`` methods. Note that there may
be situations where an object does not add any extra methods or
members to the class or object that it inherits from, and just
requires a different implementation of the methods. In these cases a
simple ``typedef`` of the parent class can be used as the definition.

We define five function prototypes:

.. code-block:: c

   SRectangle *SRectangleNew(int x, int y, int width, int height, s_erc *error);
   void SRectangleSetWidth(SRectangle *self, int new_width, s_erc *error);
   void SRectangleSetHeight(SRectangle *self, int new_height, s_erc *error);
   void _s_rectangle_class_reg(s_erc *error);
   void _s_rectangle_class_free(s_erc *error);

The definitions of ``SRectangleSetWidth`` and ``SRectangleSetHeight`` follow the style of ``SShapeMove``,
while ``_s_rectangle_class_reg`` and ``_s_rectangle_class_free`` follow the registering and freeing
functions of the shape class, and are not repeated here. To clarify the example we will first give
the implementations of the *rectangle* class methods:

.. code-block:: c
 
   static void InitRectangle(void *obj, s_erc *error)
   {
       SRectangle *self = obj;


       S_CLR_ERR(error);
       self->width = 0;
       self->height = 0;
   }


   static void DisposeRectangle(void *obj, s_erc *error)
   {
       S_CLR_ERR(error);
       SObjectDecRef(obj);
   }


   static char *PrintRectangle(const SObject *self, s_erc *error)
   {
       SRectangle *rec = S_RECTANGLE(self);
       const char *type = "[SRectangle] at (%d,%d), width %d, height %d";
       char *buf;

       S_CLR_ERR(error);

       s_asprintf(&buf, error, type, S_SHAPE(rec)->x, S_SHAPE(rec)->y, rec->width, rec->height);
       if (S_CHK_ERR(error, S_CONTERR,
       	  	     "PrintRectangle",
		     "Call to \"s_asprintf\" failed"))
       {
           if (buf != NULL)
	       S_FREE(buf);
	   return NULL;
       }

       return buf;
   }


.. _rectangle_move_method:

.. code-block:: c


   static void MoveRectangle(SShape *self, int newx, int newy, s_erc *error)
   {
       S_CLR_ERR(error);
       self->x = newx;
       self->y = newy;
   }


   static float AreaRectangle(const SShape *self, s_erc *error)
   {
       SRectangle *rec = S_RECTANGLE(self);
       float area;


       S_CLR_ERR(error);
       area = rec->width * rec->height;

       return area;
   }


   static void SetWidthRectangle(SRectangle *self, int new_width, s_erc *error)
   {
       S_CLR_ERR(error);
       self->width = new_width;
   }


   static void SetHeightRectangle(SRectangle *self, int new_heigth, s_erc *error)
   {
       S_CLR_ERR(error);
       self->height = new_heigth;
   }


.. _rectangle_class_declaration:

and the ``Rectangle`` class initialization declaration:

.. code-block:: c
   :linenos:

   static SRectangleClass RectangleClass =
   {
       {
           /* SObjectClass */
	   {
	       "SShape:SRectangle",
	       sizeof(SRectangle),
	       { 0, 1},
	       InitRectangle,     /* init    */
	       NULL,              /* destroy */
	       DisposeRectangle,  /* dispose */
	       NULL,              /* compare */
	       PrintRectangle,    /* print   */
	       NULL,              /* copy    */
	   },
	   /* SShapeClass */
	   MoveRectangle,         /* move    */
	   AreaRectangle,         /* area    */
       },
       /* SRectangleClass */
       SetWidthRectangle,         /* set_width  */
       SetHeightRectangle         /* set_height */
   };

The first part initializes the :ref:`SObjectClass definition
<sobjectclass>`, the second part initializes the ``SShapeClass`` class
definition, while the last part initializes the ``SRectangleClass`` class
definition. Note that the name of the class contains the inheritance hierarchy of
the rectangle class.

Now we can have a look at the ``SRectangleNew`` function, used to create new instances
of ``SRectangle`` objects:

.. code-block:: c
   :linenos:

   SRectangle *SRectangleNew(int x, int y, int width, int height, s_erc *error)
   {
       SRectangle *self;


       S_CLR_ERR(error);
       
       self = S_NEW(SRectangle, error);
       if (S_CHK_ERR(error, S_CONTERR,
       	  	     "SRectangleNew",
		     "Failed to create new object"))
       {
           return NULL;
       }

       S_SHAPE(self)->x = x;
       S_SHAPE(self)->y = y;
       self->width = width;
       self->height = height;

       return self;
   }

The call to :c:macro:`S_NEW` on line 8 will do two things:
    
    * Allocate a chunk of memory of the size as defined in the
      :ref:`SRectangle class declaration
      <rectangle_class_declaration>` on line 7.
    * Loop through the inheritance hierarchy of ``SRectangle`` and
      call each parent class's ``init`` method to initialize all of
      the ``SRectangle`` object's members and inherited members.

Lines 16 and 17 show how the inherited ``SShape`` members of the ``SRectangle`` object
can be accessed and manipulated.


Circle
------

The cirlce object is defined as:

.. code-block:: c

   typedef struct
   {
       SShape     obj;

       int radius;
       char *colour;
   } SCircle;

and the definition of the circle class is:

.. code-block:: c

   typedef struct
   {
       SShapeClass  _inherit;

       void  (* const set_radius)   (SCircle *self, int new_radius, s_erc *error);
       void  (* const set_colour)   (SCircle *self, const char *new_colour, s_erc *error);
   } SCircleClass;


The function prototypes are:

.. code-block:: c

   SCircle *SCircleNew(int x, int y, int radius, const char *colour, s_erc *error);
   void SCircleSetRadius(SCircle *self, int new_radius, s_erc *error);
   void SCircleSetColour(SCircle *self, const char *new_colour, s_erc *error);
   void _s_circle_class_reg(s_erc *error);
   void _s_circle_class_free(s_erc *error);


For brevity we will only give the implementations of the circle class's ``init``, ``destroy``, ``move`` and
``area`` class methods:

.. code-block:: c

   static void InitCircle(void *obj, s_erc *error)
   {
       SCircle *self = obj;


       S_CLR_ERR(error);
       self->radius = 0;
       self->colour = NULL;
   }


   static void DestroyCircle(void *obj, s_erc *error)
   {
       SCircle *self = obj;


       S_CLR_ERR(error);
       if (self->colour != NULL)
       {
           S_FREE(self->colour);
       }
   }


The circle class's ``init`` function initializes the *colour* member to :c:type:`NULL`. Note that the 
circle class has a ``destroy`` method, which the shape and rectangle classes do not have. The ``destroy``
method is used to free dynamically allocated resources, such as the *colour* member.

.. code-block:: c
   :linenos:

   static void MoveCircle(SShape *self, int newx, int newy, s_erc *error)
   {
       SShapeClass *shapeClass = NULL;


       S_CLR_ERR(error);
       shapeClass = S_FIND_CLASS(SShape, error);
       if (S_CHK_ERR(error, S_CONTERR,
       	  	     "MoveCircle",
		     "Call to \"S_FIND_CLASS\" failed"))
           return;

       shapeClass->move(self, newx, newy, error);
   }


   static float AreaCircle(const SShape *self, s_erc *error)
   {
       SCircle *cir = S_CIRCLE(self);
       float area;


       S_CLR_ERR(error);
       area = S_PI * cir->radius * cir->radius;

       return area;
   }

The rectangle class's :ref:`move method <rectangle_move_method>` was
simple in that in just reset the x and y coordinates of the shape
object, whereas the circle class's ``move`` method shows another
approach. First the class declaration of the *shape* class is looked
up with :c:macro:`S_FIND_CLASS` (line 7). Next the *shape* class's
``move`` method is called (:ref:`shape move method
<shape_move_method>`). This approach can be shorter to code if the
method implementation is the same as the parent class's method.


.. _circle_class_declaration:

Finally, the ``Circle`` class initialization declaration, which shows
the extra ``destroy`` method when compared to the 
:ref:`SRectangle class declaration <rectangle_class_declaration>`:

.. code-block:: c
   :linenos:

   static SCircleClass CircleClass =
   {
       {
           /* SObjectClass */
	   {
	       	"SShape:SCircle",
		sizeof(SCircle),
		{ 0, 1},
		InitCircle,     /* init    */
		DestroyCircle,  /* destroy */
		DisposeCircle,  /* dispose */
		NULL,           /* compare */
		PrintCircle,    /* print   */
		NULL,           /* copy    */
	    },
	    /* SShapeClass */
	    MoveCircle,         /* move    */
	    AreaCircle,         /* area    */
       },
       /* SCircleClass */
       SetRadiusCircle,        /* set_radius */
       SetColourCircle         /* set_colour */
   };


Example Usage
-------------

The following code snippets were extracted from ``speect/engine/examples/base/objsystem/objsystem_example.c``
and are abbreviated to show the basic usage of the above defined objects. The example can also be viewed at
:doc:`../examples/c/objsystem_example`.

We can now declare and instantiate circles and rectangles as follows:

.. code-block:: c

   s_erc error = S_SUCCESS;
   SCircle *circleShape = NULL;
   SRectangle *rectangleShape = NULL;
 

   /* create new circle */
   circleShape = SCircleNew(20, 62, 70, "green", &error);


   /* create new rectangle */
   rectangleShape = SRectangleNew(10, 15, 100, 140, &error);


The area of the two shapes can be calculated with the ``SShapeArea`` function, and by casting
both *rectangleShape* and *circleShape* to ``SShape`` type objects:

.. code-block:: c


   s_erc error = S_SUCCESS;
   float area = 0.0;


   area = SShapeArea(S_SHAPE(rectangleShape), &error);
   
   ...


   area = SShapeArea(S_SHAPE(circleShape), &error);


The ``SShapeArea`` function will first check the class declarations of the given objects to
see if the ``area`` method is implemented, and if so call it on the given object. The :c:func:`SObjectPrint`
can be called on the two shapes, and each will produce a different output.

   
.. code-block:: c


   s_erc error = S_SUCCESS;
   char *buf = NULL;


   buf = SObjectPrint(S_OBJECT(rectangleShape), &error);
   printf("%s\n", buf);
   
   ...

   buf = SObjectPrint(S_OBJECT(circleShape), &error);
   printf("%s\n", buf);
   
 
With output::

     [SRectangle] at (10,15), width 100, height 140
     [SCircle] at (20,62), radius 70, colour green


Note that these examples use unsafe casting, but it is possible to do
type safe casting with the :c:macro:`S_CAST` macro.
Finally the :c:macro:`S_DELETE` macro is used to delete the objects.

.. code-block:: c

   S_DELETE(rectangleShape, "main", &error);
   S_DELETE(circleShape, "main", &error);


The call to :c:macro:`S_DELETE` will do two things:
    
    * The object's ``dispose`` method is called, then
    * if the object is no longer referenced, a call is made to
      the object's ``destroy`` method.

Not all of SObjectClass's methods were implemented in these examples,
but the full details of each method can also be found at
:doc:`../api/c/engine/base/objsystem/SObjectClass_structure`.

