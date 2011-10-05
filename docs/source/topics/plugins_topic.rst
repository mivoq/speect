.. _plugins_topic/main:

.. index:: 
   single: Topic Guides (C API); Plug-ins

========
Plug-ins
========

Plug-ins are used to extend the functionality of Speect, and as a
mechanism to keep the Speect Engine independent of any linguistic of
acoustic specific code. This extending of Speect is done by defining
new classes (see :doc:`object_system_topic`) in the plug-ins and
registering them with the object system, so that objects of these new
classes can be instantiated and used by other code. See
:doc:`../api/c/engine/pluginmanager/main` for a detailed description of
the API.

Plugins are deployed as dynamic libraries. Dynamic libraries allow
many of the advantages of plug-ins such as hot swapping (reloading a
new implementation without shutting the system), safe extension by
third-party developers (additional functionality without modifying the
core system), and shorter link times.


