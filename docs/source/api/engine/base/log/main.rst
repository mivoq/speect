.. index:: ! Logger (C API)

.. _log/main:

======
Logger
======

.. toctree::
   :hidden: 

   s_logger_detail


.. todo::
   
   add reference for main function

.. todo::

   add reference to doxygen 

Definition
==========

.. doxybridge:: s_logger
   :type: typedef struct
   :members: none
   	    
   
.. seealso:: :doc:`s_logger_detail`.


Logger Creators
===============

Two basic loggers are provided, one logging to file and one to a console. 

.. doxybridge:: s_logger_file_new


.. doxybridge:: s_logger_console_new


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
