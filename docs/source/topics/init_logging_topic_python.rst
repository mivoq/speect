
.. index:: 
   single: Topic Guides; Initialization, Finalization and Logging (Python API)

.. _init_logging_topic_python:

========================================
Initialization, Finalization and Logging
========================================

.. _initialization_python:

Initialization
==============

The Speect Engine is initialized when the Speect :mod:`module
<speect>` is imported (see :ref:`initialization_c` for the details of
the Speect Engine C initialization)::

    import speect

.. note::
   
   When using Speect from a built tree one must tell Python where to
   find the Speect Python bindings, see the installation
   :ref:`documentation <python_path_installation>`.


Finalization
============

There is no finalization method for Speect as the destruction of
modules and objects in modules is done in random order (in Python),
which may cause destructors to fail when they depend on other objects
(even functions) or modules.

.. _logging_python:

Logging
=======

When imported the Speect Engine :ref:`errdbg_topic_c` mechanism does
logging based on the :cmake:`CMAKE_BUILD_TYPE` definition:

     * if ``Debug`` then logging will be written to the ``stderr``
       stream, otherwise
     * no logging will be done.

If the Speect Engine error handling mechanism is active (see
:ref:`Error handling and Debugging in Python <errdbg_topic_python>`),
then a Python logger instance of the type :class:`logging.Logger` can
be given to Speect to log errors, warnings and debug messages to
(:meth:`speect.setLogger`). The Speect Engine debugging level is set
to :c:type:`S_DBG_ALL <s_dbg_lvl>` and all logging events are passed
on to the Python logger, which can filter the logging messages based
on the level. The Speect :c:type:`s_log_event` levels are converted to
Python :class:`logging` levels as in the table below:

=================  ========================            
Speect             Python
=================  ========================
``S_FATAL_EVENT``  :class:`logging.CRITICAL`
``S_ERROR_EVENT``  :class:`logging.ERROR`
``S_WARN_EVENT``   :class:`logging.WARNING`
``S_INFO_EVENT``   :class:`logging.INFO`
``S_DEBUG_EVENT``  :class:`logging.DEBUG`
``S_TRACE_EVENT``  :class:`logging.DEBUG`
=================  ========================

The logger fills the logging info dictionary with the following key-value pairs:

===========  ===============================================================
Key          Value
===========  ===============================================================
se_event     String value of the log event level (:c:func:`s_log_event_str`)
se_function  Function name where error occurred or "unknown"
se_file      File name where error occurred or "unknown" 
se_line      Line number where error occurred or "unknown" 
se_user_msg  User message (message in context setting :ref:`macros <errdbg_c_context_macros>`)
===========  ===============================================================


Example
=======

An example Python logging configuration (see Python documentation on
configuration file formats for the :meth:`logging.fileConfig` method)
is given in :ref:`Python logging configuration example
<examples/python/python_logging>` and can be used as follows:

.. code-block:: python

   import speect
   import logging
   import logging.config

   # read config file
   logging.config.fileConfig("spct_python_log.conf")

   # create the logger instance
   logger = logging.getLogger("SpeectEngine")

   # pass it to speect
   speect.setLogger(logger)
   
