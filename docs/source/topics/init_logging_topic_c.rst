
.. index:: 
   single: Topic Guides; Initialization, Finalization and Logging (C API)

.. _init_logging_topic_c:

========================================
Initialization, Finalization and Logging
========================================

.. _initialization_c:

Initialization
==============

The Speect engine must be initialized before any calls are made to any
Speect functions. Most functionality of the Speect engine can only be
used after it has been initialized. The basic initialization function
is :c:func:`speect_init`. This initializes the logging mechanism (see
:ref:`logging <logging>` below), the :ref:`error handling module
<errdbg_topic_c>`, and the :ref:`object system <object_system_topic>`.
The *default* plug-in path is also set.


.. index:: Default Plug-in Path

.. _default_plugin_path:

.. rubric:: *Default* plug-in path

The default plug-in path is calculated based on a guess of the
location of the Speect installation, or where Speect was built. When
Speect is built, a file ``plugin.lib`` is created in the
``speect/build/plugins/lib/`` directory, and when Speect is installed
that file is created in the
:cmake:`CMAKE_INSTALL_PREFIX`/speect/plugins directory. During
initialization the ``plugin.lib`` file is looked for in these two
locations, with the installation location taking preference.

The user can override this behavior by setting the
environment variable 

.. envvar:: SPCT_PLUGIN_PATH 
   
   Environment variable pointing to the Speect plug-ins path.

which will take preference over the installation and build locations.

Finalization
============

One should finalize Speect after using it. Finalizing Speect will close the *logging system*,
free up resources taken by the *object system* and quit the *error handling module*.

The finalization function is :c:func:`speect_quit`.


.. _logging:

Logging
=======

The :c:func:`speect_init` function takes a logger (:c:type:`s_logger`)
as an argument. If :c:type:`NULL` then :ref:`errdbg_topic_c`
logging will be set as follows:

   * if :cmake:`CMAKE_BUILD_TYPE` is set to ``Debug`` then logging
     will be written to the ``stderr`` stream, otherwise
   * no logging will be done.

A file or console logger can be created with the functions
:c:func:`s_logger_file_new` and :c:func:`s_logger_console_new`, which
can then be passed to :c:func:`speect_init`. All logging of the
:ref:`error handling module <errdbg_topic_c>` will then be done to the
passed logger. The function pointers of :c:type:`s_logger` can also be
used to create callbacks to other loggers, as in the Python logger in
``speect/engine/swig/python/native/py_logger.c``. 

.. seealso::

   :ref:`Disabling error handling and debugging <errdbg_topic_disable>`.
