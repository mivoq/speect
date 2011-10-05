.. index:: ! Error Handling And Debugging (C API)

.. _errdbg/main:

============================
Error Handling And Debugging
============================

Error handling and debugging module, utilities, macros and
definitions.  See the :doc:`../../../../../topics/errdbg_topic` topic
guide for more details. 

Summary
=======

.. doxybridge-autosummary::
   :nosignatures:

   macro S_NEW_ERR
   macro S_CLR_ERR
   macro S_CTX_ERR
   macro S_FTL_ERR
   macro S_CHK_ERR
   macro S_WARNING
   macro S_DEBUG
   s_set_errdbg_level
   s_get_errdbg_level
   s_set_errdbg_logger
   s_errdbg_on
   s_error_str
   

Definitions
===========

.. index:: 
   single: Error Handling And Debugging (C API); error codes

.. doxybridge:: s_erc
   :type: typedef enum


.. index:: 
   single: Error Handling And Debugging (C API); debugging levels

.. doxybridge:: s_dbg_lvl
   :type: typedef enum


Macros
======

Set/Clear
---------

Macros that sets and clears errors.

.. doxybridge:: S_NEW_ERR
   :type: macro

.. doxybridge:: S_CLR_ERR
   :type: macro

.. doxybridge:: S_CTX_ERR
   :type: macro

.. doxybridge:: S_FTL_ERR
   :type: macro


Check & Set
-----------

Macro that tests for errors and sets new context based on test.

.. doxybridge:: S_CHK_ERR
   :type: macro


Warnings
--------

Macro that sets warning messages.


.. doxybridge:: S_WARNING 
   :type: macro


Debugging
---------

Macro that sets debug messages.


.. doxybridge:: S_DEBUG
   :type: macro


Functions
=========

.. doxybridge:: s_set_errdbg_level

.. doxybridge:: s_get_errdbg_level

.. doxybridge:: s_set_errdbg_logger

.. doxybridge:: s_errdbg_on

.. doxybridge:: s_error_str





