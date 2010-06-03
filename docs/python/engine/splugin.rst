.. _python_engine_splugin:

=======
SPlugin
=======

This section describes the :class:`SPlugin` class, used for loading Speect
Plug-ins that add some or other functionality.

|

.. currentmodule:: speect

.. autoclass:: SPlugin


.. note::

   The plug-in is unloaded when the ``SPlugin`` object is deleted.


.. note::

   The ``SPlugin`` object inherits from ``SObject`` in the Speect Engine
   and can therefore be used in functions/methods that require parameters
   of type ``SObject``, e.g. :meth:`SVoice.data_set`
