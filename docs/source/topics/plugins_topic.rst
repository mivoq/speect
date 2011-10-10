
.. index:: 
   single: Topic Guides; Plug-ins

.. index:: ! Plug-ins

.. todo:: where do plug-ins go?

.. _plugins_topic:

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
all plug-ins that want to work with the plug-in manager.


plugin.c
--------

The template provides a simple function that defines the
``s_plugin_init`` symbol, this function is used unchanged by all
implemented plug-ins, as the defined symbols are created by the build
system (see :ref:`plugin_info.h <plugin_info_h>`):

.. _splugin_init_example:

.. literalinclude:: ../../../plugins/template/src/plugin.c
   :language: c
   :tab-width: 4 
   :linenos:
   :lines: 79-93

This function is called by the plug-in manager. It basically does a
version check and then returns the :c:type:`s_plugin_params` structure
for the specific plug-in to the plug-in manager. The template also
defines this structure, and it can be used unchanged in any plug-in as
the defined symbols are created by the build system (see
:ref:`plugin_info.h <plugin_info_h>`):

.. literalinclude:: ../../../plugins/template/src/plugin.c
   :language: c
   :tab-width: 4 
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
   :tab-width: 4
   :linenos:
   :lines: 102-131



source.c and source.h
---------------------

The template provides skeleton source and header files which can be
used to implement a plug-in. Each implemented class has registration
and freeing functions in the following format:

.. literalinclude:: ../../../plugins/template/src/source.c
   :language: c
   :tab-width: 4
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

:c:func:`s_pm_load_plugin` will load the DSO named "shape.spi" (Speect
uses the ".spi" extension for it's plug-ins), look up the
``s_plugin_init`` symbol which includes the shape class's registering
function, execute it, and set the DSO and related information in the
:c:type:`SPlugin` object. 

.. note::

   If the given plug-in path does not include any path separators
   (just a file name) then the path is concatenated with the
   :ref:`default plug-in path <default_plugin_path>`.


The same rules for of order of registration
holds, in other words we must first load the shape plug-in **before**
registering the circle class.  And then in the
``plugin_exit_function`` we can delete the shape plug-in:

.. code-block:: c
   
   static void plugin_exit_function(s_erc *error)
   {
	S_CLR_ERR(error);

	S_DELETE(shapePlugin, "plugin_exit_function", error);

   ...

We have to include the shape header in our source code if we want to
use any shape defined members or methods. We don't need to include the
whole path to the specific location of the shape plug-in
implementation as the :ref:`build system <source_include>` will take care
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
   :language: cmake
   :lines: 14-26

Any additional sources that make up part of the plug-in must be included here.


CMakeLists.txt
--------------

``CMakeLists.txt`` is the top-level CMake specification file for the
plug-in. The following describes the CMake commands executed to create
a plug-in project:


.. rubric:: Define plug-in

First a plug-in definition is required:

.. literalinclude:: ../../../plugins/template/CMakeLists.txt
   :language: cmake
   :lines: 17

The plug-in is defined by the :cmake_macro:`speect_plugin_definition`
macro, which does the following:

    * check if a plug-in with the same name, ``Plugin_Name``, already
      exists (all plug-ins must have unique names),
    * create a CMake project for the plug-in,
    * create DSO names for the plug-in, "lowercase_name.spi" with
      links to
      - "lowercase_name.spi.version_major" and
      - "lowercase_name.spi.version_major.version_minor.version_patch"
    * and create a class name for the plug-in which is used in the
      defined symbols in ``plugin_info.h``.

For example:

.. code-block:: cmake
   
   speect_plugin_definition(Viterbi "SViterbi" 0 9 5)

will create a plug-in DSO named "viterbi.spi" that points "to
viterbi.spi.0" which in turn points to "viterbi.spi.0.9.5". 

.. note::
   The plug-in's name (``Plugin_Name``) will be lower cased, and can be
   used to reference the plug-in in other plug-ins projects. See for
   example the :ref:`utterance processors <voice_utt_proc>`
   definitions in the voice file where an utterance processor plug-in
   is reference by it's name.

.. note::

   All plug-in DSO's are place in the ...

.. _plugin_info_h:

.. rubric:: Configure plugin_info.h

All plug-ins must include the ``plugin_info.h`` header file in the
``plugin.c`` source file. This header file is generated by the build
system based on information in ``CMakeLists.txt``. The configuration
is as follows:

.. literalinclude:: ../../../plugins/template/CMakeLists.txt
   :language: cmake
   :lines: 24-30

A description, and version numbers are set, and then the
:cmake_macro:`speect_plugin_configure_info` is called to generate
``plugin_info.h``, which defines the following C pre-processor
variables:

	* ``SPCT_PLUGIN_NAME``: the plug-in name, which is the class
          name created by :cmake_macro:`speect_plugin_definition`,
	* ``SPCT_PLUGIN_DESCRIPTION``: the description that was set,
	* ``SPCT_PLUGIN_INIT_STR`` : the initialization string used in
          :ref:`s_plugin_init <splugin_init_example>`,
	* the ``SPCT_PLUGIN_REG_STR``, ``SPCT_PLUGIN_REG_FAIL_STR``,
	  ``SPCT_PLUGIN_EXIT_STR`` and ``SPCT_PLUGIN_EXIT_FAIL_STR``
	  strings used in the plug-in register and exit
	  :ref:`functions <class_register_exit>`,
	* ``SPCT_PLUGIN_VERSION_MAJOR`` and
          ``SPCT_PLUGIN_VERSION_MINOR`` version variables, defined by
          :cmake_macro:`speect_plugin_definition`,
	* and the minimum required Speect Engine version variables
          ``SPCT_MAJOR_VERSION_MIN`` and ``SPCT_MINOR_VERSION_MIN``.

The strings variables are defined just to make the definitions of
:ref:`s_plugin_init <splugin_init_example>` and the plug-in register
and exit :ref:`functions <class_register_exit>` less tedious when
implementing a lot of plug-ins, but they can of course be ignored are
replaced in those functions with something more appropriate if
desired. The version variables are used in a version handshaking
function between the plug-in and the Speect Engine in the plug-in
manager.


.. _source_include:

.. rubric:: Source files and includes

The ``sources.cmake`` file that defines the list of source and header files of the plug-in
is added to the plug-in project:

.. literalinclude:: ../../../plugins/template/CMakeLists.txt
   :language: cmake
   :lines: 38


Other plug-in's projects that are used in this plug-in must also be included
(their correct paths will be determined by the build system), for example:

.. code-block:: cmake

       include(shape)

This will allow us to use their headers in this plug-in. The name is
the lower cased plug-in project name, the same way it is defined in
``speect_plugin_definition``. A list of plug-in project configurations
that can be included are in ``speect/build/plugins/cmakeconf``.


.. rubric:: Plug-in DSO

and finally we create the plug-in DSO with the
:cmake_func:`speect_plugin_create` function:


.. literalinclude:: ../../../plugins/template/CMakeLists.txt
   :language: cmake
   :lines: 45

This function will create the plug-in shared object, which will be linked
against the Speect Engine library. The build options (``Debug``, etc.)
will the same as for the Speect Engine library. Optional parameters,
which are libraries to link with, can be given in a list form.

The full ``CMakeLists.txt`` CMake project file is given in the
:ref:`examples <examples/misc/cmakelists_template>`.
