.. _python_plugins_featproc_callback:


:mod:`speect.featproc_cb` -- Feature Processor Callback
=======================================================


.. automodule:: speect.featproc_cb
   :synopsis: Feature Processor callback functionality


This module adds a method to the :class:`speect.SFeatProcessor`,
which enables one to define a callback function in Python. This
callback function will then be used whenever the feature processor's
:meth:`speect.SFeatProcessor.run` method is called.

.. automethod:: speect.SFeatProcessor.callback()
