.. _errdbg_topic/main:

============================
Error handling and Debugging
============================

The error handling mechanism works on the basis that *all* [#]_ functions are passed
a pointer to an error code variable as a function parameter (:c:type:`s_erc`). If an
error occurred during function execution, then the function can set an appropriate error
code in this pointer. 

.. code-block:: c
   
   s_erc error; /* declare the error return code variable */


   S_CLR_ERR(&error); /* clear the error return code */

The macro :c:type:`S_CLR_ERR` will check that ``error`` is not ``NULL`` and set it to ``S_SUCCESS``.
It is good practice to always clear the error return code at the start of a function.





   

All calling functions can then check the error pointer and take appropriate action based
on the error code. Each calling function can then add context to the error and in turn
set an error code for it's calling function again, thereby giving a detailed trace from
the origin of the error to the source of the error.

Let us look at an example of using error handling and debugging.



.. literalinclude:: ../../../engine/examples/base/errdbg/errdbg_example_short.c
   :language: c
   :linenos:
   :lines: 36-	


There are two types of errors, *fatal* and *non-fatal* errors. Fatal errors
are usually programming errors, memory errors and so forth, where there are no alternative
options if such an error occured. Non-fatal errors are errors that occured from which
the system can recover or reset to a previous state and continue.

The error handling system as described enables one to have a detailed "backtrace" that has
been logged to some or other place, and therefore makes development and bug tracking a much easier
task.

.. [#] There are a few exceptions to this rule, especially where errors might occur before
       the *Error Handling And Debugging* module has been initialized.
