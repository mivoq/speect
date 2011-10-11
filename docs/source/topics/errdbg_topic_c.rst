.. index:: 
   single: Topic Guides; Error handling and Debugging (C API)

.. _errdbg_topic_c:

============================
Error handling and Debugging
============================

The error handling mechanism works on the basis that *all* [#]_
functions are passed a pointer to an error code variable,
:c:type:`s_erc`, as a function parameter. If an error occurred during
function execution, then the function can set an appropriate error
code in this pointer. This of course requires us to *check everything
everywhere*.  Basically, this consists of assuming that every function
call might fail for some reason. This scheme is simple, easy to
understand, straightforward to implement and it is transparent, there
are no ``setjmp``/``longjmp`` type macros that might hide
implementation detail and make debugging a nightmare. It doesn't
require runtime support, is highly portable, and supports
multiple-threads by requiring only the most basic synchronisation
primitives in most code. However, it demands discipline and commitment
to check all function calls when writing, and maintaining code.


See :ref:`Error handling and debugging C API <errdbg_main_c>` for a detailed description
of the API.

Example
=======

Lets look at basic example showing the workings of the error handling system:

.. literalinclude:: ../../../engine/examples/base/errdbg/errdbg_example_short.c
   :language: c
   :tab-width: 4
   :linenos:
   :lines: 36-	


Line 6 defines the error return code variable, :c:type:`s_erc`, and
initializes it to ``S_SUCCESS``.  This initialization setting can also
be achieved with the :c:macro:`S_CLR_ERR` macro. In lines 11-16 Speect
is initialized and an error code is returned and checked.

Line 19 calls the :c:func:`s_strlen` function, with a string and the
memory location of the error code (``&error``). If an error occurs in
the :c:func:`s_strlen` function, it sets the error return code
variable, which is then check by the :c:macro:`S_CHK_ERR` macro in
line 20. The :c:macro:`S_CHK_ERR` macro takes as arguments the memory
location of the error return code, a new error code (type
:c:type:`s_erc`), which can be used to add context, this function
name, and a string describing the error that occurred. This string can
include formatted ``printf`` style arguments.

One can use the :c:macro:`S_CTX_ERR` macro for setting known context errors
inside functions. We can for example define ``s_strlen`` as follows:

.. code-block:: c
   
   size_t s_strlen(const char *s, s_erc *error)
   {
       S_CLR_ERR(error);

       if (s == NULL)
       {
           S_CTX_ERR(error, S_ARGERROR,
	             "s_strlen", "Argument \"s\" is NULL");
	   return 0;
       }

       ...

Which checks if the input argument ``s`` is valid, and if not sets a
context error (``S_ARGERROR``) with the same arguments as described
for :c:macro:`S_CHK_ERR` above. Calling functions of ``s_strlen`` with
invalid arguments can now check which error occurred with
:c:macro:`S_CHK_ERR`.
	
This simple style of error checking allows all calling functions to
check the error return codes of called functions and take appropriate
action based on the error code. Each calling function can then add
context to the error and in turn set an error code for it's calling
function again, thereby giving a detailed trace from the origin of the
error to the source of the error.

The error handling system as described enables one to create a detailed
"backtrace" that has been logged to some or other place, and therefore
makes development and bug tracking a much easier task.

A more detailed example of the error handling mechanism can be found
at ``speect/engine/examples/base/errdbg/errdbg_example.c``, or
:ref:`error handling and debugging example
<examples/c/errdbg_example>`.


.. _errdbg_topic_disable:

Disable
=======

The *Error Handling And Debugging* module can be disabled with the
:cmake:`ERROR_HANDLING` CMake build time switch. This will bypass the
system, and no errors will be checked for or :ref:`logged <logging>`.




.. [#] There are a few exceptions to this rule, especially where errors might occur before
       the *Error Handling And Debugging* module has been initialized.


