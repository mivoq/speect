.. _python_engine_misc:

=============
Miscellaneous
=============

A collection of miscellaneous functions.

|

.. currentmodule:: speect

.. autofunction:: quit()

.. autofunction:: version()

.. autofunction:: sobject_2_pyobject()

.. autofunction:: pyobject_2_sobject()

.. note::
   :meth:`pyobject_2_sobject()` will not work on any Python objects more complex
   than integers, floats or strings. To use anything more complex you will need
   to import the Python module.


.. index:: Speect runtime error, error log

Speect Errors
=============

When a Speect error occurs in Python, a ``RuntimeError`` error
will be raised with a message stating the function name where
the error occured, for example::

    RuntimeError: Speect runtime error at function 'SMap___setitem__'

Speect creates an error log, located at :file:`speect/engine/spct_error.log`
which will then contain a detailed "traceback" of where and why the error occured.
