.. index:: 
   single: Topic Guides; Error handling and Debugging (Python API)

.. _errdbg_topic_python:

============================
Error handling and Debugging
============================

Almost all Python wrapped Speect C implementation that set error
contexts with the Speect Engine :ref:`error handling mechanism
<errdbg_topic_c>` will raise Python :class:`RuntimeError`
exceptions. There are a few functions that raise other Python
exceptions, but these are indicated in the API documentation (see for
example the :ref:`__getitem__ <slist_python_get_item>` Python :class:`list`
operation of the :class:`speect.SList` class).

Disable
=======

The *Error Handling And Debugging* module can be disabled with the
:cmake:`ERROR_HANDLING` CMake build time switch. This will bypass the
system, and no Speect C implementation errors will be checked for or
:ref:`logged <logging>`.

One can query if the Speect Engine error handling mechanism is activated
with the :meth:`speect.errdbg_is_on()` method.
