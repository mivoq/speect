.. _plugins_topic:

.. index:: 
   single: Topic Guides; Plug-ins

.. index:: ! Plug-ins

========
Plug-ins
========

Plug-ins are used to extend the functionality of Speect, and as a
mechanism to keep the Speect Engine independent of any linguistic or
acoustic specific code. This extending of Speect is done by defining
new classes (see :doc:`object_system_topic`) in the plug-ins and
registering them with the object system, so that objects of these new
classes can be instantiated and used by other code. See
:doc:`../api/c/engine/pluginmanager/main` for a detailed description of
the API.

In Speect each plug-in is in it's own directory. A template of this
plug-in directory format is provided in ``speect/plugins/template``::

    speect/plugins/template/
    ├── AUTHORS
    ├── cmake
    │   └── sources.cmake
    ├── CMakeLists.txt
    ├── README
    └── src
        ├── plugin.c
        ├── source.c
	└── source.h

Below follows a discussion of defining a plug-in with regards to this template.


Dynamic Libraries
=================

Plug-ins are deployed as dynamic libraries. Dynamic libraries allow
many of the advantages of plug-ins such as hot swapping (reloading a
new implementation without shutting the system), safe extension by
third-party developers (additional functionality without modifying the
core system), and shorter link times.

Speect provides support for the use of :doc:`dynamic shared objects
<../api/c/engine/pluginmanager/dso>` (DSO) as plug-ins. When a DSO, or
plug-in, is loaded the *plug-in manager* looks for the
``s_plugin_init`` symbol in the plug-in. This is the *only* symbol
that is loaded by the plug-in manager and therefore must be defined by
all plug-ins.


plugin.c
--------

The template provides a simple function that defines the
``s_plugin_init`` symbol, this function is used unchanged by all
implemented plug-ins, as the defined symbols are created by the build
system:


.. literalinclude:: ../../../plugins/template/src/plugin.c
   :language: c
   :linenos:
   :lines: 79-93

This function is called by the plug-in manager. It basically does a
version check and then returns the :c:type:`s_plugin_params` structure
for the specific plug-in to the plug-in manager. The template also
defines this structure, and it can be used unchanged in any plug-in as
the defined symbols are created by the build system:

.. literalinclude:: ../../../plugins/template/src/plugin.c
   :language: c
   :linenos:
   :lines: 45-70

The two important members in this structure are on lines 22
and 25. These are the register and exit functions (with signatures as
defined in :c:type:`s_plugin_reg_fp` and :c:type:`s_plugin_exit_fp`)
that the plug-in manager calls on registering the plug-in and freeing
it. In these two functions one must register and free the plug-in's
provided classes with the Speect object system (:c:func:`s_class_reg`
and :c:func:`s_class_free`), see for example the shape class
:ref:`registering and freeing <reg_free_classes>` functions. The
template provides a skeleton implementation of these two functions
which should be completed with the correct class registration and
freeing for your plug-in:

.. _class_register_exit:

.. literalinclude:: ../../../plugins/template/src/plugin.c
   :language: c
   :linenos:
   :lines: 102-131



source.c and source.h
---------------------

The template provides skeleton source and header files which can be
used to implement a plug-in. Each implemented class has registration
and freeing functions in the following format:

.. literalinclude:: ../../../plugins/template/src/source.c
   :language: c
   :linenos:
   :lines: 46-63

These functions are used in the plug-in registration and exit
:ref:`functions <class_register_exit>`.


Order of registration
---------------------

Lets say we implemented the shape, circle and rectangle classes as
discussed in the *Generic Object System* :ref:`example
<objsystem_example>`.  Now the order of registering and freeing
classes in the ``plugin_register_function`` and
``plugin_exit_function`` becomes very important because of the way the
object system class registration functions work.

When a class is registered with :c:func:`s_class_reg`, the object
system will create a structure containing the class's inheritance
hierarchy of :ref:`initialization <sobjectclass_init>` and
:ref:`destroy <sobjectclass_destroy>` functions. The circle class
*inherits* from the shape class, and therefore must be registered
**after** the shape class, otherwise the object system cannot find the
unregistered shape class's ``init`` and ``destroy`` functions.

This restriction on the order of registering classes does not hold
when freeing classes in the ````plugin_exit_function`` with the
:c:func:`s_class_free` function.


Using other plug-ins
--------------------

Lets say we implemented the shape, circle and rectangle classes as
plug-ins, each in it's own directory. The circle class inherits from
the shape class and therefore requires it's plug-in. This can be done
by first loading the shape plug-in in the ``plugin_register_function``
of the circle plug-in. We create a *static* plug-in variable to hold the 
shape plug-in and then load the plug-in.

.. code-block:: c
   
   static SPlugin *shapePlugin = NULL;


   static void plugin_register_function(s_erc *error)
   {
	S_CLR_ERR(error);
	
	shapePlugin = s_pm_load_plugin("shape.spi", error);

   ...

The same rules for of order of registration holds, in other words we
must first load the shape plug-in **before** registering the circle class.
And then in the ``plugin_exit_function`` we can delete the shape plug-in:

.. code-block:: c
   
   static void plugin_exit_function(s_erc *error)
   {
	S_CLR_ERR(error);

	S_DELETE(shapePlugin, "plugin_exit_function", error);

   ...

We have to include the shape header in our source code if we want to
use any shape defined members or methods. We don't need to include the
whole path to the specific location of the shape plug-in
implementation as the :ref:`plugins_build_system` will take care
thereof. 


.. _plugins_build_system:

Build System
============

The following sections describe the build system files of a plug-in, again in reference
to the template plug-in.

sources.cmake
-------------

The source files of the plug-in needs to be specified in
``speect/template/cmake/sources.cmake``:

.. literalinclude:: ../../../plugins/template/cmake/sources.cmake
   :language: c
   :linenos:
   :lines: 14-26

Any additional sources that make up part of the plug-in must be included here.


CMakeLists.txt
--------------

``CMakeLists.txt`` is the top-level CMake specification file for the plug-in. First a plug-in
definition is required:

.. literalinclude:: ../../../plugins/template/CMakeLists.txt
   :language: c
   :linenos:
   :lines: 17

The :cmake_macro:`speect_plugin_definition` macro 


Define a plug-in. The plug-in name is lowercased and the project name is set to “speect_lowercase_name_plugin” with the C language. The plug-in library’s name will be “lowercase_name.spi” with links to:

        “lowercase_name.spi.version_major”
        “lowercase_name.spi.version_major.version_minor.version_patch”

The class name is used in the documentation strings (for example “SArrayFloat plug-in initialization”). The plug-in’s “cmake” directory will be added to CMAKE_MODULE_PATH. Defined in speect/plugins/cmake/pluginFunctions.cmake.
