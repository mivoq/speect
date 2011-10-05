.. _python/engine/plugin:

.. index:: ! Plug-ins (Python API)

.. index::
   single: Plug-ins (C API); SPlugin

=======
SPlugin
=======

.. currentmodule:: speect

.. autoclass:: SPlugin


   .. note::

      The plug-in is unloaded when the ``SPlugin`` object is deleted, and no
      other references to the plug-in exists.

   .. note::

      The ``SPlugin`` object inherits from ``SObject`` in the Speect Engine
      and can therefore be used in functions/methods that require parameters
      of type ``SObject``.
