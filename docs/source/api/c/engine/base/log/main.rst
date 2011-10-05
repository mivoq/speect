.. index:: ! Logger (C API)

.. _log/main:

======
Logger
======

The logging system provides a very basic mechanism to write messages
to different types of streams. It is used by all the error and debug
reporting facilities in :doc:`../errdbg/main`, and therefore aims to
be as robust as possible. If any error occurs during one of the logger
functions, an error will be printed to ``stderr``. If a logger is
unsuccessful at writing the log message to the log stream then it will
log the message to ``stdout``. Also see
:doc:`../../../../../topics/init_logging_topic`.


.. todo::
   
   add reference for main function

.. todo::

   add reference to doxygen 


Summary
=======

.. doxybridge-autosummary::
   :nosignatures:
   
   s_logger_file_new
   s_logger_console_new
   s_logger_null_new
   s_logger_write
   s_logger_vwrite
   s_logger_destroy
   s_log_event_str
   


Definition
==========

.. doxybridge:: s_logger
   :type: typedef struct
   :members: none
   	    
.. toctree::
   :hidden: 

   s_logger_structure

:doc:`s_logger_structure`


Logger Creators
===============

Three loggers are provided, one logging to file, one to a console and
a null logger. The loggers must be freed by first calling
:c:func:`s_logger_destroy` to free any private data of the logger, and
then a call to :c:macro:`S_FREE` to free the logger itself.

.. doxybridge:: s_logger_file_new

.. doxybridge:: s_logger_console_new

.. doxybridge:: s_logger_null_new


Standard Layout Format
======================

The standard layout for the file and console loggers define the format of the printed
messages. There are two different formats, one for error messages and one for debugging/warning
messages. 

The error messages layout is as follows:
    
    *Date & Time* [*Logging event level*\ :sup:`1`\  (*Error message*\ :sup:`2`\)  *Thread id*\ :sup:`3`\]  *User message* (in function '*Function name*\ :sup:`4`\', *File name*\ :sup:`4`\, *File line number*\ :sup:`4`\)

where:
	#. Derived automatically from :c:type:`s_log_event`
	#. Derived automatically from :c:type:`s_erc`
	#. Only if compiled with threads
	#. Optional


For example, the following will be printed with all the above information available::
    
    Wed 18 May 2011 11:58:41 SAST [ERROR (Memory error) 10] Failed to allocate object memory (in function 'func', /home/nobody/test.c, 121)


The debugging/warning messages layout is as follows:

    *Date & Time* [*Debugging/warning message type*\ :sup:`1`\   *Thread id*\ :sup:`2`\]  *User message*

where:
	#. Derived automatically from :c:type:`s_log_event` and :c:type:`s_dbg_lvl`
	#. Only if compiled with threads

For example, the following will be printed with all the above information available::
    
    Wed 18 May 2011 11:58:41 SAST [TRACE 10] value = 20.321


Logging Functions
=================

.. doxybridge:: s_logger_write


.. doxybridge:: s_logger_vwrite


Logger Destroyer
================

.. doxybridge:: s_logger_destroy


Logging Events
==============

Logging event definitions. The log events defines the type of information
that will be output to the log.

.. doxybridge:: s_log_event
   :type: typedef enum

.. doxybridge:: s_log_event_str
