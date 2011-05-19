.. index:: ! Error Handling And Debugging (C API)

.. _errdbg/main:

============================
Error Handling And Debugging
============================

The error handling mechanism works on the basis that *all* [#]_ functions are passed
a pointer to an error code variable as a function parameter. If an error occurred
during function execution, then the function can set an appropriate :ref:`error code <error_codes>`
in this pointer.

All calling functions can then check the error pointer and take appropriate action based
on the error code. Each calling function can then add context to the error and in turn
set an error code for it's calling function again, thereby giving a detailed trace from
the origin of the error to the source of the error.

There are two types of errors, *fatal* and *non-fatal* errors. Fatal errors
are usually programming errors, memory errors and so forth, where there are no alternative
options if such an error occured. Non-fatal errors are errors that occured from which
the system can recover or reset to a previous state and continue.

The error handling system as described enables one to have a detailed "backtrace" that has
been logged to some or other place, and therefore makes development and bug tracking a much easier
task.



Set/Clear
=========

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
===========

Macro that tests for errors and sets new context based on test.


.. doxybridge:: S_CHK_ERR
   :type: macro



Warnings
========

Macro that sets warning messages.


.. doxybridge:: S_WARNING 
   :type: macro



Debugging
=========

Macro that sets debug messages.


.. doxybridge:: S_DEBUG
   :type: macro


.. _error_codes:

.. index:: 
   single: Error Handling And Debugging (C API); error codes

Error Codes
===========


.. doxybridge:: s_erc
   :type: typedef enum

.. doxybridge:: s_error_str


.. _debug_levels:

Debug Levels
============

.. doxybridge:: s_dbg_lvl
   :type: typedef enum

.. doxybridge:: s_set_errdbg_level

.. doxybridge:: s_get_errdbg_level



.. [#] There are a few exceptions to this rule, especially where errors might occur before
       the *Error Handling And Debugging* module has been initialized.
