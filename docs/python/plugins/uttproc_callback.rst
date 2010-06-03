.. _python_plugins_uttproc_callback:


:mod:`speect.uttproc_callback` -- Utterance Processor Callback
==============================================================


.. automodule:: speect.uttproc_callback
   :synopsis: Utterance Processor callback functionality


This module adds a method to the :class:`speect.SUttProcessor`,
which enables one to define a callback function in Python. This
callback function will then be used whenever the utterance processor's
:meth:`speect.SUttProcessor.run` method is called.

.. automethod:: speect.SUttProcessor.callback()
