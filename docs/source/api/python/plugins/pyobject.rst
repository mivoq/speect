.. _python_plugins_pyobject:


:mod:`speect.pyobject` -- Python Objects Support
================================================

.. module:: speect.pyobject
   :synopsis: Support for Python objects as Speect objects

This module does not add any classes or methods, but allows one to use
Python objects in Speect containers (:class:`speect.SMap`). Therefore
Python objects can be used as features of :class:`speect.SVoice`,
:class:`speect.SUtterance`, :class:`speect.SItem` and any container
type class that takes an ``SObject`` as an argument.
